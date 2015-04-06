Update:

getDataBlock works well when calling getDataBlock(blank414,475),
but not others, not general enough?

  getDataBlock(data475,530)
  
  getDataBlock(blank475,530)
  
  getDataBlock(data414,475) 

√ getDataBlock(blank414,475)

  getDataBlock(data414,530)

  getDataBlock(blank414,530)

Goal:

get the four functions work in a QT sytle instead of a pure c++ style **(Make sure they stay in a separate file and are imported into mainwindow.cpp)**

readFile()

getDataBlock()

subtractBlanks()

calculate_emFret()