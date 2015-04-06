#ifndef PARSER_H
#define PARSER_H

#include <QFile>
#include <QDebug>
#include <QTextStream>

typedef QVector<QVector<QString> > GridStr;
typedef QVector<QVector<double> > GridDbl;
GridStr data;

// returns a 2d vector of strings from raw text file output from machine
// the file has a fixed format, so we can parse it accordingly.

GridStr readFile(const QString & file){

    QVector<QString> tempStr;

    // open a file
    QFile filein(file);
    if (!filein.open(QFile::ReadOnly ))
     {
        qDebug()<< " can not read";
        exit(-1);
    }
    QTextStream in(&filein);

    while (!in.atEnd()) {
          QString line = in.readLine(); // read a line
          line = line.simplified(); // discard \t \n

          QStringList list1 = line.split(" "); // split by whitespace

          for (int i = 0; i < list1.size(); ++i)
          {
              tempStr.push_back( list1.at(i));
          }
          data.push_back(tempStr);   // pushback to a 2d string vector
    }

    filein.close();
    return data;
}
void  writeFile(const QString & file){
        QFile fileout(file);    // below is testing
        if (!fileout.open(QFile::WriteOnly ))
        {
            qDebug()<< " can not write";
            exit(-1);
        }

         QTextStream out(&fileout);
         for(int i= 0; i < data.size(); ++i)
         {
             for(int j= 0; j < data.at(i).size(); ++j)
             {
                  out<< data.at(i).at(j) << " ";
             }
             out<<"\n";
         }
         fileout.close();
}

#endif // PARSER_H

