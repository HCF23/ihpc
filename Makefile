stencil: stencil.c
	icc -std=c99 -Ofast -Wall  $^ -o stencil_Ofast
	icc -pg -std=c99 -Ofast -Wall  $^ -o stencil_Ofast_prof





