### Compilation

To compile use command:

```
make compile
```

or simply

```
make
```

To delete the compiled files, use:

```
make clean
```

### Execution

To run the program, use:

```
make run i=I k=K c=C r=R
```

where the I, K, C and R are integers and are within the following ranges:

$i \in [1,100]$ where $i$ is the instance number (There are 100 pre-built instances in the MVDRP_Instances folder)

$k \in [1,8]$ where $k$ is the amount of drones to simulate

$c \in \{3,6,10\}$ where c is the carry capacity of each drone

$r \in [1, \infty]$ where $r$ is the amount of restarts the hill-climbing algorithm goes through

#### Example Execution

```
make run i=1 k=2 c=3 r=20
```

Running _MVDRP.cpp_ generates an _output.txt_ file and a _path.txt_ file. 

### Solution Visualization

To visualize the obtained solution, simply run __Path_Visualizer.py_. This will read the _path.txt_ file and plot the route taken by the truck and drones on a two-dimensional plane.

The MVDRP_Instances folder must be in the same folder as the compiled program.

This project was developed on Linux, using WSL on a Windows 10 system.

Tomas Barros, student at UTSFM, Chile