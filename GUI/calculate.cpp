#include <QDebug>
#include "calculate.h"
#include "mainwindow.h"


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


// converts vector<double> to string with each value separated by spaces
QString vecToString(QVector<double> v)
{
    QString result;
    for(int i = 0; i < v.size(); ++i)
    {
        result.append(QString::number(v[i])).append(' ');
    }
    return result.append('\n');
}


// converts GridDbl to string separated by spaces and newlines
QString gridToString(GridDbl g)
{
    QString result;
    for(int i = 0; i < g[0].size(); ++i)
    {
        for(int j = 0; j < g.size(); ++j)
        {
            result.append(QString::number(g[j][i])).append(' ');
        }
        result.append('\n');
    }
    return result;
}
