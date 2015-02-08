import numpy as np
import scipy.optimize as optimize


def func(params, xdata, ydata):
    a = 3
    return (ydata - (params[1] * (1 - (2 * params[0]) / (xdata - a + params[0] +  np.sqrt((xdata - a - params[0]) ** 2)) + 4 * params[0] * xdata)));


def main():
    xdata = np.array([10.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.5, 0.0])
    ydata = np.array([1506945.0, 1539493.0, 1559078.0, 1570980.0, 1463291.0, 1341564.0, 1266176.0, 1204986.0, 800148.8, 539888.1, 201413.1, -73367.02])    # EmFret values
    params = np.array([1.0, 1506945.0])    # [Kd, EmFretMax]  

    print(optimize.leastsq(func, params, args=(xdata, ydata)))


main()
