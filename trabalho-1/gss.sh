#!/bin/bash

#compilação
cd c
gcc gss.c -o ../exec/gss -lm

if [ $? -eq 0 ]; then
    #execução
    cd ../gss
    ./main
else
    echo "Erro na compilação"
fi

cd ../python
python main.py