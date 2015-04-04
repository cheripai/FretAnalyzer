#include <QDebug>
#include <QDir>
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

void MainWindow::on_calculateButton_clicked()
{
    QProcess fretPy;
    QStringList arguments;
    arguments << "-i" << "../../test/data1.txt" << "-p" << "../plot.png";
    fretPy.start("../../src/fret.py", arguments);
    fretPy.waitForFinished();
    QByteArray result = fretPy.readAll();
    fretPy.close();
    qDebug() << result;

    QString plotPath = "../plot.png";
    QImage plot(plotPath);
    ui->plotFrame->setPixmap(QPixmap::fromImage(plot));
}
