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

<b>Input File Format:</b><br>
The first line - <number of variables (int)> <number of clauses (int)> <br>
The other lines - <1st variable in the clause (int)> <2nd variable in the clause (int)> ... <0 (int)> <weight(float)> <br>
\* For representing variables, positive integers indicate they are in true form, negatives integers indicate they are in negated form.

<b>File List:</b><br>
<table border="0">
    <tr>
        <td>1.cnf, 2.cnf, 3.cnf, 4.cnf</td>
        <td>Input files</td>
    </tr>
    <tr>
        <td>easygl_constants.h</td>
        <td>Helper head file for easygl</td>
    </tr>
    <tr>
        <td>graphics.cpp</td>
        <td>Source code for easygl</td>
    </tr>
    <tr>
        <td>graphics.h</td>
        <td>Head file for easygl</td>
    </tr>
    <tr>
        <td>main.cpp</td>
        <td>Source code for main program</td>
    </tr>
    <tr>
        <td>makefile</td>
        <td>UNIX makefile</td>
    </tr>
    <tr>
        <td>maxsat.cpp</td>
        <td>Source code for MAX-SAT solver</td>
    </tr>
    <tr>
        <td>maxsat.h</td>
        <td>Head file for MAX-SAT solver</td>
    </tr>
    <tr>
        <td>README.md</td>
        <td>This file</td>
    </tr>
    <tr>
        <td>segment.cpp</td>
        <td>Source code for data structure</td>
    </tr>
    <tr>
        <td>segment.h</td>
        <td>Head file for data structure</td>
    </tr>
</table>

<b>Copyright © 2021 Weixuan Yang</b>
