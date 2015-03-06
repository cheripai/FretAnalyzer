from argparse import ArgumentParser
from lmfit import conf_interval, report_ci, fit_report, minimize, Parameters, Parameter
import lmfit
from matplotlib.pyplot import plot, savefig
from numpy import array, linspace, sqrt


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


def init_argparse():
    parser = ArgumentParser(description='Runs script on commandline arguments')
    parser.add_argument('-i', '--input', help='Input file', required=True)
    parser.add_argument('-o', '--output', help='Output results to file', required=False)
    parser.add_argument('-p', '--plot', help='Output file for plot', required=False)
    return parser.parse_args()
    

def main():

    # sets up command line argument parser
    args = init_argparse()


    # grab data from file to run lmfit on
    f = open(args.input, 'r')
    x = array([float(val) for val in f.readline().split()])
    y = array([float(val) for val in f.readline().split()])
    a = array([float(val) for val in f.readline().split()])
    f.close()
    assert x.size == y.size, 'number of values of x and y are mismatched'
    assert a.size == 1, 'invalid number of values for a'


    # adding parameters, initial guesses, and constraints
    params = Parameters()
    params.add('kd', value=1, min=0)
    params.add('emfretmax', value=1, min=0)


    # run fitting procedure and display results
    result = minimize(residuals, params, args=(x, y, a))
    ci = conf_interval(result)

    # print results to file or terminal (depends on -o flag)
    if(args.output):
        f = open(args.output, 'w')
        for param_name in params:
            f.write('%s\n' % repr(params[param_name].value))
        f.write('kd')
        for _ in ci['kd']:
            f.write('\n'+repr(_))
        f.write('\nemfretmax')
        for _ in ci['emfretmax']:
            f.write('\n'+repr(_))
        f.close()
    else:
        print(fit_report(params, show_correl=False)+'\n')
        print('kd')
        for _ in ci['kd']:
            print(repr(_))
        print('emfretmax')
        for _ in ci['emfretmax']:
            print(repr(_))


    # plots data and curve on graph and displays if output file is given
    if(args.plot):
        create_plot(args.plot, result, x, y, a)


if __name__ == "__main__":
    main()
