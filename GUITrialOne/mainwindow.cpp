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
    plotPath = "plot.png";
}

MainWindow::~MainWindow()
{
    QFile::remove(plotPath);
    delete ui;
}

// TODO: read filenames from textboxes when calculate is clicked (in case user manually typed in path)
void MainWindow::on_calculateBtn_clicked()
{
    ui->statusBar->showMessage(tr("Calculating..."));

    QString x = ui->xValText->toPlainText();
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
}


QByteArray MainWindow::runFretPy(QString x, QString y, QString a, QString plotPath)
{
    QProcess fretPy;
    QStringList arguments;
    QByteArray result;
    QByteArray error;

    if(plotPath != "")
    {
        arguments << "-p" << plotPath;
    }

    // Starts python script and writes data to stdin of script
    fretPy.start("./fret.py", arguments);
    fretPy.write(x.toLatin1().append('\n'));
    fretPy.write(y.toLatin1().append('\n'));
    fretPy.write(a.toLatin1().append('\n'));
    fretPy.closeWriteChannel();
    fretPy.waitForFinished();

    result = fretPy.readAll();
    error = fretPy.readAllStandardError();
    fretPy.close();

    if(error != "")
    {
        qDebug() << error;
        result = error;
    }

    return result;
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

