#ifndef _PARSER
#define _PARSER

#include <iostream>
#include <vector>
using namespace std;


typedef vector<vector<string> > GridStr;
typedef vector<vector<double> > GridDbl;


GridDbl calculate_emFret(const GridDbl& fldd, const GridDbl& flaa, const GridDbl& emTotal);

//
// returns a 2d vector of strings from raw text file output from machine
// the file has a fixed format, so we can parse it accordingly.
//
GridStr readFile(string file);

// 
// returns specified 2d vector of doubles of size 16x24 
// the data selected is based on the given wavelength
//
GridDbl getDataBlock(string file, int waveLength);

#endif
