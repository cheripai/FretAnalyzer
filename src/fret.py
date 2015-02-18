from argparse import ArgumentParser
from lmfit import minimize, Parameters, Parameter, report_fit
from matplotlib.pyplot import plot, savefig
from numpy import array, sqrt, linspace


def emfret(params, x, a):
    kd = params['kd'].value
    emfretmax = params['emfretmax'].value
    return emfretmax * (1 - (2 * kd) / (x - a + kd + sqrt((x - a - kd) ** 2 + 4 * kd * x)))


def residuals(params, x, y, a):
    return y - emfret(params, x, a)


def create_plot(filename, result, x, y, a):
        xx = linspace(x.min(), x.max(), 50)
        yy = emfret(result.params, xx, a)
        plot(x, y, 'bo', xx, yy, 'g-')
        savefig(filename, bbox_inches='tight')


def main():

    # sets up command line argument parser
    parser = ArgumentParser(description='Runs script on commandline arguments')
    parser.add_argument('-i', '--input', help='Input file name', required=True)
    parser.add_argument('-o', '--output', help='Output file name for plot', required=False)
    args = parser.parse_args()


    # grab data from file to run lmfit on
    f = open(args.input, 'r')
    x = array([float(val) for val in f.readline().split()])
    y = array([float(val) for val in f.readline().split()])
    a = array([float(val) for val in f.readline().split()])
    f.close()
    assert x.size == y.size, 'number of values of x and y are mismatched'
    assert a.size == 1, 'invalid number of values for a'


    # adding parameters and initial guesses
    params = Parameters()
    params.add('kd', value=1, min=0)
    params.add('emfretmax', value=1, min=0)


    # run fitting procedure and display results
    result = minimize(residuals, params, args=(x, y, a))
    report_fit(params)


    # plots data and curve on graph and displays if output file is given
    if(args.output):
        create_plot(args.output, result, x, y, a)


if __name__ == "__main__":
    main()
