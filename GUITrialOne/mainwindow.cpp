#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parser.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
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

    //use Qprocess to generate the graph

}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    // process file

    statusBar()->showMessage(tr("File loaded"), 2000);  //show a message to notify the user
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);

    out << "12345";         // test writing!

    statusBar()->showMessage(tr("File saved"), 2000);  //show a message to notify the user
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
