#!/bin/bash

#compilação
cd c
gcc main.c -o ../exec/main -lm

if [ $? -eq 0 ]; then
    #execução
    cd ../exec
    ./main
else
    echo "Erro na compilação"
fi

cd ../python
python main.py