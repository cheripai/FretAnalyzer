#include <QClipboard>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QPrinter>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "calculate.h"


MainWindow::MainWindow(int nRow, int nSet, int nRep, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    plotPath = "plot.png";
    if(nRow <20)   // ensure at least 20 rows
        nRows =20;
    else{
        nRows = nRow;
    }
    nSets = nSet;
    nReplicates = nRep;
    clipboard = QApplication::clipboard();
    organizeInputTable( nRows, nSets, nReplicates);
}


MainWindow::~MainWindow()
{
    QFile::remove(plotPath);
    delete ui;
}


// Sets up input table format based on number of sets and replicates determined in startup wizard
void MainWindow::organizeInputTable(int nRows, int nSets, int nReplicates)
{
    ui->inputTable->setColumnCount(nSets *nReplicates +1);
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
        ui->inputTable->setItem(0, i * nReplicates + 1, y);
        ui->inputTable->setSpan(0, i * nReplicates + 1, 1, nReplicates);
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


// returns filepath of file selection from wizard
QString MainWindow::selectFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
        tr("Text Files (*.txt)"));

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

    QString x = "10 9 8 7 6 5 4 3 2 1 0.5 0";
    // FIXME: Grab values from C++ functions instead of hardcoded
    QString y = "588971.60 629608.20 601458.60 591147.50 521218.80 509342.30 497718.90 430004.60 440683.00 279258.60 198418.50 -21109.88";
    QString a = "1";

    // ASSERT that x y and a are of the right length and properly formatted
    QByteArray result = runFretPy(x, y, a, plotPath);
    ui->resultsFrame->setText(result);

    // Presents plot on GUI
    QImage plot(plotPath);
    ui->plotFrame->setPixmap(QPixmap::fromImage(plot));
    ui->statusBar->showMessage(tr("Finished."));

    // FIXME: Testing function calls
    GridStr grid = readGrid();
    QVector<double> xValues = getXValues(grid);
    GridDbl yValues = getYValues(grid);
    if(xValues.length() != yValues.length()-1)
    {
        qDebug() << "Error: x and y have different sizes";
    }
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
    QMessageBox::information(this, "About","FretAnalyzer Version: 0.1\nBy: Dat Do, Hui Yang, and Patrick Cammall");
}


// Clears all existing data
void MainWindow::on_actionNew_triggered()
{
    ui->resultsFrame->setText("");
    QFile::remove(plotPath);
    QImage plot(plotPath);
    ui->inputTable->clearContents();
    ui->plotFrame->setPixmap(QPixmap::fromImage(plot));
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
