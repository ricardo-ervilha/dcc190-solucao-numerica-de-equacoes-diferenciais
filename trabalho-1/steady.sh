#!/bin/bash

#compilação
cd c
gcc -Wall -o ../exec/gsrb gsrb.c -O3 -lm -fopenmp

if [ $? -eq 0 ]; then
    cd ../exec
    ./gsrb 6 ../inout/steady.bin ../inout/steady.txt
else
    echo "Erro na compilação."
fi

cd ../python
python steady_state.py ../inout/steady.bin steady_final_gsrb