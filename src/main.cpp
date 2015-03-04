#include <iostream>
#include <vector>
#include "../include/parser.h"
using namespace std;


int main()
{
    string file = "FretData/ex414 blank.txt";
    GridDbl dataBlock = getDataBlock(file, 475);
    cout.precision(14);
    for(unsigned i = 0; i < dataBlock.size(); ++i)
    {
        for(unsigned j = 0; j < dataBlock.at(0).size(); ++j)
            cout << dataBlock.at(i).at(j) << " ";
        cout << endl;
    }
}
