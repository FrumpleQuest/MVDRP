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

where I, K, C and R are integers and are within the following ranges:

$i \in [1,100]$ where $i$ is the instance number (There are 100 pre-built instances in the MVDRP_Instances folder)

$k \in [1,8]$ where $k$ is the amount of drones to simulate

$c \in \{3,6,10\}$ where c is the carry capacity of each drone

$r \in [1, \infty]$ where $r$ is the amount of restarts the hill-climbing algorithm goes through

### Example Execution

```
make run i=3 k=2 c=3 r=20
```

Running the above command generates an _output.txt_ file showing the simulation's results, a _path.txt_ file utilized for visualization, and a Visualized_Path.png presenting the path taken by the truck and all drones.

### Usage Notes

The MVDRP_Instances folder must be in the same folder as the compiled program.

This project was developed on Linux, using WSL on Windows 10.

Tomas Barros, student at UTSFM, Chile