#!/bin/bash

#compilação
cd c
gcc -Wall -o ../exec/pennes_seq pennes_seq.c -fopenmp -Ofast -lm

if [ $? -eq 0 ]; then
    cd ../exec
    ./pennes_seq
else
    echo "Erro na compilação!"
fi

cd ../python
python parabolic.py ../inout/parabolic/seq/h1 parabolic_animation