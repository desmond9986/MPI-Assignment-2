/**
Student Name: Yi How Tan
Student Number: 3011712
Processor: i5-7200u
Result: 4.82e-05, 0.0028779, 0.102941,6.10574
**/
#include <iostream>
#include <mpi.h>
#include <cmath>

/** the world rank and size that will be useful in many functions **/
int world_size;
int world_rank;

// prime couter
int primeCounter(int n) {
	int count = 0;
	for (int a = 2; a < n; a++) {
		bool isPrime = true;
		for (int c = 2; c * c <= a; c++) {
			if (a % c == 0) {
				isPrime = false;
				break;
			}
		}
		if (isPrime) count++;
	}
	return count;
}


int main(int argc, char** argv) {
	// see if we can initialise the mpi library this is always the first thing that we
	// must do at the start of an MPI program
	MPI_Init(NULL, NULL);
	// one of the important tasks that we have to establish is how many processes are in
	// this MPI instance. this will determine who and what we have to communicate with
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	// another important job is finding out which rank we are. we can use this rank
	// number to assign seperate jobs to different mpi units
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	// arrays that we need for communicating data
	double* total_array = new double[world_size];
	double* partition = new double[1];

	// record start time
	double t;
	t = MPI_Wtime();

	primeCounter(pow(25, (world_rank + 2)));

	// record end time
	t = MPI_Wtime() - t;

	// put the duration in array
	partition[0] = t;

	// use gather to gather the array in node 2 
	MPI_Gather(partition, 1, MPI_DOUBLE, total_array, 1, MPI_DOUBLE, 2, MPI_COMM_WORLD);

	// only display the results in node 2
	if (world_rank == 2) {
		// print name and student number to console once
		std::cout << "Student name: Yi How Tan" << std::endl;
		std::cout << "Student number: 3011712" << std::endl;
		std::cout << std::endl;
		// print duration taken by all ranks
		for (unsigned int i = 0; i < world_size; i++) {
			std::cout << "Duration for rank " << (i + 1) << " is " << total_array[i] << " seconds." << std::endl;
		}
	}
	// before we can end our application we need to finalise everything so MPI can shut
	// down properly
	MPI_Finalize();
	delete partition;
	delete total_array;
	// standard C/C++ thing to do at the end of main
	return 0;
}