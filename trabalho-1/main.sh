#!/bin/bash

#compilação
cd c
gcc main.c -o ../exe/main

if [ $? -eq 0 ]; then
    #execução
    cd ../exe
    ./main
else
    echo "Erro na compilação"
fi

#plot do gráfico
cd ../python
python main.py