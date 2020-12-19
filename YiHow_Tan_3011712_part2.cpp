/**
Student Name: Yi How Tan
Student Number: 3011712
**/
#include <iostream>
#include <mpi.h>
#include <cmath>
/** the world rank and size that will be useful in many functions **/
int world_size;
int world_rank;

// prime couter
int primeCounter(int* num_array, int array_size) {
	int count = 0;
	int i;
	bool isPrime = true;

	// check if the first element of the array is even number
	if (num_array[0] % 2 == 0) {
		// if the first element is 2 then count + 1
		if (num_array[0] == 2)
			count = 1;
		// skip the first element in the loop below
		i = 1;
	}
	else {
		// if first element is 1 then skip 2 element and add 1 to count
		if (num_array[0] == 1) {
			count = 1;
			i = 2;
		}
		else // if first element is not 1 then start the loop from this element
			i = 0;
	}
	// every iteration plus 2 to counter since all the even number(except 2) is not prime number, enhance performance
	for (int a = i; a < array_size; a += 2) {
		isPrime = true;
		// start the loop from 3 
		// plus 2 to the counter for evry iteration, because all the even number(except 2) is not prime number, enhance performance
		for (int c = 3; c * c <= num_array[a]; c += 2) {
			if (num_array[a] % c == 0) {
				isPrime = false;
				break;
			}
		}
		if (isPrime)
			count++;
	}
	return count;
}

// a method to create array of int
int* createArray(int n) {
	int* num_array = new int[n];
	for (int i = 0; i < n; i++)
		num_array[i] = i + 1;

	return num_array;
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


	// arrays and variables that we need for communicating data
	int array_size = 4000;
	int partition_size = array_size / world_size;
	int* total_array = new int[array_size];
	int* partition = new int[partition_size];

	// rank 0 will initial the array
	if (world_rank == 0) {
		std::cout << "Student name: Yi How Tan" << std::endl;
		std::cout << "Student number: 3011712" << std::endl;
		std::cout << std::endl;
		total_array = createArray(array_size);
	}

	// use scatter to send partition to each node
	MPI_Scatter(total_array, partition_size, MPI_INT, partition, partition_size, MPI_INT, 0, MPI_COMM_WORLD);

	// count the primes in partition and print it to console
	int count = primeCounter(partition, partition_size);
	std::cout << "Total amount of primes has encountered in rank " << world_rank << " is " << count << std::endl;

	// use reduce function to get the total primes count
	int total_count = 0;
	MPI_Reduce(&count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	// only print the total primes count in rank 0
	if (world_rank == 0)
		std::cout << "Total amount of primes have encountered in all ranks is " << total_count << std::endl;
	
	delete total_array;
	delete partition;
	// before we can end our application we need to finalise everything so MPI can shut
	// down properly
	MPI_Finalize();
	// standard C/C++ thing to do at the end of main
	return 0;
}