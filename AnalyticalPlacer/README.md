# An Analytical Placer 
<b>Description:</b><br>

For running the code, please:
1) Change directory to the source folder.
2) Type "make" to compile the code. The executable "place" will be created.
3) Type "./place" followed by the path or name of the input file to run the code, such as "./place cct1". (No need to input the extension for .txt files)
4) The initial placement will be generated automatically. Press the buttons in the GUI to change the configuration of spreading:<br>
   "Default" - Reset all the configurations to default* <br>
   "Linear Psi" - Linear cell limitation (&Psi;) increment function <br>
   "Reversed Bin" - Reverse the bin list <br>
   "Strong Anchor" - Strong anchor weight <br>
   "Weak Anchor" - Weak anchor weight <br>
   <sub>\* Original &Psi; increment function, ascending order sorted bin list, and no anchor implementation, which means the blocks are moved to the spread location immediately. </sub>
5) Press "Proceed" button to continuelly spread the circuit until it converged.
6) Press "Index" button to show the index numbers of the blocks
7) Type "make clean" to get rid of the object and executable files.

<br><b>Input File Format:</b><br>

The first several lines - <block index (int)> <1st net index (int)> <2nd net index (int)> ... <-1 (int)> <br>
The split line - <-1 (int> <br>
The several lines then - <block index (int)> <block x coordinates (int)> <block y coordinates (int)> <br>
The last line -  <-1 (int> 

<br><b>File List:</b><br>

<table border="0">
    <tr>
        <td>cct1.txt, cct2.txt, cct3.txt</td>
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
