# A Branch-and-Bound Solver for MAX-SAT
<b>Description:</b><br>

For running the code, please:
1) Change directory to the source folder.
2) Type "make" to compile the code. The executable "maze" will be created.
3) Type "./maze" with the path or name of the input file to run the code, such as "./maxsat cct1".
4) Add following commands after 3) to configure the router,
   "-oct" - Using octagon switch blocks instead of square switch blocks <br>
   "-min" - Minimizing the channel width usage when routing <br>
5) The graph will be generated automatically. Press "Proceed" button to exit. 
6) Type "make clean" to get rid of the object and executable files.

<br><b>Input File Format:</b><br>

The first line - <grid size (int)> <br>
The second line - <channel width (int)> <br>
The other lines\* - <x coordinate of the source (int)> <y coordinate of the source (int)> <pin number of the source (int)> 
<x coordinate of the sink (int)> <y coordinate of the sink (int)> <pin number of the sink (int)> <br>
The last line - <-1 (int)> <-1 (int)> <-1 (int)> <-1 (int)> <-1 (int)> <-1 (int)>

<br><b>File List:</b><br>
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


<br><b>Copyright © 2021 Weixuan Yang</b>
