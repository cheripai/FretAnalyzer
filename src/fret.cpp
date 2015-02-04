#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "../lib/interpolation.h"
#include "../lib/stdafx.h"

using namespace alglib;
void function_debt_func(const real_1d_array &c, const real_1d_array &x, double &func, void *ptr) 
{   
    int a = 3;  
    // this callback calculates the EmFret and Kd value
    func = c[1] * (1 - (2 * c[0])  / (x[0] - a + c[0] + sqrt(pow((x[0] - a - c[0]), 2)) + 4 * c[0] * x[0]));
}

int main(int argc, char **argv)
{
	//
	// Data initialization
	//
	real_2d_array x = "[[10],[9],[8],[7],[6],[5],[4],[3],[2],[1],[0.5],[0]]";    // x values
	real_1d_array y = "[1506945, 1539493, 1559078, 1570980, \
						1463291, 1341564, 1266176, 1204986, \
						800148.8, 539888.1, 201413.1, -73367.02]";               // EmFret values
	real_1d_array c = "[1, 1506945]";    // [Kd, EmFretMax]  


    double epsf = 0;
	double epsx = 1.0e-5;
    real_1d_array bndl = "[-100, -inf]";    // lower bounds for [Kd, EmFretMax]
    real_1d_array bndu = "[100, +inf]";     // upper bounds for [Kd, EmFretMax]
    real_1d_array s = "[1, 1.0e+6]";      // step scale for [Kd, EmFretMax]
    ae_int_t maxits = 0;				    // maximum iterations (0 is unlimited)
    ae_int_t info;
    lsfitstate state;
    lsfitreport rep;
    double diffstep = 1.0e-5;


    lsfitcreatef(x, y, c, diffstep, state);             // puts data into state
    lsfitsetcond(state, epsf, epsx, maxits);            // sets termination conditoin
    lsfitsetbc(state, bndl, bndu);                      // sets upper and lower bounds
    lsfitsetscale(state, s);                            // sets step scale
    alglib::lsfitfit(state, function_debt_func);        // starts fitting procedure
    lsfitresults(state, info, c, rep);                  // grabs results


	std::cout << int(info) << std::endl;
	std::cout << c.tostring(-2).c_str() << std::endl;
    return 0;
}
