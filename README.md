# IC Layout Synthesis CAD Toolset
<b>Description:</b><br>

This folder includes three toy CAD tools for digital integrated circuit (IC) layout synthesis that demonstrate the algorithms for partitioning, placing, and routing. The graphics package [easygl](https://www.eecg.utoronto.ca/~vaughn/easygl/easygl.html) is from Prof. V. Betz's work, the test case files are provided by Prof. J. Anderson, and the sparse linear systems solver used in the placer is from [Suitesparse](https://github.com/DrTimothyAldenDavis/SuiteSparse) by Prof. T. Davis. 

Brief introductions of the three CAD tools are as follows:<br>
**1. The analytical placer** <br> An analytical-based global placer. The program accepts circuits that include both movable and fixed blocks (dies), places the movable blocks by using conjugate gradient method to minimize the HPWL, then spreads them by using a flow-based algorithm to remove overlaps. When spreading the blocks, the placer can maintain HPWL as much as possible when the anchor cell options are enabled. <br>
**2. The MAX-SAT solver** <br> A depth-first Branch-and-Bound solver for solving unweighted (Vanilla) and weighted maximum satisfiability (MAX-SAT) problems, which accepts multiple clauses Boolean formula expressed in conjunctive-normal form.<br>
**3. The maze router** <br> An field-programmable gate array (FPGA) router implemented Lee's Algorithm. The program allows users to specify the size of the circuit grid, the maximum width of the segment channels (W), and a list of nets that need to be routed. In addition to standard square switch blocks (Fs = 3), the router implements optional octagon switch blocks (Fs = 7). The program is also capable to minimize the segment channel width.

<br><b>Copyright © 2021 [Weixuan Yang](https://www.linkedin.com/in/weixuanyang/)</b>
