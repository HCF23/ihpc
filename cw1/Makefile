stencil: stencil.c
	icc -std=c99 -fast -Wall -qopt-report-phase=vec,loop -qopt-report=5 -qopt-report-file=compile.out -restrict -xCORE-AVX2 $^ -o stencil_vec_report
	icc -g -std=c99 -fast -Wall  -xSSE4.2 -axAVX,CORE-AVX2 $^ -o stencil_dbg
	icc -S -fverbose-asm -std=c99 -Wall -xSSE4.1 -axAVX,CORE-AVX2 -fast -fno-fnalias -falign-loops=32 -m64 -no-inline-max-size $^ -o stencil_asm
	icc -std=c99 -Wall -xSSE4.1 -axAVX,CORE-AVX2 -fast -fno-fnalias -falign-loops=64 -m64 -no-inline-max-size -simd $^ -o stencil
