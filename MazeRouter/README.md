# A Branch-and-Bound Solver for MAX-SAT
<b>Description:</b><br>

For running the code, please:
1) Change directory to the source folder.
2) Type "make" to compile the code. The executable "maze" will be created.
3) For running the router, type "./maze" followed by the path or name of the input file. Following commands can be added then to configure the router: <br>
   "-oct" - Using octagon switch blocks instead of square switch blocks <br>
   "-min" - Minimizing channel width usage when routing <br>
   Such as "./maze cct1 -oct" to route the circuit 'cct1' with the octagon switch blocks and without channel width minimization.
4) The graph will be generated automatically. Press the "Proceed" button to exit. 
5) Type "make clean" to get rid of the object and executable files.

<br><b>Input File Format:</b><br>

The first line - <grid size (int)> <br>
The second line - <channel width (int)> <br>
The other lines - <x coordinate of the source (int)> <y coordinate of the source (int)> <pin number of the source (int)> <br>
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&nbsp;<x coordinate of the sink (int)> <y coordinate of the sink (int)> <pin number of the sink (int)> <br>
The last line - <-1 (int)> <-1 (int)> <-1 (int)> <-1 (int)> <-1 (int)> <-1 (int)>

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
        <td>README.md</td>
        <td>This file</td>
    </tr>
    <tr>
        <td>router.cpp</td>
        <td>Source code for Maze router</td>
    </tr>
    <tr>
        <td>router.h</td>
        <td>Head file for Maze router</td>
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
