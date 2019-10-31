#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <malloc.h>

// Define output file name
#define OUTPUT_FILE "stencil.pgm"
#define mNoTen (10)

void stencil(const int nx, const int ny, const int width, const int height,
             double* image, double* tmp_image);
void init_image(const int nx, const int ny, const int width, const int height,
                double* image, double* tmp_image);
void output_image(const char* file_name, const int nx, const int ny,
                  const int width, const int height, double* image);
double wtime(void);

int main(int argc, char* argv[])
{
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

  // Allocate the image
  // in an aligned fashion, sizeof(double) = 8 [bytes] using posix_memalign or
  //  _mm_malloc(SIZE, CACHE_LINE_SIZE)
  double* image     = (double *) _mm_malloc(sizeof(double) * width * height, 64);
  double* tmp_image = (double *) _mm_malloc(sizeof(double) * width * height, 64);

  // Set the input image`:
  init_image(nx, ny, width, height, image, tmp_image);

  // Call the stencil kernel
  double tic = wtime();
  for (int t = 0; t < niters; ++t) {
    stencil(nx, ny, width, height, image, tmp_image);
    stencil(nx, ny, width, height, tmp_image, image);
  }
  double toc = wtime();

  // Open output file
  FILE* fp = fopen("stencil.csv","a");
  if (!fp) {
    printf("Error: Could not open \"stencil.csv\"\n");
    exit(EXIT_FAILURE);
  }

  // Ouptut program run args and runtime 
  printf("%d;%d;%d;%lf\n", nx, ny, niters, (toc - tic)); 
  fprintf(fp, "%d;%d;%d;%lf\n", nx, ny, niters, (toc - tic)); 
  fclose(fp);

  output_image(OUTPUT_FILE, nx, ny, width, height, image);
  _mm_free(image);
  _mm_free(tmp_image);
}

__inline__ void 
stencil(const int nx, const int ny, const int width, const int height,
             double* /*restrict*/ image, 
	     double* /*restrict*/ tmp_image)
{
  __assume_aligned(image, 32);
  __assume_aligned(tmp_image, 32);

  __assume(nx%32==0);
  __assume(ny%32==0);
  __assume(height%32==0);

  //reference image[j+i*(ny+2)] has unaligned acces
	
//if tile size == cache size
//should be able to vectorise as well
//so long as the data is aligned to 64
//therefore
//match memory as it was initialised
//to ensure maximal alignment
  const int tile_size = 64;
  for (int jb = 0; jb < ny; jb+=tile_size) {
    for (int ib = 0; ib < nx; ib+=tile_size) {

      const int jlim = (jb + tile_size > ny) ? ny : jb + tile_size;
      const int ilim = (ib + tile_size > nx) ? nx : ib + tile_size;

        for (int j = jb + 1; j < jlim + 1; ++j) {
          for (int i = ib + 1; i < ilim + 1; ++i) {
	     tmp_image[j + i * height] =
		(image[j     + i       * height] * 0.6)    + \
		(image[j     + (i - 1) * height] / mNoTen) + \
		(image[j     + (i + 1) * height] / mNoTen) + \
		(image[j - 1 + i       * height] / mNoTen) + \
		(image[j + 1 + i       * height] / mNoTen);
    } 
   }
  }
 }
}

// Create the input image
void init_image(const int nx, const int ny, const int width, const int height,
                double* image, double* tmp_image)
{
  // Zero everything
  for (int j = 0; j < ny + 2; ++j) {
    for (int i = 0; i < nx + 2; ++i) {
      image[j + i * height] = 0.0;
      tmp_image[j + i * height] = 0.0;
    }
  }

  const int tile_size = 64;
  // checkerboard pattern
  for (int jb = 0; jb < ny; jb += tile_size) {
    for (int ib = 0; ib < nx; ib += tile_size) {
      if ((ib + jb) % (tile_size * 2)) {
        const int jlim = (jb + tile_size > ny) ? ny : jb + tile_size;
        const int ilim = (ib + tile_size > nx) ? nx : ib + tile_size;
        for (int j = jb + 1; j < jlim + 1; ++j) {
          for (int i = ib + 1; i < ilim + 1; ++i) {
            image[j + i * height] = 100.0;
          }
        }
      }
    }
  }
}

// Routine to output the image in Netpbm grayscale binary image format
void output_image(const char* file_name, const int nx, const int ny,
                  const int width, const int height, double* image)
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
