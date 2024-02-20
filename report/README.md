# Project 2 Report
### By Max Brombach, Ahmad Hejasebazzi, and Nima Mirzaeian

## Part 1: Warmup Exercises

2.18) In the first loop a[i] is written to and stored in cache. However, assuming a large enough N, it is cached out by the end of the loop's execution. In the second loop, a[i] is then written to again, thus requiring it to be read into cache again. If the process of setting a[i] were in the second loop (the parallelized loop) then the second write would occur when a[i] was still in cache. It is also worth noting that there is no legitimate reason to initialize a[i] to 0 in the first loop since it is immediately overwritten and never used.

2.19) Using a chunksize of 1 is a bad strategy because multiple threads are accessing the same cache block at the same time. This can force each thread to reload cache lines as if it were actually utilizing the same piece of data and drastically slow down each thread. A larger chunksize that is divisible by the size of a cache line is much preferred.

2.21) The following pseudocode solves the additional problem with the boundary conditions:
```
for (i=0; i < LocalProblemSize; i++)
{
  if (myTaskID == 0 && i == 0) {
    a[i] = (b[i] + b[i+1]) / 2;
  }
  else if (myTaskID == ntasks - 1 && i == LocalProblemSize - 1) {
    a[i] = (b[i-1] + b[i]) / 2;
  }
  else {
    if (i==0) {
      bleft = bfromleft;
    }
    else {
      bleft = b[i-1];
    }
    if (i==LocalProblemSize-1) {
      bright = bfromright;
    }
    else {
      bright = b[i+1];
    }  
    a[i] = (b[i] + bleft + bright) / 3;
  }
}
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

1.) 
report/mpi_pi_calc.c [Every rank executes same number of rounds]

report/mpi_pi_calc_div.c [Number of rounds are distributed across ranks]

2.) 
Every rank executes the same number of rounds=100.
Running time for number of ranks=1: 0.041468 

Running time for number of ranks=2: 0.043172 

Running time for number of ranks=3: 0.051310

Running time for number of ranks=4: 0.043451

Running time for number of ranks=20: 0.044228 

The running times for these 5 different runs are very similar since the same number of rounds have
to be executed on each rank per run. There is more computation to be done as we increase ranks but these computations are being
parallelized. There is a slight increase in running time if we keep on adding ranks and this slight increase will become
stable as we keep increasing the number of ranks. This stable slight increase is due to the added setup time and 
because of the reduce operation time increase. That being said, it doesn't seem to be a problem when compared
with the added number of needed computation (number of rounds). This is a weak scaling study.


Note: For each different run, we allocated number of processors equal or greater than the number of ranks to be executed
since we don't want any 2 or morre ranks to be executed on the same core in-serial. 

3.) 

Number of rounds is fixed to = 100. Every rank/process executes 100/(number of processes) rounds.
Remainder number of rounds is equally distributed across ranks (remainder is always less than
number of ranks so each rank at most executes 1 round from the remainder rounds).

Running time for number of ranks=1: 0.043623

Running time for number of ranks=2: 0.019951

Running time for number of ranks=3: 0.013891

Running time for number of ranks=4: 0.010499 

Running time for number of ranks=20: 0.002677


This is a strong scaling study. The number of rounds is fixed to 100 and is distributed across the ranks. The running
time as we increase the ranks is decreasing because fixed computations (across different runs) are being distributed in a single run. Specifically, if we look at the parallel scaling efficiency when we have 2 ranks it's equal to 1.09 which means that there's a super linear speedup when we have 2 ranks. With 3 ranks, the parallel scaling efficiency is 1.04 which means super linear speedup as well. this is the
case as well for ranks=4 with parallel scaling efficiency of 1.03. For ranks=20, the parallel scaling efficiency becomes 0.84 since the 
number of ranks is high enough such that the computatoins have been parallelized as much as possible and now the setup time costs and communication time costs for each newly added rank is greater than what this newly rank can offer in terms of parallelizing computation.

The running time is constantly decreasing which means that there are no bottlenecks such as a communication bottleneck.

Note: For each different run, we allocated number of processors equal or greater than the number of ranks to be executed
since we don't want any 2 ranks to be executed on the same core in-serial. 

4.)
Code: report/mpi_pi_calc_div.c [Change the number of darts accordingly]
Also, change the number of processors allocated in job_q4.sb accordingly (1 node is used only).


5.)
(not done yet)

6.) 
(For all experiments, number of rounds is fixed to 100).

-> Number of darts = e6 (running time of rank=1 is 2.666473 s)
- 2 ranks parallel scaling efficiency: 2.666473 /(1.330916*2) = 1.00174354
- 4 ranks parallel scaling efficiency: 2.666473 /(0.671042*4) = 0.993407641
- 8 ranks parallel scaling efficiency: 2.666473 /(0.354540*8) = 0.940117124
- 16 ranks parallel scaling efficiency: 2.666473 /(0.187276*16) = 0.889887452
- 32 ranks parallel scaling efficiency:  2.666473 /(0.109468*32) = 0.761202189
- 64 ranks parallel scaling efficiency: 2.666473 /(0.065464*64) = 0.636435913

-> Number of darts = e3 (running time of rank=1 is 0.002684 s)
- 2 ranks parallel scaling efficiency: 0.002684 /(0.001342*2) = 1.0
- 4 ranks parallel scaling efficiency: 0.002684 /(0.000671*4) = 1.0
- 8 ranks parallel scaling efficiency: 0.002684 /(0.0003355*8) = 1.0
- 16 ranks parallel scaling efficiency: 0.002684 /(0.00016775*16) = 1.0
- 32 ranks parallel scaling efficiency:  0.002684 /(8.3875e-5*32) = 1.0
- 64 ranks parallel scaling efficiency: 0.002684 /(4.19375e-5*64) = 1.0
  
-> Number of darts = e9 (running time of rank=1 is 2673.575519 s)
- 2 ranks parallel scaling efficiency: 2673.575519 /(1598.9*2) = 0.883
- 4 ranks parallel scaling efficiency: 2673.575519 /(992.74*4) = 0.673
- 8 ranks parallel scaling efficiency: 2673.575519 /(428.475023 *8) =  0.779968311
- 16 ranks parallel scaling efficiency: 2673.575519 /(231.552746*16) = 0.721643223
- 32 ranks parallel scaling efficiency:  2673.575519 /(107.661615*32) = 0.776035498
- 64 ranks parallel scaling efficiency: 2673.575519 /(57.994527*64) = 0.720319996



Note:
(not done yet)
checking for each dart count if parallelism (adding more ranks) keeps on offering running time improvement
or the setup cost and communication cost is greater than the cost of parallelism...
communication cost will be studied in question 7 since the processes will have to communicate across nodes rather
than inter-process communication like shared memory communication. 

small test here for 1e4:
running time is 0.0089 when the ranks are increased to 64 as compared to 0.0026 for 20 ranks.
This is because the computations have been parallelized as much as possible so the setup costs
and communication costs are what will be increased for every rank added. 

7.)
(not done yet) ( same as 6 but use 3-4 different nodes) 

