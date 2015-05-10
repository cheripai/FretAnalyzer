#!/usr/bin/python
from argparse import ArgumentParser
from decimal import *
import json
from lmfit import conf_interval, report_ci, fit_report, minimize, Parameters, Parameter
from matplotlib.pyplot import plot, savefig, text
from numpy import array, linspace, sqrt, zeros, reshape, average


def emfret(params, x, a):
    kd = params['kd'].value
    emfretmax = params['emfretmax'].value
    return emfretmax * (1 - (2 * kd) / (x - a + kd + sqrt((x - a - kd) ** 2 + 4 * kd * x)))


def residuals(params, x, y, a):
    return y - emfret(params, x, a)


def create_plot(filename, result, x, y, a, i):
    xx = linspace(x.min(), x.max(), 50)         #so this returns 50 evenly spaced values between the start and stop points
    yy = emfret(result.params, xx, a)           #it uses each of these to calculate emfretmax (one per xx value)
    z = linspace(x.min(), x.max(), 50);
    #this plots all the values contained in Y
    plot(x, y, 'bo')
    #this plots the calcualted EMfret (xx,yy), and a black horizontal line through the origin (xx, z) for orientation of the axis
    plot(xx, yy, 'g-')
    plot(xx, z, 'k-')
    text(xx[-1], yy[-1], '{}'.format(i+1), ha='left', position=(xx[-1]+0.05, yy[-1]))
    savefig(filename, bbox_inches='tight', dpi=400)


def init_argparse():
    parser = ArgumentParser(description='Runs script on commandline arguments')
    parser.add_argument('-i', '--input', help='Input file', required=False)
    parser.add_argument('-o', '--output', help='Output results to file', required=False)
    parser.add_argument('-p', '--plot', help='Output file for plot', required=False)
    return parser.parse_args()
    

def main():

    # sets up command line argument parser
    args = init_argparse()


    # IMPORTANT: The format of the file has been changed. The new
    #       format is:
    #           X values of size n
    #           Number of replicates
    #           A values
    #           Y values with n values on each line
    if(args.input): #has i flag, read from specified file
        f = open(args.input, 'r')
        x = array([float(val) for val in f.readline().split()])
        num_rep = int(f.readline())
        a = array([float(val) for val in f.readline().split()])
        y = zeros((len(a), len(x))) 
        for i in range(len(a)):
            all_y = zeros((num_rep, len(x)))
            for j in range(num_rep):
                all_y[j] = array([float(val) for val in f.readline().split()])
            y[i] = average(all_y, axis=0)
        f.close()

    else: #read from cmdline
        x = array([float(val) for val in input().split()])  
        num_rep = int(input())
        a = array([float(val) for val in input().split()])
        y = zeros((len(a), len(x)))
        for i in range(len(a)):
            all_y = zeros((num_rep, len(x)))
            for j in range(num_rep):
                all_y[j] = array([float(val) for val in input().split()])
            y[i] = average(all_y, axis=0)


    #adding parameters, initial guesses, and constraints
    params = Parameters()
    params.add('kd', value=1, min=0)
    params.add('emfretmax', value=1, min=0)

    #run fitting procedure and display results
    #this needs to be repeated for each A value. Note that A[i] corresponds to Y[i]
    #X is assumed to hold constant across all of these, so it remains unchanged across iterations
    for i in range(len(a)):
        result = minimize(residuals, params, args=(x, y[i], a[i] ))
        ci = conf_interval(result, maxiter=1000)

        # print results to file or terminal (depends on -o flag)
        if(args.output):
            f = open(args.output, 'w')
            f.write(json.dumps({param_name: params[param_name].value for param_name in params}) + '\n')
            f.write(json.dumps(ci) + '\n')
            f.close()
        else:
            print('Y{}:\n'.format(i+1))
            for param_name in params: 
                print('{}:\n{}\n'.format(param_name, round(params[param_name].value, 4)))
                p_width = max(len(str(p)) for p in ci[param_name][0])
                getcontext().prec = 4
                for _ in ci[param_name]:
                    print('{}%\t{}'.format(_[0]*100, round(_[1], 4)))
                print('\n')
        
        # plots data and curve on graph and displays if output file is given
        if(args.plot):
            create_plot(args.plot, result, x, y[i], a[i], i)
    
if __name__ == "__main__":
    main()
