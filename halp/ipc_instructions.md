from https://github.com/UoB-HPC/hpc-course-getting-started

module list 		- list modules loaded
module av <pattern>  	- list available modules <matching pattern>
module av |& grep -i gcc - find gcc related modules 
module load <module>	- load module
module rm <module>	- unload <module>
module purge		- unload all
** some intel modules do not unload - log out and log in for fresh env **
module show languages/gcc-7.1.0 	- shows --help


3_Queueing_Systems
from https://github.com/UoB-HPC/hpc-course-getting-started/blob/master/3_Queueing_Systems.md

workload managers (WLMs) 
=> implement job queue

The typical workflow with a WLM can be summarised as follows:

1)    The user defines their job. This includes the application to be run and the amount of hardware resources that it will need, e.g. number of processor cores, amount of RAM, and so on. This is done on a login node.
2)    The job is submitted to the system's queue. It is not unusual to have tens or hundreds of jobs in the queue at a given time.
3)    The WLM looks at available resources together with queued jobs and any higher/lower priorities applied to them in order to decide which job(s) will run next.
4)    When the requested resources are available and the job starts running, those resources (on compute nodes) become allocated to it and no other job will be able to use it until the current one is stopped.
5)    The job is run according to its definition. It will stop when it either completes, crashes, or exceeds the requested resources, e.g. it has used more CPU time than the job definition requested.
6)    When the job is done, its resources are freed so they can be used for other jobs.

squeue 			- list all jobs on sys
	-u $USER 	- list for specific usr
	-R COMS30005	- reservation
	-A COMS30005	- account

scontrol show -d job 1340971		- all details of job even after
				  	 its complete
sinfo 					- list resourses, 
				  	 organised into partitions
sinfo -p hmem				- query specified partition

scancel					- rm job from queue

srun [options] /path/to/binary		- run binary
sbatch [options] /path/to/script	- run script

	-p <partition> 			- partition to run on
	-N <n>				- request n nodes
	--ntasks-per-node <c> 		- request c tasks per node
	-t <t>				- max job run time, t = hh:mm:ss
	-J <name>			- give job a name
	-o <file>			- specify output file
	--exclusive			- no other jobs allowed
	--gres=gpu:<g>			- request g GPUs
	-A <account>			- usrs grouped into accounts
					 which share resources
					 usr can be signed into multiple
					 so A is used to choose which
	--reservation <name>		- nodes reserved for subset of usr
