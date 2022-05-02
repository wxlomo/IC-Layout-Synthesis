# A Maze Router
<b>Description:</b><br>

For running the code, please:
1) Change directory to the source folder.
2) To compile the code, type:
    ```
    make
    ```
3) To run the executable, type the name of it followed by the path or name of the input file:
    ```
    ./maze [PATH OF THE TEST CASE FILE] -oct -min
    ```
    `-oct` - Use octagon switch blocks instead of square switch blocks. <br/>
    `-min` - Minimizing channel width usage when routing.
4) The graph will be generated automatically.
5) To get rid of the object and executable files, type:
    ```
    make clean
    ```

<br><b>Input File Format:</b><br>

The first line: `grid size (int)` <br>
The second line: `channel width (int)` <br>
The other lines: `x coordinate of the source (int)` `y coordinate of the source (int)` `pin number of the source (int)` <br>
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&nbsp;`x coordinate of the sink (int)` `y coordinate of the sink (int)` `pin number of the sink (int)` <br>
The last line: `-1 (int)` `-1 (int)` `-1 (int)` `-1 (int)` `-1 (int)` `-1 (int)`

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
        <td>Source code for the maze router</td>
    </tr>
    <tr>
        <td>router.h</td>
        <td>Head file for the maze router</td>
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
