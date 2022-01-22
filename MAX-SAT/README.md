# A Branch-and-Bound Solver for MAX-SAT
<b>Description:</b><br>
For running the code, please:

1) Change directory to the source folder.

2) Type "make" to compile the code. The executable "maxsat" will be created.

3) Type "./maxsat" with the path or name of the input file to run the code, 
   such as "./maxsat 1". (No need to input the extension for .cnf files)

4) Press the buttons in the GUI to change the configuration of MAX-SAT:
   [Vanilla ] Unweighted MAX-SAT mode (Default)
   [Weighted] Weighted MAX-SAT mode

5) Press "Proceed" button to start finding the optimal solution. The tree 
   will be generated, the red dots denote the pruned nodes, and the green
   dot denotes the best solution node.

6) Type "make clean" to get rid of the object and executable files.

<b>File List:</b><br>
1.cnf, 2.cnf, 3.cnf, 4.cnf                                       Input files
easygl_constants.h                               Helper head file for easygl
graphics.cpp                                          Source code for easygl
graphics.h                                              Head file for easygl
main.cpp                                        Source code for main program
makefile                                                       UNIX makefile
maxsat.cpp                                    Source code for MAX-SAT solver
maxsat.h                                        Head file for MAX-SAT solver
README                                                             This file
segment.cpp                                   Source code for data structure
segment.h                                       Head file for data structure

<b>Copyright © 2021 Weixuan Yang</b>
