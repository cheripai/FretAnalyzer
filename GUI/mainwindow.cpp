#include <QClipboard>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QPrinter>
#include "dialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(int nRow, int nSet, int nRep, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    plotPath = "plot.png";
    nRows = nRow;
    nSets = nSet;
    nReplicates = nRep;
    clipboard = QApplication::clipboard();
    organizeInputTable(nRows, nSets, nReplicates);
}


MainWindow::~MainWindow()
{
    QFile::remove(plotPath);
    delete ui;
}


// Displays results from QProcess fretPy in results tab
void MainWindow::displayResultsFromProcess(int exitCode, QProcess::ExitStatus exitStatus)
{
    QByteArray result = fretPy->readAll();
    QByteArray error = fretPy->readAllStandardError();
    fretPy->close();

    if(error != "")
    {
        qDebug() << error;
        result = error;
    }

    ui->resultsFrame->setText(result);

    // Presents plot on GUI
    QImage plot(plotPath);
    ui->plotFrame->setPixmap(QPixmap::fromImage(plot));
    ui->statusBar->showMessage(tr("Finished."));

    delete fretPy;
}


// TODO: Add error checking for sizes of input and user set
void MainWindow::importFromCSV(QString fileName)
{
    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly)) {

        int lineindex = 0;                     // file line counter
        QTextStream in(&file);                 // read to text stream

        while (!in.atEnd()) {

            // read one line from textstream(separated by "\n")
            QString fileLine = in.readLine();

            // parse the read line into separate pieces(tokens) with "," as the delimiter
            QStringList lineToken = fileLine.split(",", QString::SkipEmptyParts);

            // load parsed data to model accordingly
            for (int j = 0; j < lineToken.size(); j++) {
                QString value = lineToken.at(j);
                QTableWidgetItem *item = new QTableWidgetItem(value);
                ui->inputTable->setItem(lineindex, j, item);
            }

            lineindex++;
        }

        file.close();
    }
}


// Sets up input table format based on number of sets and replicates determined in startup wizard
void MainWindow::organizeInputTable(int nRows, int nSets, int nReplicates)
{
    nRows = nRows < 20 ? 20 : nRows;
    ui->inputTable->setColumnCount(nSets *nReplicates + 1);
    ui->inputTable->setRowCount(nRows);

    QTableWidgetItem *gray = new QTableWidgetItem("");
    gray->setBackgroundColor(Qt::gray);
    ui->inputTable->setItem(0, 0, gray);

    QTableWidgetItem *x = new QTableWidgetItem("X");
    x->setTextAlignment(Qt::AlignCenter);
    x->setBackgroundColor(Qt::gray);
    ui->inputTable->setItem(1, 0, x);

    for(int i = 0; i < nSets; ++i)
    {
        QTableWidgetItem *y = new QTableWidgetItem(QString("Y%1").arg(i+1));
        y->setTextAlignment(Qt::AlignCenter);
        y->setBackgroundColor(Qt::gray);
        QTableWidgetItem *a = new QTableWidgetItem();
        a->setTextAlignment(Qt::AlignCenter);
        ui->inputTable->setItem(0, i * nReplicates + 1, y);
        ui->inputTable->setSpan(0, i * nReplicates + 1, 1, nReplicates);
        ui->inputTable->setItem(1, i * nReplicates + 1, a);
        ui->inputTable->setSpan(1, i * nReplicates + 1, 1, nReplicates);
    }
}


// returns 2d vector<QString> of data currently in inputTable
GridStr MainWindow::readGrid()
{
    GridStr grid;
    int rows = ui->inputTable->rowCount();
    int columns = ui->inputTable->columnCount();

    grid.resize(rows);

    for(int i = 0; i < rows; ++i)
    {
        grid[i].resize(columns);
        for(int j = 0; j < columns; ++j)
        {
            if(ui->inputTable->item(i, j))
            {
                grid[i][j] = ui->inputTable->item(i, j)->text();
            }
            else
            {
                grid[i][j] = '\0';
            }
        }
    }

    return grid;
}


// Calls external python script to perform nonlinear regression
void MainWindow::runFretPy(QVector<double> a, QVector<double> x, GridDbl y, int nReplicates, QString plotPath)
{
    QStringList arguments;
    QString aStr = vecToString(a);
    QString xStr = vecToString(x);
    QString yStr = gridToString(y);

    if(plotPath != "")
    {
        arguments << "-p" << plotPath;
    }

    fretPy = new QProcess(this);
    connect(fretPy, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(displayResultsFromProcess(int, QProcess::ExitStatus)));


    // Starts python script and writes data to stdin of script
    fretPy->start("./fret.py", arguments);
    fretPy->write(xStr.toLatin1());
    fretPy->write(QString::number(nReplicates).toLatin1().append('\n'));
    fretPy->write(aStr.toLatin1());
    fretPy->write(yStr.toLatin1());
    fretPy->closeWriteChannel();
}


// returns filepath of file selection from wizard
QString MainWindow::selectFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
        tr("CSV Files (*.csv)"));

    return fileName;
}


void MainWindow::copy()
{
    QTableWidget *view = ui->inputTable;
    QAbstractItemModel *model = view->model();
    QItemSelectionModel *selection = view->selectionModel();
    QModelIndexList indexes = selection->selectedIndexes();
    qSort(indexes);

    QString selected_text;
    QModelIndex current;
    QModelIndex previous = indexes.first();
    indexes.removeFirst();

    foreach(current, indexes)
    {
        QVariant data = model->data(previous);
        QString text = data.toString();
        selected_text.append(text);

        if(current.row() != previous.row())
        {
            selected_text.append('\n');
        }
        else
        {
            selected_text.append('\t');
        }
        previous = current;
    }
    selected_text.append(model->data(current).toString());
    selected_text.append('\n');
    clipboard->setText(selected_text);
}


void MainWindow::paste()
{
    QTableWidget *view = ui->inputTable;
    QItemSelectionModel *selection = view->selectionModel();
    QModelIndexList indexes = selection->selectedIndexes();

    QString selected_text = clipboard->text();
    QStringList cells = selected_text.split(QRegExp(QLatin1String("\\n|\\t")));

    int rows = selected_text.count(QLatin1Char('\n'));
    int cols = cells.size() / rows;

    int cell = 0;
    int r = selection->currentIndex().row();
    int c = selection->currentIndex().column();
    for(int row = r; row < rows + r; ++row)
    {
        for(int col = c; col < cols + c; ++col, ++cell)
        {
            QTableWidgetItem *newItem = new QTableWidgetItem(cells[cell]);
            view->setItem(row, col, newItem);
        }
    }
}


void MainWindow::del()
{
    QTableWidget *view = ui->inputTable;
    QItemSelectionModel *selection = view->selectionModel();
    QModelIndexList indexes = selection->selectedIndexes();
    QModelIndex current = indexes.first();

    foreach(current, indexes)
    {
        view->setItem(current.row(), current.column(), NULL);
    }
}


void MainWindow::on_calculateBtn_clicked()
{
    ui->statusBar->showMessage(tr("Calculating..."));
    QFile::remove(plotPath);    // removes old plot (if it exists) prior to new generation

    GridStr grid = readGrid();
    QVector<double> a = getAValues(grid, nSets, nReplicates);
    QVector<double> x = getXValues(grid);
    GridDbl y = getYValues(grid);

    if(x.length() != y.length())
    {
        qDebug() << "Error: x and y have different sizes";
    }
    if(a.length() != nSets)
    {
        qDebug() << "Error: size of a and nSets have different values";
    }

    runFretPy(a, x, y, nReplicates, plotPath);
}


void MainWindow::on_actionExit_triggered()
{
    QMessageBox messageBox;
    messageBox.setWindowTitle(tr("Quit Dialog"));
    messageBox.setText(tr("Do you really want to quit?"));
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    messageBox.setDefaultButton(QMessageBox::No);
    if(messageBox.exec() == QMessageBox::Yes)
        qApp->quit();
}


void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "About","FretAnalyzer Version: 1.0\nBy: Dat Do, Hui Yang, and Patrick Cammall");
}


// Clears all existing data
void MainWindow::on_actionNew_triggered()
{
    ui->resultsFrame->setText("");
    QFile::remove(plotPath);
    QImage plot(plotPath);
    ui->inputTable->clearContents();
    ui->plotFrame->setPixmap(QPixmap::fromImage(plot));

    Dialog dialog;
    dialog.show();
    if(dialog.exec() == QDialog::Accepted)
    {
        nRows = dialog.rowValueSpinBox->value();
        nReplicates = dialog.yvalueSpinBox->value();
        nSets = dialog.numSetsSpinBox->value();
    }
    organizeInputTable(nRows, nSets, nReplicates);
}

void MainWindow::on_actionCopy_triggered()
{
    copy();
}

void MainWindow::on_actionPaste_triggered()
{
    paste();
}

void MainWindow::on_actionDelete_triggered()
{
   del();
}

void MainWindow::on_actionExport_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), "untitled", tr("PDF Document (*.pdf)"));
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filename.append(".pdf"));

    ui->resultsFrame->print(&printer);

    QPixmap plot(plotPath);
    QPainter painter(&printer);
    painter.drawPixmap(0, 100, 700, 500, plot);
    painter.end();
}

void MainWindow::on_actionImport_triggered()
{
    QString fileName = selectFile();
    importFromCSV(fileName);
    organizeInputTable(nRows, nSets, nReplicates);
}
