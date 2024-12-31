#!/bin/bash

#compilação
cd c
gcc -o ../exec/gsrb gsrb.c -O3 -lm -fopenmp

if [ $? -eq 0 ]; then
    #execução
    cd ../exec
    ./gsrb
else
    echo "Erro na compilação"
fi

cd ../python
python main.py