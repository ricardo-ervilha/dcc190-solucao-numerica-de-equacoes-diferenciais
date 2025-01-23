#!/bin/bash

#compilação
cd c
gcc -Wall -o ../exec/gs_steady_par gs_steady_par.c -Ofast -lm -fopenmp

if [ $? -eq 0 ]; then
    cd ../exec
    ./gs_steady_par
else
    echo "Erro na compilação!"
fi

cd ../python
python steady_state.py ../inout/steady/steady_par.bin steady_par