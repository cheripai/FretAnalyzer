#include <QDebug>
#include "calculate.h"


// Extracts A values from grid of data after readGrid()
QVector<double> getAValues(GridStr grid, int nSets, int nReplicates)
{
    const int leftSpacing = 1;
    const int aRow = 1;

    QVector<double> aValues;
    aValues.resize(nSets);

    int j = leftSpacing;
    for(int i = 0; i < nSets; ++i, j += nReplicates)
    {
        aValues[i] = grid[aRow][j].toDouble();
    }

    return aValues;
}


// Extracts X values from grid of data after readGrid()
QVector<double> getXValues(GridStr grid)
{
    const int topSpacing = 2;     // first two rows are not used for x values

    QVector<double> xValues;
    xValues.resize(grid.size()-topSpacing);

    for(int i = topSpacing; i < grid.size(); ++i)
    {
        xValues[i-topSpacing] = grid[i][0].toDouble();
    }

    return xValues;
}


// Extracts set of Y values from grid of data after readGrid()
GridDbl getYValues(GridStr grid)
{
    const int topSpacing = 2;
    const int leftSpacing = 1;

    GridDbl yValues;
    yValues.resize(grid.size()-topSpacing);

    for(int i = topSpacing; i < grid.size(); ++i)
    {
        yValues[i-topSpacing].resize(grid[i].size()-leftSpacing);
        for(int j = leftSpacing; j < grid[i].size(); ++j)
        {
            yValues[i-topSpacing][j-leftSpacing] = grid[i][j].toDouble();
        }
    }

    return yValues;
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
