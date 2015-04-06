#!/usr/bin/python
from argparse import ArgumentParser
from decimal import *
import json
from lmfit import conf_interval, report_ci, fit_report, minimize, Parameters, Parameter
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
    savefig(filename, bbox_inches='tight', dpi = 400)


def init_argparse():
    parser = ArgumentParser(description='Runs script on commandline arguments')
    parser.add_argument('-i', '--input', help='Input file', required=False)
    parser.add_argument('-o', '--output', help='Output results to file', required=False)
    parser.add_argument('-p', '--plot', help='Output file for plot', required=False)
    return parser.parse_args()
    

def main():

    # sets up command line argument parser
    args = init_argparse()


    # grab data from file to run lmfit on
    if(args.input):
        f = open(args.input, 'r')
        x = array([float(val) for val in f.readline().split()])
        y = array([float(val) for val in f.readline().split()])
        a = array([float(val) for val in f.readline().split()])
        f.close()
    else:
        x = array([float(val) for val in input().split()])
        y = array([float(val) for val in input().split()])
        a = array([float(val) for val in input().split()])
    assert x.size == y.size, 'number of values of x and y are mismatched'
    assert a.size == 1, 'invalid number of values for a'


    # adding parameters, initial guesses, and constraints
    params = Parameters()
    params.add('kd', value=1, min=0)
    params.add('emfretmax', value=1, min=0)


    # run fitting procedure and display results
    result = minimize(residuals, params, args=(x, y, a))
    ci = conf_interval(result, maxiter=1000)

    # print results to file or terminal (depends on -o flag)
    if(args.output):
        f = open(args.output, 'w')
        f.write(json.dumps({param_name: params[param_name].value for param_name in params}) + '\n')
        f.write(json.dumps(ci) + '\n')
        f.close()
    else:
        for param_name in params:
            print('{}:\n{}\n'.format(param_name, round(params[param_name].value, 4)))
            p_width = max(len(str(p)) for p in ci[param_name][0])
            getcontext().prec = 4
            for _ in ci[param_name]:
                print('{}%\t{}'.format(_[0]*100, round(_[1], 4)))
            print('\n')

    # plots data and curve on graph and displays if output file is given
    if(args.plot):
        create_plot(args.plot, result, x, y, a)


if __name__ == "__main__":
    main()
