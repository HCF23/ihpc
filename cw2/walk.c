#include <mpi.h>
#include <stdlib.h>

typedef struct Walker_node {
		int location;
		int num_steps_left_in_walk;
		int val;
		struct Walker_node * next;
	} walker_t;

int pop(walker_t ** head)
{
	int ret_val = -1;
	walker_t * next = NULL;

	if (*head == NULL) {
		return ret_val;
	}

	next = (*head)->next;
	if (next != NULL) {
		ret_val = 0;		
	}

	free(*head);
	*head = next;

	return ret_val;
}

void push_end(walker_t ** head)
{
	walker_t * current = *head;

	//Advance to the end of the list
	while (current->next != NULL) {
		current = current->next;
	}

	//Malloc new walker	
	current->next = malloc(sizeof(walker_t));
	current->next->val = current->val + 1;
	current->next->location = 0;
	current->next->num_steps_left_in_walk = (*head)->num_steps_left_in_walk;
	current->next->next = NULL;

	//Having put a new copy of head at end
	//we can just pop the older version and move on
	//like nothing happened	
	pop(head);

}

void push(	walker_t ** head, 
		int * location, 
		int * num_steps_left_in_walk)
{
	walker_t * new_walker;
	new_walker = malloc(sizeof(walker_t));

	new_walker->location = *location;
	new_walker->num_steps_left_in_walk = *num_steps_left_in_walk;
	new_walker->next = *head;

	*head = new_walker;
}

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

void init_walkers(	int num_walkers_per_proc,
			int max_walk_size,
			int * subdomain_size,
			int * subdomain_start,
			walker_t ** incoming_walkers)
{
	int num0 = 0;
	int * num = &num0;
	for (int i = 0; i < num_walkers_per_proc; i++) {
		*num = (rand()/(float)RAND_MAX) * max_walk_size;
		push(incoming_walkers, subdomain_start, num);
	}
}

void walk(	walker_t ** walker,
		int subdomain_start,
		int subdomain_size,
		int domain_size)
{
	//Walk until end of num_steps_left_in_walk
	while ((*walker)->num_steps_left_in_walk > 0) {
		if ((*walker)->location == subdomain_start + subdomain_size) {
		//If walker location is at the end of THIS subdomain...
			if ((*walker)->location == domain_size) {
			//If walker location is at the end of the WHOLE domain (sum::SUBdomains)
			//reset walker location ie wrap back around to beginning
				(*walker)->location = 0;
			}
			push_end(walker);		
			break;
		} else {
			(*walker)->num_steps_left_in_walk--;
			(*walker)->location++;
		}
	}
}

int main(void)
{
	int* start = malloc(sizeof(walker_t));
	int* size  = malloc(sizeof(walker_t));

	
	//Init the head of the walker list
	walker_t ** head = NULL;
	
	*head = malloc(sizeof(walker_t));
	if (*head == NULL) {
		return 1;
	} else {
		(*head)->location = 0;
		(*head)->num_steps_left_in_walk = 0;
		(*head)->val = 1;
		(*head)->next = NULL;
	}
	
	//Init the domain
	//decompose_domain(2,1,2,start,size);
	//init_walkers(3,3,start,size,head);
	return 0;
}


