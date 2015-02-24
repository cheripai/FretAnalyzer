#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>

using namespace std;

void print_vect(vector<vector<double> > v)
{
    for ( unsigned x = 0; x < v.size(); ++x)
    {
        for (unsigned y = 0; y < v[x].size(); ++y)
            cout << v[x][y] << "\t";
        cout << endl;
    }
}

int main()
{

    fstream f;
	cerr << "opeing file" << endl;
	string file = "ex414 blank.txt";
	f.open(file.c_str() );
	cerr << "file opened" << endl;

    string input;
    vector< vector<double> > fldd, flaa, emTotal;
    vector< double > d;

    bool flag = false;
    int null_count = 0;
    char* token;
    bool outer = true, inner = true, mid = true;
    const char* tab = "\t";
    char* cstr;
    while ( outer )
    {
        getline(f, input);
        cerr << "outerloop curr line : " << input << endl;
        mid = true;
        if ( file.find("ex414") == 0 ) //in the ex475 file, containing FLdd and EmTotal
        {
            getline(f, input);
            cerr << "file find() curr line : " << input << endl;
            if ( input.find("475") == 0 )
            {
                while ( mid ) //mid loop keeps parsing lines
                {
                    getline(f, input);
                    cerr << "curr line : " << input << endl;
                    cstr = const_cast<char*>(input.c_str() );
                    while ( inner ) //inner loop keeps parsing tokens
                    {
                        token = strtok( NULL,  "\t");
                        cerr << "current token: " << token << endl;
                        if ( !token ) //not empty, not null, so filled with something
                        {
                            null_count++;
                            if ( null_count >= 9) //if we've already eaten actaul numbers, but keep encountering empty stuff
                            {   //then move on to finding emTotal in ex475
                                inner = false;
                            }
                            if ( null_count >= 10)
                                mid = false;
                                
                        }
                        else    //add it to the array
                        {
                            d.push_back( strtod( const_cast<const char*>(token), NULL ) );
                            flag = true;
                            null_count = 0;
                        }

                            
                    }//end while inner
                    null_count = 0; //reset null_count for next line
                    cerr << "pushing back" << endl;
                    fldd.push_back(d); //add current line to fldd
                    d.clear(); //erase everything in d so it can be reused
                    inner = true;
                    
                }//end while mid
            }//end if find(475)
            else if ( input.find("530") == 0 )
            {
                while ( mid )
                {
                    getline(f, input);
                    cstr = const_cast<char*>(input.c_str() );
                    while ( inner )
                    {
                        token = strtok(cstr, tab);
                        if ( !token)
                        {
                            null_count++;
                            if ( null_count >= 9)
                                inner = false;
                            if ( null_count >= 10)
                            {
                                mid = false;
                                outer = false;
                            }
                        }
                        else
                        {
                            d.push_back( strtod( const_cast<const char*>(token), NULL ) );
                            flag = true;
                            null_count = 0;
                        }
                    }//end while inner

                    null_count = 0;
                    emTotal.push_back(d);
                    d.clear();
                    inner = true;
                }//end while mid
            }//end if find(530)

        }//end if ex475

        else if ( file.find("ex475") == 0 ) //ex475 has em530, which is flaa
        {
            if ( input.find("530") == 0 )
            {
                while ( mid ) //mid loop keeps parsing lines
                {
                    getline(f, input);
                    cstr = const_cast<char*>(input.c_str() );
                    while ( inner ) //inner loop keeps parsing tokens
                    {
                        token = strtok( NULL,  tab);
                        if ( !token ) //not empty, not null, so filled with something
                        {
                            null_count++;
                            if ( null_count >= 9) //if we've already eaten actaul numbers, but keep encountering empty stuff
                            {   //then move on to finding emTotal in ex475
                                inner = false;
                            }
                            if ( null_count >= 10)
                            {
                                mid = false;
                                outer = false;
                            }
                        }
                        else    //add it to the array
                        {
                            d.push_back( strtod( const_cast<const char*>(token), NULL ) );
                            flag = true;
                            null_count = 0;
                        }

                            
                    }//end while inner
                    null_count = 0; //reset null_count for next line
                    flaa.push_back(d); //add current line to fldd
                    d.clear(); //erase everything in d so it can be reused
                    inner = true;
                    
                }//end while mid
            }//end if find(530)
        }//end else if
    }//end while outer
                    
                            
            
    print_vect(fldd);

    return 0;
}
