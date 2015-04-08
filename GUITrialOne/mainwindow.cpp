#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parser.h"

#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
        GridDbl flaa = subtractBlanks(getDataBlock(data475,530),getDataBlock(blank475,530));
       GridDbl fldd = subtractBlanks(getDataBlock(data414,475),getDataBlock(blank414,475));
      GridDbl emTotal = subtractBlanks(getDataBlock(data414,530),getDataBlock(blank414,530));
       GridDbl emfret = calculate_emFret(fldd, flaa, emTotal);
    statusBar()->showMessage(tr("Emfret calculated"), 2000);
    //use Qprocess to generate the graph
        //  readFile(blank414);
}


void MainWindow::loadFile(const QString &fileName)
{
    statusBar()->showMessage(fileName+tr("File loaded"), 2000);
}

bool MainWindow::saveFile(const QString &fileName)
{
    writeFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}
bool MainWindow::saveAs()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    QStringList files;
    if (dialog.exec())
        files = dialog.selectedFiles();
    else
        return false;

    return saveFile(files.at(0));
}

QString MainWindow::selectFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
        tr("Text Files (*.txt)"));

    return fileName;
}
void MainWindow::on_fileOpen1_clicked()
{
    blank414 = selectFile();
    ui->fileName1->setText(blank414);
    loadFile(blank414);
}

void MainWindow::on_fileOpen2_clicked()
{
    blank475 = selectFile();  
    ui->fileName2->setText(blank475);
    loadFile(blank475);
}

void MainWindow::on_fileOpen3_clicked()
{
    data414 = selectFile();
    ui->fileName3->setText(data414);
    loadFile(data414);
}

void MainWindow::on_fileOpen4_clicked()
{
    data475 = selectFile();
    ui->fileName4->setText(data475);
    loadFile(data475);
}

void MainWindow::on_fileSave_clicked()
{
    saveAs();
}
