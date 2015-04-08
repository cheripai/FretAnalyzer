#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    bool saveAs();

    void on_pushButton_clicked();
    void on_fileOpen1_clicked();
    void on_fileOpen2_clicked();
    void on_fileOpen3_clicked();
    void on_fileOpen4_clicked();

    void on_fileSave_clicked();

private:
    Ui::MainWindow *ui;
    QString selectFile();

    QString blank414;
    QString blank475;
    QString data414;
    QString data475;

    QString waveLength1;
    QString waveLength2;
    QString waveLength3;
    QString waveLength4;
};

#endif // MAINWINDOW_H
