#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <vector>

using namespace std;
typedef vector<vector<string> > GridStr;
typedef vector<vector<double> > GridDbl;


// Reads text file into 2d vector
GridStr readFile(string file)
{
    GridStr data;
    ifstream input(file.c_str());
    const char row_delim = '\n';
    const char field_delim = '\t';

    for(string row; getline(input, row, row_delim); ) 
    {
        data.push_back(GridStr::value_type());
        istringstream ss(row);
        for(string field; getline(ss, field, field_delim); ) 
        {
            data.back().push_back(field);
        }
    }
    return data;
}


// Returns desired numerical data from text file
GridDbl getDataBlock(string file, int waveLength)
{
    GridStr data = readFile(file);
    const int dataHeight = 16;
    const int dataWidth = 24;

    // Gets following data from corresponding cell in data array
    int increment = atoi(data.at(1).at(14).c_str());
    int maxWaveLength = atoi(data.at(1).at(13).c_str());
    int minWaveLength = atoi(data.at(1).at(12).c_str());

    if(waveLength > maxWaveLength 
    || waveLength < minWaveLength
    || waveLength % increment != 0)
    {
        cerr << "ERROR: given wavelength is out of bounds" << endl;
        exit(1);
    }

    // Calculates desired row of wave length based on format of input file
    int startRow = 3 + (dataHeight + 1) * (waveLength - minWaveLength) / increment;
    // First two columns of input file are irrelevant to our desired data
    int startCol = 2;

    if(atoi(data.at(startRow).at(0).c_str()) != waveLength)
    {
        cerr << "Starting row was calculated incorrectly" << endl;
        exit(1);
    }

    GridDbl dataBlock;   
    dataBlock.resize(dataHeight);
    for(int i = 0; i < dataHeight; ++i)
    {
        dataBlock.at(i).resize(dataWidth);
        for(int j = 0; j < dataWidth; ++j)
            dataBlock.at(i).at(j) = atof(data.at(startRow+i).at(startCol+j).c_str());
    }   

    return dataBlock;
}


int main()
{
    string file = "../FretData/ex414 blank.txt";
    GridDbl dataBlock = getDataBlock(file, 475);
    cout.precision(20);
    for(int i = 0; i < dataBlock.size(); ++i)
    {
        for(int j = 0; j < dataBlock.at(0).size(); ++j)
            cout << dataBlock.at(i).at(j) << " ";
        cout << endl;
    }
}
