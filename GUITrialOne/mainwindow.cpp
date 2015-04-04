#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parser.h"

#include <QFile>
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
        // read in file name
        QString blank414 = ui->textEdit->toPlainText();
        QString blank475 =  ui->textEdit_2->toPlainText();
        QString data414 = ui->textEdit_3->toPlainText();
        QString data475 = ui->textEdit_4->toPlainText();

        // test read in file name
        ui->label->setText(blank414);
        ui->label_2->setText(blank475);
        ui->label_3->setText(data414);
        ui->label_4->setText(data475);

        // main functionality
         //GridDbl flaa = subtractBlanks(getDataBlock(data475,530),getDataBlock(blank475,530));
       // GridDbl fldd = subtractBlanks(getDataBlock(data414,530),getDataBlock(blank414,475));
       // GridDbl emTotal = subtractBlanks(getDataBlock(data414,530),getDataBlock(blank414,530));
       // GridDbl emfret = calculate_emFret(fldd, flaa, emTotal);


          // below is writing the result
          QVector<double> x = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0.5, 0};

          readFile(blank414);
}
