## Link of fret.exe

https://drive.google.com/open?id=0B428rXBxUzFeOTRqczI3UkEzTEE&authuser=0

### 1. Preparation
  
download and install <strong>Anaconda</strong>: 

<small>A free distribution for the SciPy stack. Supports Linux, Windows and Mac.</small>


from
+ http://www.scipy.org/install.html
+ http://continuum.io/downloads


download <strong>pyinstaller </strong>
from
+ https://github.com/pyinstaller/pyinstaller/wiki


you might need <strong>pywin32</strong> as the prerequiste for pyinstaller:


+ http://sourceforge.net/projects/pywin32/files/

### 2. get into cmd of windows (win+R cmd)

type <code>ipython</code> to get into the environment 
 
<strong>notice: the commands below need to run in <code>ipython</code>. 
Because Anaconda already integrated the scipy, numpy and other packages.</strong>


### 3. <code>cd</code> to the directory of pyinstaller.py

type <code>run setup.py install</code> to install pyinstaller from source

### to compile
<code>run <...directory...>\pyinstaller.py -F <...directory...>\fret.py</code> 

### to run
<code><...directory...>\fret\dist\fret.exe</code> 