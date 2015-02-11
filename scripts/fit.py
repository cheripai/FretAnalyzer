from lmfit import minimize, Parameters, Parameter, report_fit
from matplotlib.pyplot import plot, show
from numpy import array, sqrt, linspace


def emfret(params, x, a):
    kd = params['kd'].value
    emfretmax = params['emfretmax'].value
    return emfretmax * (1 - (2 * kd) / (x - a + kd + sqrt((x - a - kd) ** 2 + 4 * kd * x)))


def residuals(params, x, y, a):
    return y - emfret(params, x, a)


def main():

    # provided data
    x = array([ 10.0, 9.0, 8.0, 7.0,
                 6.0, 5.0, 4.0, 3.0, 
                 2.0, 1.0, 0.5, 0.0 ])

    # EmFret values
    y = array([ 1506945.0, 1539493.0, 1559078.0, 
                1570980.0, 1463291.0, 1341564.0, 
                1266176.0, 1204986.0,  800148.8,
                 539888.1,  201413.1, -73367.02 ])    

    a = 3


    # adding parameters and initial guesses
    params = Parameters()
    params.add('kd', value=1, min=0)
    params.add('emfretmax', value=1, min=0)


    # run fitting procedure and display results
    result = minimize(residuals, params, args=(x, y, a))
    report_fit(params)


    # plots data and curve on graph and displays
    xx1 = linspace(x.min(),x.max(),50)
    yy1 = emfret(result.params, xx1, a)
    plot(x,y,'bo',xx1,yy1,'g-')
    show()


if __name__ == "__main__":
    main()
