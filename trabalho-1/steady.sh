#!/bin/bash

#compilação
cd c
gcc -Wall -o ../exec/gss gss.c -O3 -lm

if [ $? -eq 0 ]; then
    cd ../exec
    ./gss ../inout/steady.bin ../inout/steady.txt
else
    echo "Erro na compilação."
fi

cd ../python
python steady_state.py ../inout/steady.bin steady_state