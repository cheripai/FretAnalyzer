#!/usr/bin/python
from argparse import ArgumentParser
from decimal import *
import json
from lmfit import conf_interval, report_ci, fit_report, minimize, Parameters, Parameter
from matplotlib.pyplot import plot, savefig
from numpy import array, linspace, sqrt, zeros, reshape
import sys #for debugging stuffs

def emfret(params, x, a):
    kd = params['kd'].value
    emfretmax = params['emfretmax'].value
    return emfretmax * (1 - (2 * kd) / (x - a + kd + sqrt((x - a - kd) ** 2 + 4 * kd * x)))
    


def residuals(params, x, y, a):
    return y - emfret(params, x, a)


def create_plot(filename, result, x, y, a):
    xx = linspace(x.min(), x.max(), 50)         #so this returns 50 evenly spaced values between the start and stop points
    yy = emfret(result.params, xx, a)           #it uses each of these to calculate emfretmax (one per xx value)
    z = linspace(x.min(), x.max(), 50);
    #this plots all the values contained in Y
    plot(x, y, 'bo')
    #this plots the calcualted EMfret (xx,yy), and a black horizontal line through the origin (xx, z) for orientation of the axis
    plot(xx, yy, 'g-', xx, z, 'k-')
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

    if(args.input): #has i flag, read from specified file
        f = open(args.input, 'r')

        # IMPORTANT: The format of the file has been changed. The new
        #       format is:
        #           X values
        #           number of times measurements are repeated
        #           A values
        #           Y values (continuing to end of file)
        #

        x = array([float(val) for val in f.readline().split()])
        num_rep = int( f.readline() )
        a = array([float(val) for val in f.readline().split()])

        
        #the remaining lines are all going to be the values for y
        #y is initialized to an 2d array of zeros, number of rows equal to number of A values
        #number of colums is equal to the number of measurements made in a single row
        y = zeros( (len(a), len(x)) ) 
        count = 0
        for line in f: #eats the remaining lines of the file
            temp = ( array([float(val) for val in line.split()]) )
            if ( len(temp) == 0):
                break;

            #now at this part, average the values of y
            #num_rep tells us how many values of y to average at one time, hence the increment by num_rep
            #averaged temp is an array holding the averaged values. Initialized to an array of 0s
            averaged_temp = zeros( (len(temp) / num_rep) )
            for i in range( 0, len(temp) - num_rep, num_rep ): #I is the starting position of values to be averaged
                avg = 0
                
                for j in range( 0, num_rep): #j is the offset from that starting position
                    avg += temp[i+j]
                avg = avg / num_rep
                averaged_temp[ i / num_rep] = avg #remember, I jumps in increments of num_rep, and so dividing by it will yield 0,1,2,3....
            y[count] = averaged_temp
            count += 1
        
        f.close()

    else: #read from cmdline
        x = array([float(val) for val in input().split()])  
        y = array([float(val) for val in input().split()])  
        a = array([float(val) for val in input().split()])



    #adding parameters, initial guesses, and constraints
    #from lmfit library
    #don't touch these for now
    params = Parameters()
    params.add('kd', value=1, min=0)
    params.add('emfretmax', value=1, min=0)

    #run fitting procedure and display results
    #this needs to be repeated for each A value. Note that A[i] corresponds to Y[i]
    #X is assumed to hold constant across all of these, so it remains unchanged across iterations
    for i in range (len(a)):
        result = minimize(residuals, params, args=(x, y[i], a[i] ))
        ci = conf_interval(result, maxiter=1000)

        # print results to file or terminal (depends on -o flag)
        if(args.output):
            f = open(args.output, 'w')
            f.write(json.dumps({param_name: params[param_name].value for param_name in params}) + '\n')
            f.write(json.dumps(ci) + '\n')
            f.close()
        else:
            for param_name in params: #prints to stdout. Needs to be modified
                print('{}:\n{}\n'.format(param_name, round(params[param_name].value, 4)))
                p_width = max(len(str(p)) for p in ci[param_name][0])
                getcontext().prec = 4
                for _ in ci[param_name]:
                    print('{}%\t{}'.format(_[0]*100, round(_[1], 4)))
                print('\n')
        #end else
        
        # plots data and curve on graph and displays if output file is given
        if(args.plot):
            create_plot(args.plot, result, x, y[i], a[i])
    #end for
    
if __name__ == "__main__":
    main()
