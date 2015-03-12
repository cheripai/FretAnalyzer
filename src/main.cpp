#include <iostream>
#include <fstream>
#include <unistd.h>
#include "../include/parser.h"
using namespace std;


int main()
{
    string blank414 = "FretData/ex414 blank.txt";
    string blank475 = "FretData/ex475 blank.txt";
    string data414 = "FretData/ex414 Rangep 1-5uM Ubc9 0-10uM.txt";
    string data475 = "FretData/ex475 Rangep 1-5uM Ubc9 0-10uM.txt";
    GridDbl flaa = subtractBlanks(getDataBlock(data475,530),getDataBlock(blank475,530));
    GridDbl fldd = subtractBlanks(getDataBlock(data414,475),getDataBlock(blank414,475));
    GridDbl emTotal = subtractBlanks(getDataBlock(data414,530),getDataBlock(blank414,530));
    GridDbl emfret = calculate_emFret(fldd, flaa, emTotal);

    // double *x = new double[emfret.size()];
    double x[12] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0.5, 0};


    ofstream f;
    f.setf(ios::fixed);
    for(unsigned i = 0; i < emfret.at(0).size(); ++i)
    {
        f.open("data.txt");
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
        f.close();
        // FIXME: Run python script here
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
