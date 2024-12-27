#!/bin/bash

#compilação
cd c
gcc exercise.c -o ../exe/exercise -lm

if [ $? -eq 0 ]; then
    #execução
    cd ../exe
    ./exercise
else
    echo "Erro na compilação"
fi

#plot do gráfico
cd ../python
python main.py