## Relevant concepts

- **I/O bottlenecks**.  We have learned in previous assignments that writing and reading from files can be slow and 
  one wants to limit the number of read/writes to a file.
  
- **Data type conversion**.  Conversion from string to int can be costly and one has to choose the most effcient method here.
  
- **Multithreading**.  In this case, we will achieve this using OpenMP.

- **Synchronization**.  Or the lack thereof. We want to avoid explicit synchronization if possible.

- **Memory management**.  We will want to load as much of the input data into the memory as possible without breaking 
   the constraints given in the assignment. This to further minimize the read/write operations needed.

- **Data types**.  We will need to pay close attention to what data types we choose to limit memory usage and increase 
   performance in calculations.

***CONTINUE HERE WITH FURTHER CONCEPTS***

## Intended program layout

Per instructions the program naturally splits into two subtasks: The
computation of the Newton iteration and the writing of results to the two
output files. Each of them will be implemented in a separate function, that are
intended to be run as the main function of corresponding POSIX threads.

The computation of the Newton iteration can be further split up into
***INSERT***

As for the writing to file, we have identified ***INSERT*** independent
subtasks. ***INSERT***

***CONTINUE BY FURTHER SPLITTING UP THE TWO TASKS***
