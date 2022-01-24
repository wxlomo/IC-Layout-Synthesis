# A MAX-SAT Solver
<b>Description:</b><br>

For running the code, please:
1) Change directory to the source folder.
2) Type "<i>make</i>" to compile the code. The executable "maxsat" will be created.
3) Type "<i>./maxsat</i>" followed by the path or name of the input file to run the code. Type "<i>-w</i>" then to run the solver in weighted MAX-SAT mode, such as "<i>./maxsat 1 -w</i>", otherwise it will treat all clauses equally weighted.
5) The binary decision tree will be generated automatically, the red dots denote the pruned nodes, and the green dot denotes the best solution node. The Boolean assignments of the variables will be visible on the terminal once the optimization is finished.
6) Type "make clean" to get rid of the object and executable files.

<br><b>Input File Format:</b><br>

The first line - <number of variables (int)> <number of clauses (int)> <br>
The other lines\* - <1st variable in the clause (int)> <2st variable in the clause (int)> ... <0 (int)> <clause weight (int)> <br>
<sub>\* Positive integers represent variables in true form, negatives integers represent variables negated form.</sub>

<br><b>File List:</b><br>

<table border="0">
    <tr>
        <td>cct1, cct2, cct3, cct4</td>
        <td>Test case files</td>
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
        <td>Source code for the MAX-SAT solver</td>
    </tr>
    <tr>
        <td>maxsat.h</td>
        <td>Head file for the MAX-SAT solver</td>
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


<br><b>Copyright © 2021 Weixuan Yang</b>
