#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include "mainwindow.h"
#include "ui_mainwindow.h"

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

// TODO: read filenames from textboxes when calculate is clicked (in case user manually typed in path)
void MainWindow::on_calculateBtn_clicked()
{
    QProcess fretPy;
    QStringList arguments;
    arguments << "-i" << "../data1.txt" << "-p" << "plot.png";
    fretPy.start("./fret.py", arguments);
    fretPy.waitForFinished();
    QByteArray result = fretPy.readAll();
    fretPy.close();
    ui->resultsFrame->setText(result);

    QString plotPath = "plot.png";
    QImage plot(plotPath);
    ui->plotFrame->setPixmap(QPixmap::fromImage(plot));
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
}


void MainWindow::on_fileOpen2_clicked()
{
    blank475 = selectFile();
    ui->fileName2->setText(blank475);
}


void MainWindow::on_fileOpen3_clicked()
{
    data414 = selectFile();
    ui->fileName3->setText(data414);
}


void MainWindow::on_fileOpen4_clicked()
{
    data475 = selectFile();
    ui->fileName4->setText(data475);
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
    QMessageBox::information(this, "About","FretAnalyzer Version: 0.0\nBy: Dat Do, Patrick Cammall, Shilpa Chirackel, and Hui Yang");
}

