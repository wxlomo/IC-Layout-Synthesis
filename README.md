# IC Layout Synthesis Toys
<b>Description:</b><br>

This folder includes three toy CAD tools for digital integrated circuit layout synthesis that demonstrate the algorithms for partitioning, placing, and routing. The codes were prepared as submissions for the University of Toronto ECE1387H "CAD for Digital Circuit Synthesis and Layout" course assignment. The graphics package "easyGL" is from Prof. V. Betz's work, and the sparse linear systems solver used in the placer is from "Suitesparse" by Prof. T. Davis. 

Brief descriptions of the three CAD tools are as follows:
1. The analytical Placer <br> An analytical-based global placer. The program accepts circuits that include both movable and fixed blocks (dies), places the movable blocks by solving the Hessian matrices regarding their coordinates to minimize the HPWL, then spreads them by using a flow-based algorithm to remove overlaps. When spreading the blocks, the placer can maintain HPWL as much as possible when the anchor cell options are enabled. 
2. The Branch-and-Bound solver for MAX-SAT <br> A solver for solving unweighted (Vanilla) and weighted maximum satisfiability (MAX-SAT) problems, which accepts multiple clauses Boolean formula expressed in conjunctive-normal form.
4. The Maze router <br> An FPGA router using Lee's Algorithm. The program allows users to specify the size of the circuit grid, the maximum width of the segment channels (W), and a list of nets that need to be routed. In addition to standard square switch blocks (Fs = 3), the router implements optional octagon switch blocks (Fs = 7). The program is also capable to minimize the segment channel width.

<br><b>File List:</b><br>

<table border="0">
    <tr>
        <td>AnalyticalPlacer</td>
        <td>The analytical placer</td>
    </tr>
    <tr>
        <td>MAX-SAT</td>
        <td>The Branch-and-Bound solver for MAX-SAT</td>
    </tr>
    <tr>
        <td>MazeRouter</td>
        <td>The Maze router</td>
    </tr>
    <tr>
        <td>README.md</td>
        <td>This file</td>
    </tr>
</table>

<br><b>Copyright © 2021 Weixuan Yang</b>
