# An Analytical Placer 
<b>Description:</b><br>

For running the code, please:
1) Change directory to the source folder.
2) Type "<i>make</i>" to compile the code. The executable "place" will be created.
3) Type "<i>./place</i>" followed by the path or name of the input file, such as "<i>./place cct1</i>".
4) The initial placement will be generated automatically. Press the buttons in the GUI before and while spreading to configure the algorithm:<br>
   "<i>Default</i>" - Reset all the configurations to default* <br>
   "<i>Linear Psi</i>" - Linear cell displacement limitation (&Psi;) increment function <br>
   "<i>Reversed Bin</i>" - Reverse the bin list <br>
   "<i>Strong Anchor</i>" - Implement the anchors with strong weight to optimize HPWL <br>
   "<i>Weak Anchor</i>" - Implement the anchors with weak weight to optimize HPWL <br>
<sub>\* Quadratic incrementing &Psi;, ascending order sorted bin list, and no anchor implementation, which means the blocks are moved to the spread location immediately. </sub>
5) Press the "<i>Proceed</i>" button to continually spread the circuit until it converged.
6) Press the "<i>Index</i>" button to show the index numbers of the blocks.
7) Type "<i>make clean</i>" to get rid of the object and executable files.

<br><b>Input File Format:</b><br>

The first several lines - <block index (int)> <1st net index (int)> <2nd net index (int)> ... <-1 (int)> <br>
The split line - <-1 (int)> <br>
The several lines then - <fixed block index (int)> <fixed block x coordinates (int)> <fixed block y coordinates (int)> <br>
The last line -  <-1 (int)> 

<br><b>File List:</b><br>

<table border="0">
    <tr>
        <td>cct1, cct2, cct3</td>
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
        <td>libamd.a</td>
        <td>SuiteSparse ibrary file</td>
    </tr>
    <tr>
        <td>libsuitesparseconfig.a</td>
        <td>SuiteSparse ibrary file</td>
    </tr>
    <tr>
        <td>libumfpack.a</td>
        <td>SuiteSparse ibrary file</td>
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
        <td>placer.cpp</td>
        <td>Source code for the placer</td>
    </tr>
    <tr>
        <td>placer.h</td>
        <td>Head file for the placer</td>
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
    <tr>
        <td>umfpack</td>
        <td>Folder of SuiteSparse UMFPACK head files</td>
    </tr>
</table>


<br><b>Copyright © 2021 Weixuan Yang</b>
