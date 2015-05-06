#!/usr/bin/python
from argparse import ArgumentParser
from decimal import *
import json
from lmfit import conf_interval, report_ci, fit_report, minimize, Parameters, Parameter
from matplotlib.pyplot import plot, savefig
from numpy import array, linspace, sqrt, zeros, reshape
import sys

def emfret(params, x, a):
    kd = params['kd'].value
    emfretmax = params['emfretmax'].value
    return emfretmax * (1 - (2 * kd) / (x - a + kd + sqrt((x - a - kd) ** 2 + 4 * kd * x)))
    


def residuals(params, x, y, a):
    return y - emfret(params, x, a)


def create_plot(filename, result, x, y, a):
    xx = linspace(x.min(), x.max(), 50)         
    yy = emfret(result.params, xx, a)
    z = linspace(x.min(), x.max(), 50);
    #this loop plots each set of points held in y
    for i in y: 
        plot(x, i, 'bo')

    #this plots the emfret results (xx and yy), and plots a horizontal black line ( xx, z)
    #the horizontal black line is strictly for orienting where the origin is.
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
        x = array([float(val) for val in f.readline().split()])

        # IMPORTANT: I've altered the structure of the format for reading from files.
        #            It is now:
        #               X values
        #               number of repeated measurements
        #               A values
        #               Y values (continuing to end of file)
        #
        #            This format prevents over-consuming (possibly getting the A values as Y)            
        
        num_rep = int( f.readline() )        
        a = array([float(val) for val in f.readline().split()])

        #the remaining lines are all going to be the values for y
        y = zeros( (len(a), len(x)) ) #init y to be 2D array of zeros,
                                      #num rows is equal to number of A values, num cols equal to number of X values
        count = 0
        for line in f: #eats the remaining lines of the file
            temp = ( array([float(val) for val in line.split()]) )
            if ( num_rep == 1): #this condition is specifically for the case where there is only one measurement
                num_rep = len(temp) #instead of creating large if/else blocks, it allows reuse of code below
            
            if ( len(temp) == 0): #this condition specifically checks if we've read an empty line.
                break; #if so, stop everything and move on.



            #now at this part, average the values of y, this part will be skipped by the len(temp) == 0 above
            #num_rep tells us how many values of y to average at one time, hence the increment by num_rep
            #averaged temp is an array holding the averaged values. Initialized to an array of 0s
            averaged_temp = zeros( (len(temp) / num_rep) )
            for i in range( 0, len(temp) - num_rep, num_rep ): #I is the starting position of values to be averaged
                avg = 0
                
                for j in range( 0, num_rep): #j is the offset from that starting position
                    avg += temp[i+j]
                    
                avg = avg / num_rep
                averaged_temp[ i / num_rep] = avg #remember, I jumps in increments of num_rep, and so dividing by it will yield 0,1,2,3....
            #end for

            y[count] = averaged_temp
            count += 1
        
        f.close()
    else: #reads from terminal
        for param_name in params: #prints to stdout. Needs to be modified
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
