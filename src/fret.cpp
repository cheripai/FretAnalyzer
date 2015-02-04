#include "../lib/stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../lib/interpolation.h"

using namespace alglib;
void function_debt_func(const real_1d_array &c, const real_1d_array &x, double &func, void *ptr) 
{
    //
    // this callback calculates f(c,x)=c[0]*(1+c[1]*(pow(x[0]-1999,c[2])-1))
    //
    //func = c[0]*(1+c[1]*(pow(x[0]-1999,c[2])-1));

    int a = 3;  // concentration level
    // this callback calculates the EmFret value
    func = c[1] * (1 - (2 * c[0])  / (x[0] - a + c[0] + sqrt(pow((x[0] - a - c[0]), 2)) + 4 * c[0] * x[0]));
}

int main(int argc, char **argv)
{
    //
    // In this example we demonstrate fitting by
    //     f(x) = c[0]*(1+c[1]*((x-1999)^c[2]-1))
    // subject to bound constraints
    //     -INF  < c[0] < +INF
    //      -10 <= c[1] <= +10
    //      0.1 <= c[2] <= 2.0
    // Data we want to fit are time series of Japan national debt
    // collected from 2000 to 2008 measured in USD (dollars, not
    // millions of dollars).
    //
    // Our variables are:
    //     c[0] - debt value at initial moment (2000),
    //     c[1] - direction coefficient (growth or decrease),
    //     c[2] - curvature coefficient.
    // You may see that our variables are badly scaled - first one 
    // is order of 10^12, and next two are somewhere about 1 in 
    // magnitude. Such problem is difficult to solve without some
    // kind of scaling.
    // That is exactly where lsfitsetscale() function can be used.
    // We set scale of our variables to [1.0E12, 1, 1], which allows
    // us to easily solve this problem.
    //
    // You can try commenting out lsfitsetscale() call - and you will 
    // see that algorithm will fail to converge.

	//
	// Data initialization
	//
	real_2d_array x = "[[10],[9],[8],[7],[6],[5],[4],[3],[2],[1],[0.5],[0]]";    // points
	real_1d_array y = "[1506945, 1539493, 1559078, 1570980,\
						1463291, 1341564, 1266176, 1204986,\
						800148.8, 539888.1, 201413.1, -73367.02]";               // function values
	real_1d_array c = "[1, 1506945]";      // initial approximation to the solution

    double epsf = 0;
    double epsx = 0.000001;
    real_1d_array bndl = "[-100, -inf]";
    real_1d_array bndu = "[100, +inf]";
    real_1d_array s = "[0.1, 1.0e+6]";
    ae_int_t maxits = 0;
    ae_int_t info;
    lsfitstate state;
    lsfitreport rep;
    double diffstep = 0.000001;

    lsfitcreatef(x, y, c, diffstep, state);
    lsfitsetcond(state, epsf, epsx, maxits);
    lsfitsetbc(state, bndl, bndu);
    lsfitsetscale(state, s);
    alglib::lsfitfit(state, function_debt_func);
    lsfitresults(state, info, c, rep);
    printf("%d\n", int(info)); // EXPECTED: 2
    printf("%s\n", c.tostring(-2).c_str());
    return 0;
}
