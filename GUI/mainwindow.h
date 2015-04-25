#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QVector>


typedef QVector<QVector<QString> > GridStr;
typedef QVector<QVector<double> > GridDbl;


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
    void on_calculateBtn_clicked();
    void on_actionAbout_triggered();
    void on_actionExit_triggered();
    void on_actionNew_triggered();
    void on_actionCopy_triggered();

    void on_actionDelete_triggered();

private:
    Ui::MainWindow *ui;
    int nSets;
    int nReplicates;
    QString plotPath;
    QClipboard *clipboard;
    void organizeInputTable(int nSets, int nReplicates);
    GridStr readGrid();
    QString selectFile();
    //void paste();
    void copy();
    void del();
};

#endif // MAINWINDOW_H
