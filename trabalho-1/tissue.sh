#!/bin/bash

#compilação
cd c
gcc tissue.c -o ../exec/tissue -lm

if [ $? -eq 0 ]; then
    cd ../exec
    ./tissue
else
    echo "Erro na compilação."
fi

cd ../python
python tissue.py