#!/bin/bash

#compilação
cd c
gcc -Wall -o ../exec/pennes_parabolic pennes_parabolic.c -fopenmp -Ofast -lm

if [ $? -eq 0 ]; then
    cd ../exec
    # export OMP_NUM_THREADS=32 # descomente e configure o número de threads que deseja usar.
    ./pennes_parabolic
else
    echo "Erro na compilação!"
fi

cd ../python
python parabolic.py ../inout/snapshots animation