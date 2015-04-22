#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
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
    void read(QString fileName);
    ~MainWindow();

private slots:
    void on_calculateBtn_clicked();
    void on_actionAbout_triggered();
    void on_actionExit_triggered();
    void on_actionNew_triggered();

private:
    Ui::MainWindow *ui;
    int nSets;
    int nReplicates;
    QString plotPath;
    void organizeInputTable(int nSets, int nReplicates);
    QByteArray runFretPy(QString x, QString y, QString a, QString plotPath);
    GridStr readGrid();
    QVector<double> getXValues(GridStr grid);
    QString selectFile();
};

#endif // MAINWINDOW_H
