stencil: stencil.c
	icc -std=c99 -fast -Wall -qopt-report-phase=vec,loop,cg -qopt-report=5 -qopt-report-file=stdout -restrict $^ -o stencil_vec_reports
	icc -g -std=c99 -fast -Wall  $^ -o stencil_dbg
	icc -std=c99 -fast -Wall  $^ -o stencil





