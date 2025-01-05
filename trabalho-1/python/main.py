import numpy as np
import subprocess   

"""
Gerando os dados para fazer análise de tempo e desempenho depois.
"""
if __name__ == "__main__":    
    hs = [0.001, 0.0005, 0.00025]
    
    compilegss = ["gcc", "-Wall", "-o", "../exec/gss", "../c/gss.c", "-O3", "-lm" ]
    compilegsrb = ["gcc", "-Wall", "-o", "../exec/gsrb", "../c/gsrb.c", "-O3", "-lm", "-fopenmp"]
    
    for i, h in enumerate(hs):
        #troca o h
        print("Rodando para h = ", h)
        with open("../inout/config.txt", 'r+') as file:
            content = file.readlines()
            content[0] = str(h) + '\n'
            file.seek(0)
            file.writelines(content)
        
        # roda 1 vez pro sequencial
        print("Gauss Seidel Sequencial")
        subprocess.run(compilegss)
        fn1 = f"../inout/data/gss/h_{i+1}.bin"
        fn2 = f"../inout/data/gss/h_{i+1}.txt"
        subprocess.run(["../exec/gss", fn1, fn2])
            
        #roda 1 vez pra cada numero de threads
        for t in [2,3,4,5,6]: # numero de threads
            print("Número de threads: ", t)
           
            subprocess.run(compilegsrb)
            fn1 = f"../inout/data/gsrb/h_{i+1}_threads_{t}.bin"
            fn2 = f"../inout/data/gsrb/h_{i+1}_threads_{t}.txt"
            subprocess.run(["../exec/gsrb", str(t), fn1, fn2])
    