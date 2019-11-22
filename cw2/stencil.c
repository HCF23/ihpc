#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <malloc.h>
#include <mpi.h>

// Define output file name
#define OUTPUT_FILE "stencil.pgm"
#define TILE_SIZE (1024)
             
void stencil(	const int nx, const int ny, 
		const int width, const int height,
             	float* image, float* tmp_image, 
		int subdomain_start, int subdomain_size, int subdomain_end,
		float* tx, float* rx, int message_size);

void init_image(const int nx, const int ny, 
		const int width, const int height,
                float* image, float* tmp_image, 
		int subdomain_start, int subdomain_size, int subdomain_end,
		float* tx, float* rx, int message_size);

void output_image(const char* file_name, const int nx, const int ny,
                  const int width, const int height, 
		  float* image,
 		  int subdomain_start, int subdomain_size, int subdomain_end,
		  float* tx, float* rx, int message_size);

double wtime(void);

void decompose_domain(int domain_size, int world_rank, int world_size,
		      int * subdomain_start, int * subdomain_size,
	    	      int * subdomain_end);


int main(int argc, char* argv[])
{

  MPI_Init(&argc, &argv);

  int nprocs, rank;

  // Check usage
  if (argc != 4) {
    fprintf(stderr, "Usage: %s nx ny niters\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // Initiliase problem dimensions from command line arguments
  int nx = atoi(argv[1]);
  int ny = atoi(argv[2]);
  int niters = atoi(argv[3]);

  // we pad the outer edge of the image to avoid out of range address issues in
  // stencil
  int width = nx + 2;
  int height = ny + 2;
 
  // for every dx, pass all of y 
  int message_size = height*sizeof(float); 

  int *rank_domain_size = malloc(sizeof(int));
  int *rank_domain_start = malloc(sizeof(int));
  int *rank_domain_end = malloc(sizeof(int));

  // Allocate the image, nice and aligned, like.
  float* image = _mm_malloc(sizeof(float) * width * height, 16);
  float* tmp_image = _mm_malloc(sizeof(float) * width * height, 16);

  // Allocate some buffers for message passing between procs
  float* txbuf = malloc(message_size);
  float* rxbuf = malloc(message_size);

  // Work out 'whereami' | 'whoami' for this proc
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  printf(":%d\trank:%d/%d\t start:%d   sz:%d    end:%d\n", 
	__LINE__, rank, nprocs, 
	*rank_domain_start, 
	*rank_domain_size, 
	*rank_domain_end);

  //just decompose x
  //allow full y iteration
  //the resultant subdomains are directly proportional to the current rank
  decompose_domain(nx, rank, nprocs, 
		   rank_domain_start, 
		   rank_domain_size, 
		   rank_domain_end);

  // Set the input image
  init_image(	nx, ny, width, height, image, tmp_image, 
		*rank_domain_start, *rank_domain_size, *rank_domain_end,
		txbuf, rxbuf, message_size);

  printf(":%d\trank:%d/%d\t start:%d   sz:%d    end:%d\n", 
	__LINE__, rank, nprocs, *rank_domain_start, *rank_domain_size, *rank_domain_end);
/*
  // Call the stencil kernel
  double tic = wtime();
  for (int t = 0; t < niters; ++t) {
    printf(":%d\trank %d of %d\n", __LINE__, rank, nprocs);
    stencil(nx, ny, width, height, image, tmp_image, *subdomain_start, *subdomain_size);    
    printf(":%d\trank %d of %d\n", __LINE__, rank, nprocs);
    stencil(nx, ny, width, height, tmp_image, image, *subdomain_start, *subdomain_size);
  }
  double toc = wtime();

  // Output
  printf("------------------------------------\n");
  printf(" runtime: %lf s\n", toc - tic);
  printf("------------------------------------\n");
*/
  printf(":%d\trank:%d/%d\t start:%d   sz:%d    end:%d\n", 
	__LINE__, rank, nprocs, *rank_domain_start, *rank_domain_size, *rank_domain_end);
 
  output_image( OUTPUT_FILE, nx, ny, 
		width, height, 
		image,
		*rank_domain_start, *rank_domain_size, *rank_domain_end,
		txbuf, rxbuf, message_size);

  printf(":%d\trank:%d/%d\t start:%d   sz:%d    end:%d\n", 
	__LINE__, rank, nprocs, *rank_domain_start, *rank_domain_size, *rank_domain_end);
 
  _mm_free(image);
  _mm_free(tmp_image);
  
  free(rank_domain_size);
  free(rank_domain_start); 
  free(rank_domain_end); 
  
  MPI_Finalize();
}

void decompose_domain(  int domain_size, int world_rank, int world_size,
			int * subdomain_start, int * subdomain_size,
			int * subdomain_end)
{
   //catch a nasty situation
   if (world_size > domain_size)
      MPI_Abort(MPI_COMM_WORLD,1);
   
   //Split the domain into equal parts
   *subdomain_size = domain_size / world_size;
   
   if (world_rank == 0) {
      *subdomain_start = 0;
   } else {
      *subdomain_start = domain_size / world_size * world_rank;
   }  

   if (world_rank == world_size - 1) {
      //If the last process, take on the remainder
      *subdomain_size += domain_size % world_size;
   }

   *subdomain_end = *subdomain_start + *subdomain_size;
}


void stencil(   const int nx, const int ny, 
		const int width, const int height,
             	float* image, float* tmp_image, 
		int subdomain_start, int subdomain_size, int subdomain_end,
		float* tx, float* rx, int message_size);
{
  int tile_size = TILE_SIZE;
  int subdomain_max = subdomain_start + subdomain_size;
  //split x up according to rank
  //then for each x iterate over the y
  for (int ib = subdomain_start; ib < subdomain_max + 1; ib += tile_size) {
    for (int jb = ny; jb < ny + 1; jb += tile_size) {
      const int jlim = (jb + tile_size > ny) ? ny : jb + tile_size;
      const int ilim = (ib + tile_size > subdomain_max) ? subdomain_max : ib + tile_size;

      for (int i = ib; i < ilim + 1; ++i) {
        for (int j = jb; j < jlim + 1; ++j) {
          tmp_image[j + i * height]  = image[j     + i       * height] * 3.0f / 5.0f;
          tmp_image[j + i * height] += image[j     + (i - 1) * height] * 0.5f / 5.0f;
          tmp_image[j + i * height] += image[j     + (i + 1) * height] * 0.5f / 5.0f;
          tmp_image[j + i * height] += image[j - 1 + i       * height] * 0.5f / 5.0f;
          tmp_image[j + i * height] += image[j + 1 + i       * height] * 0.5f / 5.0f;
	}
      }
    }
  }
}

// Create the input image
void init_image(const int nx, const int ny, 
		const int width, const int height,
             	float* image, float* tmp_image, 
		int subdomain_start, int subdomain_size, int subdomain_end,
		float* tx, float* rx, int message_size);
{
  // Zero everything
  for (int j = 0; j < ny + 2; ++j) {
    for (int i = 0; i < nx + 2; ++i) {
      image[j + i * height] = 0.0;
      tmp_image[j + i * height] = 0.0;
    }
  }

  const int tile_size = TILE_SIZE;
  //split x up according to rank
  //then for each x iterate over the y
  for (int ib = subdomain_start; ib < subdomain_max + 1; ib += tile_size) {
    for (int jb = ny; jb < ny + 1; jb += tile_size) {
      if ((ib + jb) % (tile_size * 2)) {
         const int jlim = (jb + tile_size > ny) ? ny : jb + tile_size;
         const int ilim = (ib + tile_size > subdomain_max) ? subdomain_max : ib + tile_size;

         for (int i = ib; i < ilim + 1; ++i) {
           for (int j = jb; j < jlim + 1; ++j) {
              image[j + i * height] = 100.0;
          }
        }
      }
    }
  }
}

// Routine to output the image in Netpbm grayscale binary image format
void output_image(const char* file_name, const int nx, const int ny,
		const int width, const int height,
             	float* image,  
		int subdomain_start, int subdomain_size, int subdomain_end,
		float* tx, float* rx, int message_size);
{
  // Open output file
  FILE* fp = fopen(file_name, "w");
  if (!fp) {
    fprintf(stderr, "Error: Could not open %s\n", OUTPUT_FILE);
    exit(EXIT_FAILURE);
  }

  // Ouptut image header
  fprintf(fp, "P5 %d %d 255\n", nx, ny);

  // Calculate maximum value of image
  // This is used to rescale the values
  // to a range of 0-255 for output
  double maximum = 0.0;
  for (int j = 1; j < ny + 1; ++j) {
    for (int i = 1; i < nx + 1; ++i) {
      if (image[j + i * height] > maximum) maximum = image[j + i * height];
    }
  }

  // Output image, converting to numbers 0-255
  for (int j = 1; j < ny + 1; ++j) {
    for (int i = 1; i < nx + 1; ++i) {
      fputc((char)(255.0 * image[j + i * height] / maximum), fp);
    }
  }

  // Close the file
  fclose(fp);
}

// Get the current time in seconds since the Epoch
double wtime(void)
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}


