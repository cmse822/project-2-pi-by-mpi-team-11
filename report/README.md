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

2.3) Code File: report/part3/2.3.c ; Job Script: report/part3/job_2.3.sb

2.4) Code File: report/part3/2.4.c

2.5) Code File: report/part3/2.5.c

## Part 4: Eat Some Pi

1.) 

Code Files:
report/mpi_pi_calc.c [Every rank executes same number of rounds]
report/mpi_pi_calc_div.c [Number of rounds are distributed across ranks]

Compilation:
mpicc mpi_pi_calc.c -o mpi_pi_calc
mpiexec -np 4 mpi_pi_calc



2.) 

-> Number of rounds is fixed to 100.

-> Every rank executes the same number of rounds=100.

-> The number of darts thrown is fixed to 10000.

-> report/mpi_pi_calc.c [Every rank executes same number of rounds]
 is the relevant code.

Results:

Running time for number of ranks=1: 0.041468 

Running time for number of ranks=2: 0.043172 

Running time for number of ranks=3: 0.051310

Running time for number of ranks=4: 0.043451

Running time for number of ranks=20: 0.044228 

The running times for these 5 different runs are very similar since the same number of rounds have to be executed on each rank per run and the ranks run in parallel. There is more computation to be done as we increase ranks but these computations are being
parallelized. There is a slight increase in running time if we keep on adding ranks and this slight increase will become more stable as we keep increasing the number of ranks. This stable slight increase is due to the added setup time and because of the “reduce” operation time increase with each additional ranks. That being said, it doesn't seem to be a problem when compared
with the added number of needed computation (number of rounds). This is a weak scaling study.


Note: For each different run (certain rank run), we allocated the number of processors equal or greater than the number of ranks to be executed since we don't want any 2 or more ranks to be executed on the same core in-serial. 




3.) 

-> 1 node is being utilized here so we refer to single machine inter-process communication done by MPI as communication costs (inter-process communication time is negligible).

-> Number of rounds is fixed to 100 (will be distributed across R ranks).

-> The number of darts thrown is fixed to 10000.

-> report/mpi_pi_calc_div.c  [Number of rounds are distributed across ranks]
 is the relevant code.

Every rank/process executes 100/(number of processes) rounds since the rounds are distributed across the processes. The remainder number of rounds is equally distributed across ranks (remainder is always less than number of ranks so each rank at most executes 1 round from the remainder rounds).

Running time for number of ranks=1: 0.043623

Running time for number of ranks=2: 0.019951

Running time for number of ranks=3: 0.013891

Running time for number of ranks=4: 0.010499 

Running time for number of ranks=20: 0.002677


This is a strong scaling study. The number of rounds is fixed to 100 and is distributed across the ranks. The running time as we increase the ranks is decreasing because fixed computations of 100 rounds (across different runs) are being distributed in a single experimental run. Specifically, if we look at the parallel scaling efficiency when we have 2 ranks it's equal to 0.043623/ (2 * 0.019951) = 1.093 which means that there's a superlinear speedup when we have 2 ranks. With 3 ranks, the parallel scaling efficiency is 0.043623/(3 * 0.013891)=1.047 which means there is a super linear speedup as well. This is the case as well for 4 ranks where the parallel scaling efficiency is (0.043623) / (4 * 0.010499) = 1.039. Although the speedup is superlinear as well for number of ranks = to 3 and 4, we can see that the parallel scaling efficiency is slowly decreasing as we increase the number of ranks. To demonstrate that, we run the program on 20 ranks and calculate the parallel scaling efficiency to get 0.043623 / (20 * 0.002677)=0.815 which is much lower than the parallel scaling efficiency we got for ranks 2 to 4. This is because the number of ranks is high enough such that the computations have been parallelized as much as possible and now the setup time costs and communication time costs for each newly added rank is greater than what this newly added rank can offer in terms of parallelizing computation.  It’s important to note that communication cost is probably negligible since the ranks are running on the same node so shared memory interprocess communication mechanisms would probably be used by MPI to communicate across processes and the communication is basically minimal since there’s only a single reduce operation at the end which means that the setup costs and cache miss effects are the major factor when scaling to more ranks since there might be more cache misses especially since the dart count studied here is e4.

The running time is constantly decreasing which means that there are no major bottlenecks such as a communication bottleneck but the rate at which the running time is decreasing is getting smaller and smaller as we increase the number of ranks which can be reflected by the parallel scaling efficiency getting lower each time we add more ranks and this is because the setup (major factor) + more cache misses + interprocess communication costs are constantly increasing and start adding up with more and more ranks added until the running time plateaus.


Note: For each different run (certain rank run), we allocated the number of processors equal or greater than the number of ranks to be executed since we don't want any 2 or more ranks to be executed on the same core in-serial. 




4.)

-> Code file: report/mpi_pi_calc_div.c [Same code file as question 3 but here we change the number of darts thrown accordingly]

-> Job script: job_q4.sb

-> We change the number of processors allocated in job_q4.sb accordingly (1 node is used only).

-> #SBATCH --constraint=amr is used to run our processes on an amr node.

5.)
As the referenced figure shows, the computed error generally decreased with the number of darts for any rank. This indicates that the calculated pi number becomes more accurate as the number of darts is increased. On the other hand, for each of the cases with a fixed number of darts, the log-log curves show an increase in computed error followed by a decrease at higher node numbers. Therefore, the ideal algorithm, where the accuracy, computation time, and resources are desired, requires a higher number of darts and moderate rank usage.


6.) 

-> For all experiments in question 6, the number of rounds is fixed to 100 which are distributed across ranks as in question 2.

-> 1 node is being utilized here so we refer to single machine ipc communication done by MPI as communication costs. It’s important to note that communication cost is probably negligible since the ranks are running on the same node so shared memory interprocess communication mechanisms would probably be used by MPI to communicate across processes and the communication is basically minimal since there’s only a single reduce operation which means that the setup costs and cache accesses as well as TLB accesses are the major factor when scaling to more ranks.

-> Linear scale is used to plot runtime versus processor count for each dart count. If we view it in log-log scale then the ideal scaling line would be linear.

-> Number of darts = e3 (running time of rank=1 is 0.002684 s) [figure report/q6_e3.png]
- 2 ranks parallel scaling efficiency: 0.002684 /(0.001480*2) = 0.90675
- 4 ranks parallel scaling efficiency: 0.002684 /(0.000803*4) = 0.83561
- 8 ranks parallel scaling efficiency: 0.002684 /(0.000574*8) = 0.5844
- 16 ranks parallel scaling efficiency: 0.002684 /(0.000797*16) = 0.2104
- 32 ranks parallel scaling efficiency:  0.002684 /(0.000364*32) = 0.23042
- 64 ranks parallel scaling efficiency: 0.002684 /(0.000336*64) = 0.124813988



-> Number of darts = e6 (running time of rank=1 is 2.666473 s) [figure report/q6_e6.png]
- 2 ranks parallel scaling efficiency: 2.666473 /(1.330916*2) = 1.00174354
- 4 ranks parallel scaling efficiency: 2.666473 /(0.671042*4) = 0.993407641
- 8 ranks parallel scaling efficiency: 2.666473 /(0.354540*8) = 0.940117124
- 16 ranks parallel scaling efficiency: 2.666473 /(0.187276*16) = 0.889887452
- 32 ranks parallel scaling efficiency:  2.666473 /(0.109468*32) = 0.761202189
- 64 ranks parallel scaling efficiency: 2.666473 /(0.065464*64) = 0.636435913



  
-> Number of darts = e9 (running time of rank=1 is 2673.575519 s) [figure report/q6_e9.png]
- 2 ranks parallel scaling efficiency: 2673.575519 /(1315.88*2) = 1.015
- 4 ranks parallel scaling efficiency: 2673.575519 /(657.401*4) = 1.017
- 8 ranks parallel scaling efficiency: 2673.575519 /( 342.02*8) =  0.977
- 16 ranks parallel scaling efficiency: 2673.575519 /(189.727*16) = 0.881
- 32 ranks parallel scaling efficiency:  2673.575519 /(108.058*32) = 0.773
- 64 ranks parallel scaling efficiency: 2673.575519 /(55.918*64) = 0.747


=> Based on the results above and the 3 figures plotted, we can first observe that the running time increases as the number of darts increases since each round will compute the same number of dart throws and the rounds are distributed across ranks.

=> For e3 dart counts, we can observe from the plot that the running time sharply decreases if we add an additional rank to get 2 ranks (parallelizing). For more rank additions, the running time decreases slightly then subsequently increases sharply and finally plateaus. We tried running the whole experiment multiple times but we noticed that the results are very noisy for the e3 dart counts experiment since the running times are very low. In conclusion, for e3 dart counts, the running time was very noisy and there’s no point in parallelizing it beyond 2 ranks since the time to setup and the consequences of a lot of L1 cache misses (problem size is very small and we want to parallelize it across different cores) would cost more than the benefit that comes from parallelizing it. This can be observed if we look at the high rate of parallel efficiency decrease and this is mainly because of the setup costs for a lot of processes for a small-sized problem in addition to a lot of inefficient memory accesses for a small-sized problem that's distributed across cores. 

=> For e6 dart counts, the speedup looks ideal at least for ranks = 2 to 4  [parallel scaling efficiency is 1.0 to 0.99 in our calculations above] which can be seen in the plot. The number of dart counts =e6 looks large enough so that it would benefit from around 2 to 8 added ranks to parallelize the computation without the setup or pattern of cache accesses being a bottleneck. Similar to question 2, as we keep increasing the ranks past 4, the running time will keep on decreasing but the rate of decrease of the running time which can be reflected by the parallel scaling efficiency will start decreasing because the optimal number of ranks to parallelize has already been reached. In other words, the computations would have been parallelized as much as possible and now the setup time costs, cache misses costs, and interprocess communication (even though negligible / same node shared memory) time costs for each newly added rank are catching up little by little such that the benefit from adding more and more processors is getting lower and lower and the overhead costs are getting higher and higher. When compared with the e3 dart counts however, the rate parallel scaling efficiency decrease is much lower since the problem size is bigger so the processes can greatly benefit from reduction of cache misses in addition to the setup costs being fixed.



=> For e9 dart counts, the problem size is bigger and the parallel scaling efficiency is much more efficient than smaller problems such as the e6 and e3 dart counts. This is because there’s more parallel computation to be done in each round for every process so the rate of decrease of parallel scaling efficiency as we add more ranks in the e9 dart counts problem is less than the rate of decrease of parallel scaling efficiency as we add more ranks in the e6 or e3 dart counts problem. In other words, it takes longer than in the e6 darts count problem to reach a point where adding more parallelization of computation doesn’t have any the same benefit as before because of “setup costs + cache misses costs + interprocess communication costs (even though negligible)” slowly becoming the main source of additional costs. Based on these observations, we argue that the major reason why the rate of decrease of parallel scaling efficiency for the e9 dart counts problem is lower than that for the e6 dart counts problem is because the problem size is bigger and there’s a reduction in cache misses and improved memory access patterns.





7.)

-> We used 4 nodes and we distributed the ranks across the nodes equally as shown in job_q7.sb.

-> We didn't put a node constraint when requesting the resources.

-> We applied question 4 again on e9 dart counts but using 4 nodes where the ranks are distributed equally across the 4 nodes. 

-> Number of darts = e9 (running time of rank=1 is 2673.575519 s) 
- 4 ranks (4 nodes used) parallel scaling efficiency: 2673.575519 /(1048.64 *4) = 0.637
- 8 ranks (4 nodes used) parallel scaling efficiency: 2673.575519 /( 545.71 *8) =  0.612
- 16 ranks (4 nodes used) parallel scaling efficiency: 2673.575519 /(477.30 *16) = 0.350
- 32 ranks (4 nodes used) parallel scaling efficiency:  2673.575519 /(201.18 *32) = 0.415
- 64 ranks (4 nodes used) parallel scaling efficiency: 2673.575519 /(96.74 * 64) = 0.43

-> The running time for e9 dart counts using 4 nodes is around double the running time for e9 dart counts when 1 node is used which was
reported in question 6. So, the parallel scaling efficiency will be consistently lower as shown above. Moreover, the decrease in the rate of parallel scaling efficiency as ranks increase is much higher when 4 nodes are being used. This is because communication plays a major factor here because the nodes are separate machines that have to communicate (they don't share memory ; no ipc communication) where all cores in a single node share the same network interface.



