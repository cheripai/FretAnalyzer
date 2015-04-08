#ifndef PARSER_H
#define PARSER_H

#include <QFile>
#include <QDebug>
#include <QTextStream>

typedef QVector< QVector<QString> > GridStr;
typedef QVector< QVector<double> > GridDbl;

GridDbl dataBlock;
// returns a 2d vector of strings from raw text file output from machine
// the file has a fixed format, so we can parse it accordingly.

GridStr readFile(const QString & file){   
     qDebug() << "readFile was called" << endl;
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
          line = line.simplified(); // discard \t \n
          QVector<QString> tempStr;
          QStringList list1 = line.split(" "); // split by whitespace

          for (int i = 0; i < list1.size(); ++i)
          {
              tempStr.push_back( list1.at(i));
          }
          int cols = list1.size();
          while(cols <32){
              tempStr.push_back(" ");
              cols++;
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
        int increment = data.at(1).at(12).toInt();
        int maxWaveLength = data.at(1).at(11).toInt();
        int minWaveLength = data.at(1).at(10).toInt();

        qDebug() << " increment: " << increment <<", maxWaveLength: " << maxWaveLength
                 << ", minWaveLength: " << minWaveLength <<endl;

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
        qDebug() <<"height: "<< data.size() << ", width: " << data.at(0).size() <<endl;

        //Initialize 2d array

         dataBlock.resize(dataHeight);
         for(int i = 0; i < dataHeight; ++i)
         {
             dataBlock[i].resize(dataWidth);
         }

           qDebug() << "************start********" << endl;
          for(int j = 0; j < dataWidth; ++j) // first row, discard the first two cols
          {
            dataBlock[0][j]= data.at(startRow).at(startCol + j).toDouble();
          }

         for(int i = 1; i < dataHeight; ++i)
         {
             for(int j = 0; j < dataWidth; ++j)
             {
                  dataBlock[i][j]= data.at(startRow+i).at(j).toDouble();
                 //qDebug() << data.at(startRow+i).at(j).toDouble();
             }
             //qDebug() << endl;
         }
            qDebug() << "************finish********" << endl;
        return dataBlock;
}

// performs matrix subtraction to remove blank fluorescence
GridDbl subtractBlanks(const GridDbl &data, const GridDbl &blank)
{

    qDebug() << "subtractBlanks was called" << endl;
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

void  writeFile(const QString & file){

        QFile fileout(file);    // below is testing
        if (!fileout.open(QFile::WriteOnly ))
        {
            qDebug()<< " can not write";
            exit(-1);
        }

         QTextStream out(&fileout);
         out.setRealNumberNotation(QTextStream::FixedNotation);
         for(int i= 0; i < dataBlock.size(); ++i)
         {
             for(int j= 0; j <dataBlock.at(i).size(); ++j)
             {
                  out<<  dataBlock.at(i).at(j) << " ";
             }
             out << endl;
         }
         fileout.close();
}

#endif // PARSER_H

