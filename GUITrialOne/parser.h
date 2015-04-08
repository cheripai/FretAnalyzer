#ifndef PARSER_H
#define PARSER_H

#include <QFile>
#include <QDebug>
#include <QTextStream>

typedef QVector< QVector<QString> > GridStr;
typedef QVector< QVector<double> > GridDbl;

GridDbl emFret;
// returns a 2d vector of strings from raw text file output from machine
// the file has a fixed format, so we can parse it accordingly.
GridStr readFile(const QString & file){   

    // open a file
    QFile filein(file);
    if (!filein.open(QFile::ReadOnly ))
     {
        qDebug()<< " can not read";
        exit(-1);
    }

    GridStr data;
    QTextStream in(&filein);

    while (!in.atEnd()) {
          QString line = in.readLine(); // read a line
          QVector<QString> tempStr;
          QStringList list1 = line.split("\t"); // split by \t

          for (int i = 0; i < list1.size(); ++i)
          {
              tempStr.push_back( list1.at(i));
          }
          data.push_back(tempStr);   // pushback to a 2d string vector      
    }

    filein.close();
    return data;
}
GridDbl getDataBlock(const QString & file, int waveLength)
{
        GridStr data = readFile(file);
        const int dataHeight = 16;
        const int dataWidth = 24;

        // Gets following data from corresponding cell in data array
        int increment = data.at(1).at(14).toInt();
        int maxWaveLength = data.at(1).at(13).toInt();
        int minWaveLength = data.at(1).at(12).toInt();

        if(waveLength > maxWaveLength
        || waveLength < minWaveLength
        || waveLength % increment != 0)
        {
            qDebug() << "ERROR: given wavelength is out of bounds" << endl;
            exit(-2);
        }

        // Calculates desired row of wave length based on format of input file
        int startRow = 3 + (dataHeight + 1) * (waveLength - minWaveLength) / increment;
        // First two columns of input file are irrelevant to our desired data
        int startCol = 2;

        if( data.at(startRow).at(0).toInt() != waveLength)
        {
            qDebug()  << "Starting row was calculated incorrectly" << endl;
            exit(-2);
        }


        //Initialize 2d array
        GridDbl dataBlock;
         dataBlock.resize(dataHeight);
         for(int i = 0; i < dataHeight; ++i)
         {
             dataBlock[i].resize(dataWidth);
         }

         for(int i = 0; i < dataHeight; ++i)
         {
             for(int j = 0; j < dataWidth; ++j)
             {
                  dataBlock[i][j]= data.at(startRow+i).at(j+startCol).toDouble();
             }

         }
        return dataBlock;
}

// performs matrix subtraction to remove blank fluorescence
GridDbl subtractBlanks(const GridDbl &data, const GridDbl &blank)
{

    if(data.size() != blank.size() || data[0].size() != blank[0].size())
    {
        qDebug() << "data and blank must be the same size";
        exit(-2);
    }
    GridDbl result;
    result.resize(data.size());

    for(int i = 0; i < data.size(); ++i)
    {
        result[i].resize(data[0].size());
        for(int j = 0; j < data[0].size(); ++j)
        {
            if(data[i][j])
            {
                result[i][j] = data[i][j] - blank[i][j];
            }
            else
            {
                result[i][j] = 0;
            }
        }
    }
    return result;
}

// calculates emFret. Assumes that parameters are all same size in rows & cols
// uses two hardcoded values given in one of  the papers.
GridDbl calculate_emFret(const GridDbl &fldd, const GridDbl &flaa, const GridDbl &emTotal)
{
    //this x and y value are hardcoded based on values given in "Development of FRET Assay into ..."
    const double x = 0.378;
    const double y = 0.026;

    if(fldd.size() != flaa.size() || fldd[0].size() != flaa[0].size()
    || fldd.size() != emTotal.size() || fldd[0].size() != emTotal[0].size())
    {
        qDebug() << "flaa, fldd, and emTotal must be the same size";
        exit(-2);
    }

    emFret.resize(emTotal.size());

    //the equation to use these in is EmFret = EmTotal - x*Fldd - y*Flaa
    //use matrix subtraction (do it for each element)
    for(int i = 0; i < emTotal.size(); ++i)
    {
        emFret[i].resize(emTotal[0].size());
        for(int j = 0; j < emTotal[i].size(); ++j)
        {
            emFret[i][j] = emTotal[i][j] - (x*fldd[i][j]) - (y*flaa[i][j]);
        }
    }

    return emFret;
}
void  writeFile(const QString & file){

        QFile fileout(file);    // below is testing
        if (!fileout.open(QFile::WriteOnly ))
        {
            qDebug()<< " can not write";
            exit(-1);
        }

         // double *x = new double[emfret.size()];
         double x[12] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0.5, 0};

         QTextStream out(&fileout);
         out.setRealNumberNotation(QTextStream::FixedNotation);
         int final = emFret.at(0).size()-1;

         for(int j= 0; j <emFret.size(); ++j)
         {
                  out<<  emFret.at(j).at(final) << " ";
         }
         out << endl;
         for(int j = 0; j < emFret.size(); ++j)
         {
                  out << x[j] << " ";
         }
         out << endl;
         out << final +1;     // represents the a value
         out << endl;

          fileout.close();
}

#endif // PARSER_H

