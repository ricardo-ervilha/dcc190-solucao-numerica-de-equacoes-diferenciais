#!/bin/bash

#compilação
cd c
gcc -Wall -o ../exec/jacobi_pennes_seq jacobi_pennes_seq.c -O3 -lm

if [ $? -eq 0 ]; then
    cd ../exec
    ./jacobi_pennes_seq
else
    echo "Erro na compilação!"
fi

# cd ../python
# python parabolic.py .../parabolic/seq/h1 parabolic_animation