#!/usr/bin/python2
from argparse import ArgumentParser
from decimal import *
import json
from lmfit import conf_interval, report_ci, fit_report, minimize, Parameters, Parameter
from matplotlib.pyplot import plot, savefig, text, ticklabel_format, xlabel, ylabel
from numpy import array, linspace, sqrt, zeros, reshape, average, std, var


def emfret(params, x, a):
    kd = params['Kd'].value
    emfretmax = params['EmFRETMAX'].value
    return emfretmax * (1 - (2 * kd) / (x - a + kd + sqrt((x - a - kd) ** 2 + 4 * kd * x)))


def residuals(params, x, y, a):
    return y - emfret(params, x, a)


def create_plot(filename, result, x, y, a, stddev, i):
    xx = linspace(x.min(), x.max(), 50)         #so this returns 50 evenly spaced values between the start and stop points
    yy = emfret(result.params, xx, a)           #it uses each of these to calculate emfretmax (one per xx value)
    z = linspace(x.min(), x.max(), 50);
    
    #this plots all the values contained in Y 
    points = [ 'b.', 'r.', 'g.', 'c.', 'm.' ] #colors are: Blue, Red, Green, Cyan, Magenta.
    #Do not use yellow, too hard to see against white background
    stddev_points = ['b_', 'r_', 'g_', 'c_', 'm_'] #will plot colored horizontal lines for std dev
    line_style = ['b-', 'r-', 'g-', 'c-', 'm-'] #uses dashes, but generally connects into a straight line

    #tracks the number of colors used in the lists above. Rotate through this list as necessary
    num_colors = len(points)

    #note that markersize is completely arbitrary
    plot(x, y, points[i % num_colors], markersize=15)

    #plot the std dev ( point +/- std dev)
    #again, markersize/width are arbitrary. Markeredgewith increases the size of the dashes used by expanding the border.
    #the border is the same color as the interior of the marker, so it appears only the dashes got thicker
    plot(x, y-stddev, stddev_points[i % num_colors], markersize=7, markeredgewidth=3)
    plot(x, y+stddev, stddev_points[i % num_colors], markersize=7, markeredgewidth=3)
    plot([x,x], [y-stddev, y+stddev], line_style[i%num_colors], linewidth=1) #this plots a vertical line between stddev values
    
    #this plots the calcualted EMfret (xx,yy), and a black horizontal line through the origin (xx, z) for orientation of the axis
    plot(xx, yy, line_style[i % num_colors], linewidth=3)
    plot(xx, z, 'k-')

    ticklabel_format(axis='y', style='sci', scilimits=(0,0) ) #this forces labels on the y axis into scientific format

    #appropriately labels the x and y axis, fontsize is arbitary
    ylabel("Em$_{FRET}$ (RFU)", fontsize=14) #removed fontweight='bold'
    xlabel('Concentration of Acceptor') #pull label from users's input (ie uMoles, mMoles, picoMoles)
    
    
    text(xx[-1], yy[-1], '{}'.format(a), ha='left', position=(xx[-1]+0.05, yy[-1])) #this numbers the lines
    savefig(filename, bbox_inches='tight', dpi=200)


def init_argparse():
    parser = ArgumentParser(description='Runs script on commandline arguments')
    parser.add_argument('-i', '--input', help='Input file', required=False)
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
        stddev = zeros( (len(a), len(x)) )
        for i in range(len(a)):
            all_y = zeros((num_rep, len(x)))
            for j in range(num_rep):
                all_y[j] = array([float(val) for val in f.readline().split()])
            y[i] = average(all_y, axis=0)
            stddev[i] = std(all_y, axis=0)
        f.close()

    else: #read from cmdline
        x = array([float(val) for val in raw_input.split()])
        num_rep = int(raw_input)
        a = array([float(val) for val in raw_input.split()])
        y = zeros((len(a), len(x)))
        stddev = zeros((len(a), len(x)))
        for i in range(len(a)):
            all_y = zeros((num_rep, len(x)))
            for j in range(num_rep):
                all_y[j] = array([float(val) for val in raw_input.split()])
            y[i] = average(all_y, axis=0)
            stddev[i] = std(all_y, axis=0)


    #adding parameters, initial guesses, and constraints
    params = Parameters()
    params.add('Kd', value=1, min=0)
    params.add('EmFRETMAX', value=1, min=0)
    return_data = {}
    ci = []
    
    #run fitting procedure and display results
    #this needs to be repeated for each A value. Note that A[i] corresponds to Y[i]
    #X is assumed to hold constant across all of these, so it remains unchanged across iterations
    for i in range(len(a)):
        result = minimize(residuals, params, args=(x, y[i], a[i] ))
        ci.append(conf_interval(result, maxiter=1000))

        # generate table of results with tabulate
        return_data[a[i]] = [a[i],
                             round(params['Kd'].value, 4), 
                             round(params['Kd'].stderr, 4), 
                             round(params['EmFRETMAX'].value, 4),
                             round(params['EmFRETMAX'].stderr, 4),
                             round(1-result.residual.var() / var(y), 4)]
        
        # plots data and curve on graph and displays if output file is given
        if(args.plot):
            create_plot(args.plot, result, x, y[i], a[i], stddev[i], i)

    print(json.dumps(return_data))
    # for c in ci:
    #     report_ci(c)
    
if __name__ == "__main__":
    main()
