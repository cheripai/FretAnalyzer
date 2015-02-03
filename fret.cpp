#include <iostream>
#include "stdafx.h"
#include <stdlib.h>
#include <math.h>
#include "interpolation.h"

using namespace alglib;


//
// Formula for calculating Kd and EmFretMax
//
void function_cx_1_func(const real_1d_array &c, const real_1d_array &x, double &func, void *ptr) 
{
    int a = 3;  // concentration level
    func = (1 - (2 * c[0])  / (x[0] - a + c[0] + sqrt(pow((x[0] - a - c[0]), 2)) + 4 * c[0] * x[0]));
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
    real_1d_array c = "[1]";                                                     // initial approximation to the solution


    //
    // Variable Initialization
    //
    double epsf = 0;                  // stopping criterion
    double epsx = 0.000001;           // 
    ae_int_t maxits = 0;              // maximum iterations (0 is unlimiteD)
    ae_int_t info;                    // completion code (fail, steps, stop condition, etc)
    lsfitstate state;                 // structure stores algorithm state
    lsfitreport rep;                  // optimization report
    double diffstep = 0.0001;         // numerical differentiation step


    //
    // Fitting procedure
    //
    lsfitcreatef(x, y, c, diffstep, state);         // initializes data to state
    lsfitsetcond(state, epsf, epsx, maxits);        // sets termination condition for state
    alglib::lsfitfit(state, function_cx_1_func);    // performs fitting on given function
    lsfitresults(state, info, c, rep);              // reads solution from state into vars


    //
    // Print results
    //
    std::cout << "Term Code: " << int(info) << std::endl;                // displays termination string
    std::cout << "Answer:    " << c.tostring(1).c_str() << std::endl;    // displays answer
    return 0;
}
