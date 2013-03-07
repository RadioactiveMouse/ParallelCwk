// mpitotientrange.c - Parallel MPI Euler Totient Function (C Version)
// compile: make mpi
// run:     ./mpi lower_num uppper_num

// Greg Michaelson 14/10/2003
// Patrick Maier   29/01/2010 [enforced ANSI C compliance]
// Tom Townsend

// This program calculates the sum of the totients between a lower and an 
// upper limit using C longs. It is based on earlier work by:
// Phil Trinder, Nathan Charles, Hans-Wolfgang Loidl and Colin Runciman

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>

// hcf x 0 = x
// hcf x y = hcf y (rem x y)

long hcf(long x, long y)
{
  long t;

  while (y != 0) {
    t = x % y;
    x = y;
    y = t;
  }
  return x;
}


// relprime x y = hcf x y == 1

int relprime(long x, long y)
{
  return hcf(x, y) == 1;
}

// euler n = length (filter (relprime n) [1 .. n-1])

long euler(long n)
{
  long length, i;

  length = 0;
  for (i = 1; i < n; i++)
    if (relprime(n, i))
      length++;
  return length;
}


// sumTotient lower upper = sum (map euler [lower, lower+1 .. upper])

long sumTotient(long lower, long upper)
{
  long sum, i;

  sum = 0;
  for (i = lower; i <= upper; i++)
    sum = sum + euler(i);
  return sum;
}
/*
void worker() {
	
	MPI_Status lstatus, hstatus;
	long lower, upper,totient;

	// receive the lower and upper bound we need to compute
	// TODO : make this into a single receive if possible to remove some communication overhead
	MPI_Recv(&lower,2,MPI_LONG,1,0,MPI_COMM_WORLD,&lstatus); // store, count, type, source, tag, comm, status
	// MPI_Recv(&upper,2,MPI_LONG,1,0,MPI_COMM_WORLD,&hstatus);
	&totient = sumTotient(lower,higher);
	// send the answer back to the coordinator thread
	MPI_Send(&totient,1,MPI_LONG,1,0,MPI_COMM_WORLD); // answer, count, type, destination, tag, comm
}
*/
int main(int argc, char ** argv)
{
	int p,id,i;
	long lower, upper, answer, result, binsize, item[2], *bounds;

	MPI_Init(&argc,&argv); // start vm
	MPI_Comm_size(MPI_COMM_WORLD,&p); // get size of VM 
	MPI_Comm_rank(MPI_COMM_WORLD,&id); // get machine id	

	if (argc != 3) {
	    printf("not 2 arguments\n");
	    return 1;
 	}

	// get inputs
	sscanf(argv[1], "%ld", &lower);
	sscanf(argv[2], "%ld", &upper);
	
	// identify node type
	if(id == 0) {
		// coordinator
		// bin the input to compute
		// bounds should be lower,upper,lower,upper
		printf("Number of bins : %i\n",p);
		bounds = (long*) malloc(p*2*sizeof(long));
		binsize = (upper-lower)/p;
		printf("Binsize : %ld\n",binsize);
		// determine the bin sizes to be handed off to the different workers	
		for(i=0;i<p*2;i++){
			// check to make sure it's the lower bound or not?
			if(i == 0){
				bounds[0] = lower;
			}else if(i%2 != 0){
				bounds[i] = bounds[i-1] + binsize;
			} else {
				bounds[i] = bounds[i-1] + 1;
			}
		}
		int last = p*2-1;
		if(bounds[last] != upper)
			bounds[last] = upper;
		printf("Last = %i\n",last);
	}
	// send the data to a worker threads
	MPI_Scatter(bounds,2,MPI_LONG,item,2,MPI_LONG,0,MPI_COMM_WORLD);
	// find local sumTotient
	answer = sumTotient(item[0],item[1]);
	// reduce answers from all nodes down to result
	MPI_Reduce(&answer, &result, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

	if(id == 0) {
		// coordinator
		printf("Co-ordinator: Sum of Totients  between [%ld..%ld] is %ld\n", lower, upper, result);
	}
	
	MPI_Finalize(); // shutdown VM
	return 0;
}

