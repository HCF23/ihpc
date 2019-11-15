#include <mpi.h>
#include <stdlib.h>

struct typedef Walker_struct {
		int location;
		int num_steps_left_in_walk;
	} Walker;

struct typedef node {
		Walker w;
		int val;
		struct node * next;
	} list_t;

void decompose_domain(	int domain_size,
			int world_rank,
			int world_size,
			int* subdomain_start,
			int* subdomain_size)
{
	if (world_size > domain_size) {
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	//Split domain into even chunks:
	*subdomain_start = domain_size / world_size * world_rank;
	*subdomain_size  = domain_size / world_size;

	if (world_rank == world_size - 1 ) {
		//Give the remainder to the last process
		*subdomain_size += domain_size % world_size;
	}
	
}



int main(void)
{
	int* start = malloc(sizeof());
	int* size  = malloc();
	
	decompose_domain(2,1,2,start,size);
	return 0;
}
