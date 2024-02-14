# Project 2 Report
### By Max Brombach, Ahmad Hejasebazzi, and Nima Mirzaeian

## Part 1: Warmup Exercises

2.18) In the first loop a[i] is written to and stored in cache. However, assuming a large enough N, it is cached out by the end of the loop's execution. In the second loop, a[i] is then written to again, thus requiring it to be read into cache again. If the process of setting a[i] were in the second loop (the parallelized loop) then the second write would occur when a[i] was still in cache. It is also worth noting that there is no legitimate reason to initialize a[i] to 0 in the first loop since it is immediately overwritten and never used.

2.19) Using a chunksize of 1 is a bad strategy because multiple threads are accessing the same cache block at the same time. This can force each thread to reload cache lines as if it were actually utilizing the same piece of data and drastically slow down each thread. A larger chunksize that is divisible by the size of a cache line is much preferred.

2.21) The follow pseudocode solves the additional problem with the boundary conditions:
```
MPI_Comm_rank(MPI_COMM_WORLD,&myTaskID);
MPI_Comm_size(MPI_COMM_WORLD,&nTasks);
if (myTaskID==0) leftproc = MPI_PROC_NULL;
  else leftproc = myTaskID-1;
if (myTaskID==nTasks-1) rightproc = MPI_PROC_NULL;
  else rightproc = myTaskID+1;
MPI_Sendrecv( &b[LocalProblemSize-1], &bfromleft, rightproc );
MPI_Sendrecv( &b[0], &bfromright, leftproc);
```


2.22)

2.23) Since we assume there is only bandwidth for a single message at a time between the two nodes, and given that we need to send four messages between the two nodes in the distributed scenario vs. only one message in the hybrid scenario, we can determine that the distributed scenario would take 4 times as long as the hybrid. Assuming we are not using fire-and-forget communication directives, each message will require a send and a receive to occur, and, therefore, the messages would not be able to be pipelined. If we assume the latency to send a single message to be `l`, we know each send/receive will take `2l`. Without being able to pipeline, each of the four messages will not overlap in any of their message sending despite the multiple processes. Therefore, in the distributed case, the total time for the send/receive is `8l`. For the hybrid case, there would be 1 bundled message, so the total time for the send/receive would be `2l`.

2.27)

## Part 3: MPI Basics

The output printed the first print statement four times, then the middle statement four times, then the final statement four times despite the first and final statements being before and after the `MPI_Init()` and `MPI_Finalize()` statements, respectively. This is due to the fact that MPI runs all "ranks" in parallel from the beginning of the code to the end, not from the `MPI_Init()` to the `MPI_Finalize()`.

2.3)

2.4)

2.5)

## Part 4: Eat Some Pi


