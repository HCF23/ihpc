#!/bin/bash

ARGS="--nodes 1 --ntasks-per-node 1 --partition veryshort --reservation COMS30005 --account COMS30005 --job-name 0w0 --time 00:00:15 --exclusive"

PY_CHECK="check.py --stencil-file stencil.pgm --ref-stencil-file "

ref1024="stencil_1024_1024_100.pgm"
ref4096="stencil_4096_4096_100.pgm"
ref8000="stencil_8000_8000_100.pgm"

STENCIL=stencil

echo "$STENCIL" > >(tee -a bc_out.log)
srun $ARGS ./$STENCIL 1024 1024 100 > >(tee -a bc_out.log)
python $PY_CHECK $ref1024 > >(tee -a bc_out.log)
#srun $ARGS ./$STENCIL 4096 4096 100 > >(tee -a bc_out.log)
#python $PY_CHECK $ref4096 > >(tee -a bc_out.log)
#srun $ARGS ./$STENCIL 8000 8000 100 > >(tee -a bc_out.log)
#python $PY_CHECK $ref8000 > >(tee -a bc_out.log)
#icc -fast -fno-fnalias -falign-loops=64 -qopt-dynamic-align -simd -guide-vec=4 -parallel -fopenmp-simd stencil.c -o stencil  
