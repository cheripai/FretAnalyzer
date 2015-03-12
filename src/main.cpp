#include <iostream>
#include <fstream>
#include <unistd.h>
#include "../include/parser.h"
using namespace std;


int main()
{
    string blank414 = "FretData/ex414 blank.txt";
    string blank475 = "FretData/ex475 blank.txt";
    GridDbl flaa = getDataBlock(blank475, 530);
    GridDbl fldd = getDataBlock(blank414, 475);
    GridDbl emTotal = getDataBlock(blank414, 530);
    GridDbl emfret = calculate_emFret(fldd, flaa, emTotal);

    // double *x = new double[emfret.size()];
    double x[12] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0.5, 0};


    ofstream f;
    f.open("data.txt");
    for(unsigned i = 0; i < emfret.at(0).size(); ++i)
    {
        for(unsigned j = 0; j < emfret.size(); ++j)
        {
            f << emfret.at(j).at(i) << " ";
        }
        f << endl;
        for(unsigned j = 0; j < emfret.size(); ++j)
        {
            f << x[j] << " ";
        }
        f << endl;
        f << i + 1;     // represents the a value
        f << endl;
    }

    // use QProcess
    /*
    QObject *parent;
    QString program = "python";
    QStringList arguments;
    arguments << "src/fret.py" << "-i" << "test/data1.txt";
    QProcess *myProcess = new QProcess(parent);
    myProcess->start(program, arguments)
    */
}
