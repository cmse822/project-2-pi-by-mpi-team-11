#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void srandom (unsigned seed);  
double dboard (int darts);


int main(int argc, char *argv[])
{

//ROUNDS is fixed across processes here
int ROUNDS=100;
int DARTS = 10000;
double pi;          	/* average of pi after "darts" is thrown */
double avepi;       	/* average pi value for all iterations */
int i, n;
double reduced_val;


MPI_Init(&argc, &argv);

//Only will be used by the rank 0 process
double end_time, time_elapsed, start_time = MPI_Wtime();

//MPI_Comm object initialize
MPI_Comm comm = MPI_COMM_WORLD;

//rank of process ; size of main process group
int rank_id, size_proc;

/*getting rank of process in the process group
 *where comm is the communicator.
*/ 
MPI_Comm_rank(comm , &rank_id);


/*getting the number of processes
 *in the main process group
*/
MPI_Comm_size(comm, &size_proc);

/*multiplying by rank_id since the
 *seed is an integer so time(NULL)
 *will probably be the same for the
 *different processes thus same seed  
*/
srandom(time(NULL)*rank_id);

avepi = 0;

for (i = 0; i < ROUNDS; i++) 
   {
     /* Perform pi calculation on serial processor */
     pi = dboard(DARTS);
    
     //No averaging of pi yet
     avepi = avepi + pi;
           
   } 


/*this is the average for one process since
 *one process performs ROUNDS rounds.
*/
double pi_est_here = avepi / (ROUNDS);
printf("PI Estimate is %f for rank  %d \n",pi_est_here, rank_id);


/*this is what is reduced to the root rank
 *to calculate the average pi value across all
 *rounds in all processes. there are
 * size_proc * ROUNDS rounds.
*/
double pi_proc = avepi / (size_proc*ROUNDS); 


/*bringing together the estimated pis per each process
 *by summing them and storing the sum in reduced_val
 *variable in the rank=0 process ; all other processes'
 *reduced_val stay the same as before the routine call
*/
MPI_Reduce(&pi_proc, &reduced_val, 1, MPI_DOUBLE, MPI_SUM, 0, comm);


if(rank_id==0)
{
end_time =  MPI_Wtime();
time_elapsed = end_time - start_time;
printf("Final Pi Estimate is %f After Time Elapse of %f \n",reduced_val,time_elapsed);
}

MPI_Finalize();

}



/*****************************************************************************
 *  * dboard
 *   *****************************************************************************/
#define sqr(x)	((x)*(x))
long random(void);

double dboard(int darts)
{
   double x_coord,       /* x coordinate, between -1 and 1  */
          y_coord,       /* y coordinate, between -1 and 1  */
          pi,            /* pi  */
          r;             /* random number scaled between 0 and 1  */
   int score,            /* number of darts that hit circle */
       n;
   unsigned int cconst;  /* must be 4-bytes in size */
/*************************************************************************
 *  * The cconst variable must be 4 bytes. We check this and bail if it is
 *   * not the right size
 *    ************************************************************************/
   if (sizeof(cconst) != 4) {
      printf("Wrong data size for cconst variable in dboard routine!\n");
      printf("See comments in source file. Quitting.\n");
      exit(1);
      }
   /* 2 bit shifted to MAX_RAND later used to scale random number between 0 and 1 */
   cconst = 2 << (31 - 1);
   score = 0;

/***********************************************************************
 *  * Throw darts at board.  Done by generating random numbers
 *   * between 0 and 1 and converting them to values for x and y
 *    * coordinates and then testing to see if they "land" in
 *     * the circle."  If so, score is incremented.  After throwing the
 *      * specified number of darts, pi is calculated.  The computed value
 *       * of pi is returned as the value of this function, dboard.
 *        ************************************************************************/

   for (n = 1; n <= darts; n++) {
      /* generate random numbers for x and y coordinates */
      r = (double)random()/cconst;
      x_coord = (2.0 * r) - 1.0;
      r = (double)random()/cconst;
      y_coord = (2.0 * r) - 1.0;

      /* if dart lands in circle, increment score */
      if ((sqr(x_coord) + sqr(y_coord)) <= 1.0)
         score++;
      }

   /* calculate pi */
   pi = 4.0 * (double)score/(double)darts;
   return(pi);
} 
