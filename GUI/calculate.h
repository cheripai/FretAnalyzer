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
QString vecToString(QVector<double> v);
QString gridToString(GridDbl g);


#endif // CALCULATE_H
