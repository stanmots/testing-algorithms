Implementation of different algorithms for solving graph theory problems with testing capabilities.
=================================================================================================

###Currently available approximation algorithms for solving the minimum vertex cover problem include:
- Connected Tree Method;
- Shortest Path Method;
- Guaranteed Predictions Methods (2 versions);
- Greedy algorithm (removing vertices with maximum degree);
- Optimized Method based on the heuristic predictions;
- Method based on the covering pairs searching;

###Each algorithm can be analyzed using such additional output:
- Number of the operations performed by the algorithm;
- Problem solving time for each graph instance used during testing;
- Frequency of non-optimal results (in comparison with the exact algorithm);
- Probability of the non-optimal results occurrences;
- Probability of completing all calculations in allowed time.

Compilation
-----------

You can compile this project with help of QtCreator (main project file is called TestingAlgorithms.pro) or from terminal using the following commands:
~~~ bash
cd your\build\directory\
qmake TestingAlgorithms.pro -r -spec win32-g++ "CONFIG+=release"
mingw32-make -f Makefile.Release
mingw32-make clean
~~~

**Note:** Instead of _mingw32-make_ as well as _-spec_ parameter you must use versions specific to your Operating System. If you wanna compile in Debug mode replace _CONFIG+=release_ with _CONFIG+=debug_ and _Makefile.Release_ with _Makefile.Debug_.
