import scipy as sp
import numpy as np
#import matplotlib.pyplot as plt
from lmfit import minimize, Parameters, Parameter, report_fit


def main():
    # data provided
    x = np.array([10.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.5, 0.0])
    y = np.array([1506945.0, 1539493.0, 1559078.0, 1570980.0, 1463291.0, 1341564.0, 1266176.0, 1204986.0, 800148.8, 539888.1, 201413.1, -73367.02])    # EmFret values

    # tpl is a tuple that contains the parameters of the fit
    a = 3
    emfret = lambda params,x : params['emfretmax'] * (1 - (2 * params['kd']) / (x - a + params['kd'] + np.sqrt((x - a - params['kd']) ** 2 + 4 * params['kd'] * x)))

    # set the function to run optimize on
    func = emfret

    params = Parameters()
    params.add('kd', value = 1)
    params.add('emfretmax', value = 10000000)

    # ErrorFunc is the diference between the func and the y "experimental" data
    ErrorFunc = lambda tpl,x,y: func(tpl,x)-y

    result = minimize(ErrorFunc, params, args=(x,y))
    final = y + result.residual
    report_fit(params)


    """
    xx1 = np.linspace(x.min(),x.max(),50)
    yy1 = func(tplFinal,xx1)
    plt.plot(x,y,'bo',xx1,yy1,'g-')
    plt.show()
    """


if __name__ == "__main__":
    main()
