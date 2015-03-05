#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "../include/parser.h"

// calculates emFret. Assumes that parameters are all same size in rows & cols
//uses two hardcoded values given in one of  the papers.
GridDbl const calculate_emFret( const GridDbl& fldd, const GridDbl& flaa, const GridDbl& emTotal)
{
    //this x and y value are hardcoded based on values given in "Development of FRET Assay into ..."
    const double x = 0.378;
    const double y = 0.026;

    //this calculates the size of emTotal once, in case it's really big
    //reserve the same number in spaces in memory for emFret (output)
    
    int size = emTotal.size();
    GridDbl emFret;
    emFret.reserve( size );
    for ( unsigned i = 0; i < emTotal.size(); ++i)
        emFret[i].reserve( size );


    //the equation to use these in is EmFret = EmTotal - x*Fldd - y*Flaa
    //use matrix subtraction (do it for each element)
    for ( unsigned i = 0; i < emTotal.size(); ++i)
        for ( unsigned j = 0; j < emTotal[i].size(); ++j)
            if ( emTotal[i][j] == 0)
                //if one of the cells left empty, skip it
            else
                emFret[i][j] = emTotal[i][j] - (x*fldd[i][j]) - (y*flaa[i][j]);

    return emTotal;
}//end calculate_emFret
    
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
