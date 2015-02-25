## How to get this exe

https://drive.google.com/open?id=0B428rXBxUzFeOTRqczI3UkEzTEE&authuser=0


### download and install Anaconda: A free distribution for the SciPy stack. Supports Linux, Windows and Mac.


from
http://www.scipy.org/install.html


http://continuum.io/downloads


download pyinstaller from
https://github.com/pyinstaller/pyinstaller/wiki


you might need pywin32 as the prerequiste for pyinstaller:


http://sourceforge.net/projects/pywin32/files/

### get into cmd of windows 


### type <code>ipython</code> to get into the environment 
 
 
<strong>notice: the commands below need to be run in ipython. because Anaconda already integrated the scipy, numpy and other packages.</strong>


### cd to the directory of pyinstaller.py


<code>run setup.py install</code> to install pyinstaller from source


### to compile
<code>run <...directory...>\pyinstaller.py -F <...directory...>\fret.py</code> 


### to run
<code>fret/dist/fret.exe</code> 

