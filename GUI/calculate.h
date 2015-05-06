#ifndef CALCULATE_H
#define CALCULATE_H

#include <QProcess>
#include <QString>
#include <QVector>

typedef QVector<QVector<QString> > GridStr;
typedef QVector<QVector<double> > GridDbl;


QByteArray runFretPy(QString x, QString y, QString a, QString plotPath);
QVector<double> getXValues(GridStr grid);
GridDbl getYValues(GridStr grid);


#endif // CALCULATE_H
