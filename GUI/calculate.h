#ifndef CALCULATE_H
#define CALCULATE_H

#include <QProcess>
#include <QString>
#include <QVector>

typedef QVector<QVector<QString> > GridStr;
typedef QVector<QVector<double> > GridDbl;


QVector<double> getAValues(GridStr grid, int nSets, int nReplicates);
QVector<double> getXValues(GridStr grid);
GridDbl getYValues(GridStr grid);
QByteArray runFretPy(QString x, QString y, QString a, QString plotPath);


#endif // CALCULATE_H
