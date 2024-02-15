# Project 2 Report
### By Max Brombach, Ahmad Hejasebazzi, and Nima Mirzaeian

## Part 1: Warmup Exercises

2.18) In the first loop a[i] is written to and stored in cache. However, assuming a large enough N, it is cached out by the end of the loop's execution. In the second loop, a[i] is then written to again, thus requiring it to be read into cache again. If the process of setting a[i] were in the second loop (the parallelized loop) then the second write would occur when a[i] was still in cache. It is also worth noting that there is no legitimate reason to initialize a[i] to 0 in the first loop since it is immediately overwritten and never used.

2.19) Using a chunksize of 1 is a bad strategy because multiple threads are accessing the same cache block at the same time. This can force each thread to reload cache lines as if it were actually utilizing the same piece of data and drastically slow down each thread. A larger chunksize that is divisible by the size of a cache line is much preferred.

2.21) The following pseudocode solves the additional problem with the boundary conditions:
```

```

2.22) The following pseudocode solves the problem in equation 2.5 with ISends and IReceives:
```
MPI_Comm_rank(MPI_COMM_WORLD,&myTaskID);
MPI_Comm_size(MPI_COMM_WORLD,&ntasks);
leftproc = myTaskID - 1;
rightproc = myTaskID + 1;
if (myTaskID == 0) {
  leftproc = ntasks - 1;
}
if (myTaskID == ntasks - 1) {
  rightproc = 0;
}
MPI_Isend(&x, rightproc, &righthandle);
MPI_Irecv(&x_left, leftproc, &lefthandle);
MPI_Wait(lefthandle);
y = y + x_left;
```
The downside to this would be that there is no guarantee of receipt of the sent variable `x`.

2.23) Since we assume there is only bandwidth for a single message at a time between the two nodes, and given that we need to send four messages between the two nodes in the distributed scenario vs. only one message in the hybrid scenario, we can determine that the distributed scenario would take 4 times as long as the hybrid. Assuming we are not using fire-and-forget communication directives, each message will require a send and a receive to occur, and, therefore, the messages would not be able to be pipelined. If we assume the latency to send a single message to be `l`, we know each send/receive will take `2l`. Without being able to pipeline, each of the four messages will not overlap in any of their message sending despite the multiple processes. Therefore, in the distributed case, the total time for the send/receive is `8l`. For the hybrid case, there would be 1 bundled message, so the total time for the send/receive would be `2l`.

2.27) The parallelism analysis is crucial in order to correctly order the loops for the loop exchange. Since i is the only loop that is parallelizable, and given that we can fit all of C and one row of B into cache, the i-loop must be exchanged with the n-loop in order to parallelize the code. The loop after exchange is still parallelizable, but only on i, whereas the prior example was parallelizable on i and j.

## Part 3: MPI Basics

The output printed the first print statement four times, then the middle statement four times, then the final statement four times despite the first and final statements being before and after the `MPI_Init()` and `MPI_Finalize()` statements, respectively. This is due to the fact that MPI runs all "ranks" in parallel from the beginning of the code to the end, not from the `MPI_Init()` to the `MPI_Finalize()`.

2.3)

2.4)

2.5)

## Part 4: Eat Some Pi


