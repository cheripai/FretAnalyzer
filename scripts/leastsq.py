from scipy.optimize import leastsq
import numpy as np
import matplotlib.pyplot as plt


def main():
   # data provided
   x = np.array([10.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.5, 0.0])
   y = np.array([1506945.0, 1539493.0, 1559078.0, 1570980.0, 1463291.0, 1341564.0, 1266176.0, 1204986.0, 800148.8, 539888.1, 201413.1, -73367.02])    # EmFret values

   # tpl is a tuple that contains the parameters of the fit
   a = 3
   emfret = lambda tpl,x : tpl[1] * (1 - (2 * tpl[0]) / (x - a + tpl[0] +  np.sqrt((x - a - tpl[0]) ** 2) + 4 * tpl[0] * x))

   # set the function to run optimize on
   func = emfret

   #tplInitial contains the "first guess" of the parameters 
   tplInitial = (1,10000)

   # ErrorFunc is the diference between the func and the y "experimental" data
   ErrorFunc = lambda tpl,x,y: func(tpl,x)-y

   # leastsq finds the set of parameters in the tuple tpl that minimizes
   # ErrorFunc=yfit-yExperimental
   tplFinal,success = leastsq(ErrorFunc,tplInitial[:],args=(x,y),ftol=0.0001,xtol=0.0001)
   print(tplFinal)

   xx1 = np.linspace(x.min(),x.max(),50)
   yy1 = func(tplFinal,xx1)
   plt.plot(x,y,'bo',xx1,yy1,'g-')
   plt.show()

if __name__ == "__main__":
   main()
