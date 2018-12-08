## Word count using MPI and c++

### prerequisites
- [X] g++
- [X] mpich

### to compile code
use the mpic++ command not g++
```console
mpic++ main.cpp Map.cpp Combiner.cpp  Reduce.cpp 
```

### to run code
use the mpirun command with -np to specify number of proc

```console
 mpirun -np 4 a.out 
```
