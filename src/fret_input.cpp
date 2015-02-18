#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <cstring>

using namespace std;

int main()
{
	fstream f;
	cerr << "opeing file" << endl;
	string file = "ex475 Rangep 1-5 uM UBC9 0-10uM.txt";
	f.open(file);
	cerr << "file opened" << endl;
	string input;
	

	//eat the first two lines, which are stuff we don't care about
	getline(f, input);
	getline(f, input);
	
	char* token;
	char* input_cstr = const_cast<char*>(input.c_str() );
	string command = "python /FretAnalyzer/src/fret.py";
	vector<vector<double> > fldd = 0.0;
	vector<vector<double> > flaa = 0.0;
	vector<vector<double> > emtotal=0.0;
	vector<double> d;
	fldd.push_back( d); //if i understand his correctly, we'll have 3 copies of the same empty vector
	flaa.push_back( d ); //but once we start pushing stuff to them, they won't be copies anymore.
	emtotal.push_back(d); //not pointers, so pushing to one won't affect the other.
	
	int col = 0, row = 0; //note, these are placeholders for the array to store the data in
	while ( !f.eof() )
	{
		getline(f, input);
		token = strtok(NULL, "\t");
		//file ex414 has EM475, which is FLdd. file ex414 also has Em530, which is Emtotal
		//file ex475 has EM530, whic his FLaa
		if ( token == 'ex414')
		{
			token = strtok(NULL, "\t");
			if (token == '475')
			{
				token = strtok(input_cstr, "\t"); //eats the temperature number
				token = strtok(input_cstr, "\t"); //advances to the next one after the temperature number
				while ( token != NULL)
				{
					token = strtok(input_cstr, "\t");
					
				}
				if ( col != 0) //if other numbers were found after temperature, then we need to advance to the next line
					row++;
				col = 0;
		//call python script using system, after appending on our information
		
	}
	
	return 0;
}

		
	
	
