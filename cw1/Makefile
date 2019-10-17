stencil: stencil.c
	icc -std=c99 -fast -Wall -fp-model-fast=2 -qopt-report-phase=vec -qopt-report=5 -qopt-report-file=stdout -restrict $^ -o stencil_vec_reports
	icc -g -std=c99 -fast -Wall  $^ -o stencil_dbg
	icc -std=c99 -fast -Wall -fp-model-fast=2  $^ -o stencil




