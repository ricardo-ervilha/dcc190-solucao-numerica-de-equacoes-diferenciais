#!/bin/bash

#compilação
cd c
gcc -Wall -o ../exec/gs_steady_seq gs_steady_seq.c -Ofast -lm

if [ $? -eq 0 ]; then
    cd ../exec
    ./gs_steady_seq
else
    echo "Erro na compilação!"
fi

cd ../python
python steady_state.py ../inout/steady/steady_seq.bin steady_seq