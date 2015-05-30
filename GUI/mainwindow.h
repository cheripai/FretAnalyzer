#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QVector>
#include "calculate.h"


typedef QVector<QVector<QString> > GridStr;
typedef QVector<QVector<double> > GridDbl;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int nRow,int nSet, int nRep, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void displayResultsFromProcess(int exitCode, QProcess::ExitStatus exitStatus);
    void on_calculateBtn_clicked();
    void on_actionAbout_triggered();
    void on_actionExit_triggered();
    void on_actionNew_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_actionDelete_triggered();
    void on_actionExport_triggered();
    void on_actionExport_Data_triggered();
    void on_actionImport_triggered();

private:
    Ui::MainWindow *ui;
    QProcess *fretPy;
    int nRows;
    int nSets;
    int nReplicates;
    int topSpacing;
    QString plotPath;
    QClipboard *clipboard;
    bool hasEmptyCells();
    void importFromCSV(QString fileName);
    void organizeInputTable(int nRows, int nSets, int nReplicates, bool organizeAll);
    GridStr readGrid();
    void runFretPy(QVector<double> a, QVector<double> x, GridDbl y, int nReplicates, QString plotPath);
    QString selectFile();
    void copy();
    void paste();
    void del();
};

#endif // MAINWINDOW_H
