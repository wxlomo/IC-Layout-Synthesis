# IC Layout Synthesis CAD Tools
<b>Description:</b><br>

This folder includes three toy CAD tools for digital integrated circuit layout synthesis that demonstrate the algorithms for partitioning, placing, and routing. The codes were prepared for Prof. J. Anderson as submissions for the University of Toronto ECE1387H "CAD for Digital Circuit Synthesis and Layout" course assignments. The graphics package "easygl" is from Prof. V. Betz's work, the test case files are provided by Prof. J. Anderson, and the sparse linear systems solver used in the placer is from "Suitesparse" by Prof. T. Davis. 

Brief introductions of the three CAD tools are as follows:
1. The analytical placer <br> An analytical-based global placer. The program accepts circuits that include both movable and fixed blocks (dies), places the movable blocks by solving the Hessian matrices regarding their coordinates to minimize the HPWL, then spreads them by using a flow-based algorithm to remove overlaps. When spreading the blocks, the placer can maintain HPWL as much as possible when the anchor cell options are enabled. 
2. The MAX-SAT solver <br> A Branch-and-Bound solver for solving unweighted (Vanilla) and weighted maximum satisfiability (MAX-SAT) problems, which accepts multiple clauses Boolean formula expressed in conjunctive-normal form.
4. The maze router <br> An FPGA router using Lee's Algorithm. The program allows users to specify the size of the circuit grid, the maximum width of the segment channels (W), and a list of nets that need to be routed. In addition to standard square switch blocks (Fs = 3), the router implements optional octagon switch blocks (Fs = 7). The program is also capable to minimize the segment channel width.

<br><b>Copyright © 2021 [Weixuan Yang](https://www.linkedin.com/in/weixuanyang/)</b>
