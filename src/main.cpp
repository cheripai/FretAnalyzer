#include <iostream>
#include <vector>
#include "../include/parser.h"
using namespace std;


int main()
{
    string file414 = "FretData/ex414 blank.txt";
    string file475 = "FretData/ex475 blank.txt";
    GridDbl flaa = getDataBlock(file475, 530);
    GridDbl fldd = getDataBlock(file414, 475);
    GridDbl emTotal = getDataBlock(file414, 530);
    //cout.precision(12);

    GridDbl emfret = calculate_emFret(fldd, flaa, emTotal);

    for(unsigned i = 0; i < emfret.size(); ++i)
    {
        for(unsigned j = 0; j < emfret.at(0).size(); ++j)
            cout << emfret.at(i).at(j) << " ";
        cout << endl;
    }
}
