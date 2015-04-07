#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void read(QString fileName);
    ~MainWindow();

private slots:
    void on_calculateBtn_clicked();
    void on_actionAbout_triggered();
    void on_actionExit_triggered();
    void on_fileOpen1_clicked();
    void on_fileOpen2_clicked();
    void on_fileOpen3_clicked();
    void on_fileOpen4_clicked();

private:
    Ui::MainWindow *ui;
    QByteArray runFretPy(QString x, QString y, QString a, QString plotPath);
    QString getXValues();
    QString selectFile();
    QString blank414;
    QString blank475;
    QString data414;
    QString data475;
    QString plotPath;
};

#endif // MAINWINDOW_H
