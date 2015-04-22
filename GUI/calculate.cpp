#include <QDebug>
#include "calculate.h"


// Extracts X values from grid of data after readGrid()
QVector<double> getXValues(GridStr grid)
{
    int topSpacing = 2;     // first two rows are not used for x values

    QVector<double> xValues;
    xValues.resize(grid[0].size()-topSpacing);

    for(int i = topSpacing; i < grid[0].size(); ++i)
    {
        xValues[i-topSpacing] = grid[i][0].toDouble();
    }

    return xValues;
}



// Calls external python script to perform nonlinear regression
QByteArray runFretPy(QString x, QString y, QString a, QString plotPath)
{
    QProcess fretPy;
    QStringList arguments;
    QByteArray result;
    QByteArray error;

    if(plotPath != "")
    {
        arguments << "-p" << plotPath;
    }

    // Starts python script and writes data to stdin of script
    fretPy.start("./fret.py", arguments);
    fretPy.write(x.toLatin1().append('\n'));
    fretPy.write(y.toLatin1().append('\n'));
    fretPy.write(a.toLatin1().append('\n'));
    fretPy.closeWriteChannel();
    fretPy.waitForFinished();

    result = fretPy.readAll();
    error = fretPy.readAllStandardError();
    fretPy.close();

    if(error != "")
    {
        qDebug() << error;
        result = error;
    }

    return result;
}
