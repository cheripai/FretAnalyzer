#include <QClipboard>
#include <QDebug>
#include <QFileDialog>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QPainter>
#include <QPrinter>
#include "dialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(int nRow, int nSet, int nRep, QString units, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scatterPath = "scatter.png";
    barPath = "bar.png";
    nRows = nRow;
    nSets = nSet;
    nReplicates = nRep;
    this->units = units;
    clipboard = QApplication::clipboard();
    topSpacing = 3;
    organizeInputTable(nRows, nSets, nReplicates, true);
}


MainWindow::~MainWindow()
{
    QFile::remove(scatterPath);
    QFile::remove(barPath);
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

    QJsonDocument jsonResponse = QJsonDocument::fromJson(result);
    QJsonObject jsonObj = jsonResponse.object();
    QStringList keys = jsonObj.keys();

    QFont bold;
    bold.setBold(true);

    ui->resultsTable->setItem(0, 0, new QTableWidgetItem(QString("Donor [%1]").arg(units)));
    ui->resultsTable->setItem(0, 1, new QTableWidgetItem(QString("%1 [%2]").arg("Kd").arg(units)));
    ui->resultsTable->setItem(0, 2, new QTableWidgetItem("Std. Error"));
    ui->resultsTable->setItem(0, 3, new QTableWidgetItem("EmFRETMAX (RFU)"));
    ui->resultsTable->setItem(0, 4, new QTableWidgetItem("Std. Error"));
    ui->resultsTable->setItem(0, 5, new QTableWidgetItem("R-Squared"));

    for(int i = 0; i < 6; ++i)
    {
        ui->resultsTable->item(0, i)->setBackgroundColor(Qt::gray);
        ui->resultsTable->item(0, i)->setFont(bold);
    }

    for(int i = 0; i < keys.length(); ++i)
    {
        QJsonArray values = jsonObj[keys[i]].toArray();
        for(int j = 0; j < values.size(); ++j)
        {
            ui->resultsTable->setItem(i+1, j, new QTableWidgetItem(QString("%1").arg(values[j].toDouble())));
        }
    }

    // Presents plot on GUI
    QImage scatter(scatterPath);
    ui->plotFrame->setPixmap(QPixmap::fromImage(scatter));
    QImage bar(barPath);
    ui->barFrame->setPixmap(QPixmap::fromImage(bar));
    ui->statusBar->showMessage(tr("Finished."));

    delete fretPy;
}


bool MainWindow::hasEmptyCells()
{
    int j = 1;
    for(int i = 0; i < nSets; ++i, j += nReplicates)
    {
        if(ui->inputTable->item(topSpacing-1, j) == NULL)
        {
            QMessageBox errorBox;
            errorBox.critical(this, "Error", QString("Error: empty cell at (%1, %2)").arg(topSpacing).arg(j+1));
            return true;
        }

    }
    for(int i = topSpacing; i < ui->inputTable->rowCount(); ++i)
    {
        for(int j = 0; j < ui->inputTable->columnCount(); ++j)
        {
            if(ui->inputTable->item(i, j) == NULL)
            {
                QMessageBox errorBox;
                errorBox.critical(this, "Error", QString("Error: empty cell at (%1, %2)").arg(i+1).arg(j+1));
                return true;
            }
        }
    }
    return false;
}


void MainWindow::importFromCSV(QString fileName)
{
    QFile file(fileName);
    int rows = 0;

    // This determines how many rows is needed for the inputTable
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        while(!in.atEnd())
        {
            QString fileLine = in.readLine();
            ++rows;
        }
        file.close();
    }

    nRows = rows - topSpacing;
    ui->inputTable->setRowCount(rows);

    if(file.open(QIODevice::ReadOnly))
    {

        int lineindex = 0;                     // file line counter
        QTextStream in(&file);                 // read to text stream

        while (!in.atEnd())
        {
            // read one line from textstream(separated by "\n")
            QString fileLine = in.readLine();

            // parse the read line into separate pieces(tokens) with "," as the delimiter
            QStringList lineToken = fileLine.split(",", QString::KeepEmptyParts);

            // load parsed data to model accordingly
            for (int j = 0; j < lineToken.size(); j++) {
                QString value = lineToken.at(j);
                QTableWidgetItem *item = new QTableWidgetItem(value);
                ui->inputTable->setItem(lineindex, j, item);
            }
            ++lineindex;
        }
        file.close();
    }
}


// Sets up input table format based on number of sets and replicates determined in startup wizard
void MainWindow::organizeInputTable(int nRows, int nSets, int nReplicates, bool organizeAll)
{
    nRows = nRows < topSpacing + 1 ? topSpacing + 1 : nRows + topSpacing;
    ui->inputTable->setColumnCount(nSets*nReplicates+1);
    ui->inputTable->setRowCount(nRows);

    QFont font;
    font.setBold(true);

    QTableWidgetItem *gray = new QTableWidgetItem("");
    gray->setBackgroundColor(Qt::gray);
    ui->inputTable->setItem(1, 0, gray);

    QTableWidgetItem *topLabel = new QTableWidgetItem("Donor Concentrations (Y)");
    topLabel->setTextAlignment(Qt::AlignCenter);
    topLabel->setBackgroundColor(Qt::gray);
    ui->inputTable->setItem(0, 0, topLabel);
    ui->inputTable->item(0, 0)->setFont(font);
    ui->inputTable->setSpan(0, 0, 1, ui->inputTable->columnCount());

    QTableWidgetItem *x = new QTableWidgetItem("Acceptor Concentrations (X)");
    x->setTextAlignment(Qt::AlignCenter);
    x->setBackgroundColor(Qt::gray);
    ui->inputTable->setItem(2, 0, x);
    ui->inputTable->item(2, 0)->setFont(font);

    for(int i = 0; i < nSets; ++i)
    {
        QTableWidgetItem *y = new QTableWidgetItem(QString("Y%1").arg(i+1));
        y->setTextAlignment(Qt::AlignCenter);
        y->setBackgroundColor(Qt::gray);
        QTableWidgetItem *a = new QTableWidgetItem();
        a->setTextAlignment(Qt::AlignCenter);
        ui->inputTable->setItem(1, i * nReplicates + 1, y);
        ui->inputTable->setSpan(1, i * nReplicates + 1, 1, nReplicates);

        // This will wipe all a values, which is why we have a special flag for it
        // We do not want this function to wipe the a values on importing from csv
        if(organizeAll)
        {
            ui->inputTable->setItem(2, i * nReplicates + 1, a);
            ui->inputTable->setSpan(2, i * nReplicates + 1, 1, nReplicates);
        }
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
void MainWindow::runFretPy(QVector<double> a, QVector<double> x, GridDbl y, int nReplicates, QString scatterPath, QString barPath)
{
    QStringList arguments;
    QString aStr = vecToString(a);
    QString xStr = vecToString(x);
    QString yStr = gridToString(y);

    if(scatterPath != "" && barPath != "" && ui->graphCheckBox->isChecked())
    {
        arguments << "-s" << scatterPath;
        arguments << "-b" << barPath;
    }
    if(units == "μM")
    {
        arguments << "-u" << "u";
    }
    else if(units == "nM")
    {
        arguments << "-u" << "n";
    }
    else if(units == "pM")
    {
        arguments << "-u" << "p";
    }
    else
    {
        qDebug() << "Error incorrect unit: " << units;
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
    if(hasEmptyCells())
    {
        return;
    }

    ui->statusBar->showMessage(tr("Calculating..."));
    QFile::remove(scatterPath);    // removes old plot (if it exists) prior to new generation
    QFile::remove(barPath);    // removes old plot (if it exists) prior to new generation

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

    runFretPy(a, x, y, nReplicates, scatterPath, barPath);
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
    QMessageBox::information(this, "About","FretAnalyzer Version: 1.3\nBy: Dat Do, Hui Yang, and Patrick Cammall");
}


// Clears all existing data
void MainWindow::on_actionNew_triggered()
{
    QFile::remove(scatterPath);
    QFile::remove(barPath);
    QImage plot(scatterPath);
    ui->inputTable->clearContents();
    ui->resultsTable->clearContents();
    ui->plotFrame->setPixmap(QPixmap::fromImage(plot));
    ui->barFrame->setPixmap(QPixmap::fromImage(plot));

    for(int i = 0; i < ui->inputTable->rowCount(); ++i)
    {
        for(int j = 0; j < ui->inputTable->columnCount(); ++j)
        {
            ui->inputTable->setSpan(i, j, 1, 1);
        }
    }

    Dialog dialog;
    dialog.show();
    if(dialog.exec() == QDialog::Accepted)
    {
        nRows = dialog.rowValueSpinBox->value();
        nReplicates = dialog.yvalueSpinBox->value();
        nSets = dialog.numSetsSpinBox->value();
    }
    organizeInputTable(nRows, nSets, nReplicates, true);
}

void MainWindow::on_actionCopy_triggered()
{
    if(ui->inputTable->selectedRanges().size() != 0)
    {
        copy();
    }
    else
    {
        QMessageBox errorBox;
        errorBox.critical(this, "Error", "Error: no cells in table selected");
    }
}

void MainWindow::on_actionPaste_triggered()
{
    if(ui->inputTable->selectedRanges().size() != 0)
    {
        paste();
    }
    else
    {
        QMessageBox errorBox;
        errorBox.critical(this, "Error", "Error: no cells in table selected");
    }
}

void MainWindow::on_actionDelete_triggered()
{
    if(ui->inputTable->selectedRanges().size() != 0)
    {
       del();
    }
    else
    {
        QMessageBox errorBox;
        errorBox.critical(this, "Error", "Error: no cells in table selected");
    }
}

void MainWindow::on_actionExport_triggered()
{
    exportImage(scatterPath);
}


void MainWindow::on_actionExport_Bar_triggered()
{
    exportImage(barPath);
}


void MainWindow::exportImage(QString path)
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), "untitled", tr("PNG image (*.png)"));
    QFileInfo checkFile(path);
    if(checkFile.exists() && checkFile.isFile())
    {
        QFile::copy(path, filename.append(".png"));
    }
    else
    {
        QMessageBox errorBox;
        errorBox.critical(this, "Error", "Error: plot does not exist");
    }
}


void MainWindow::on_actionExport_Data_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), "untitled", tr("CSV file (*.csv)")).append(".csv");
    QFile file(filename);
    if(file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        QTableWidget * table = ui->resultsTable;
        for(int i = 0; i < table->rowCount(); ++i)
        {
            for(int j = 0; j < table->columnCount(); ++j)
            {
                if(table->item(i, j))
                {
                    stream << table->item(i, j)->text() << ',';
                }
            }
            if(table->item(i+1, 0))
            {
                stream << '\n';
            }
        }
    }
    file.close();
}


void MainWindow::on_actionImport_triggered()
{
    QString fileName = selectFile();
    importFromCSV(fileName);
    organizeInputTable(nRows, nSets, nReplicates, false);
}
