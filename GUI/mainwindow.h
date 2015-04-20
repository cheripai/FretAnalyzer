#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QString>
#include <QVector>

typedef QVector<QVector<QString> > GridStr;

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
    void on_actionNew_triggered();

private:
    Ui::MainWindow *ui;
    QByteArray runFretPy(QString x, QString y, QString a, QString plotPath);
    QString selectFile();
    QString plotPath;
    GridStr readGrid();
    int nSets;
    int nReplicates;
    void organizeInputTable(int nSets, int nReplicates);
};

#endif // MAINWINDOW_H
