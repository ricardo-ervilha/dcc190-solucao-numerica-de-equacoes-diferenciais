import numpy as np
from utils import read_configs, read_matrix, plot_steady_state
import subprocess   
   
if __name__ == "__main__":    
    hs = [0.00025]
    
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
        
        #roda 5 vezes pro sequencial
        # print("Gauss Seidel Sequencial")
        # for j in range(0, 5):
        #     print("Iteração: ", j)
        #     subprocess.run(compilegss)
        #     fn1 = f"../inout/gss_h{i+1}_iter{j+1}.bin"
        #     fn2 = f"../inout/gss_h{i+1}_iter{j+1}.txt"
        #     subprocess.run(["../exec/gss", fn1, fn2])
            
        #roda 5 vezes pro paralelo com um certo numero de threads
        print("Gauss Seidel Vermelho-Preto")
        for t in [2]: # numero de threads
            print("Número de threads: ", t)
            for j in range(0, 5):
                print("Iteração: ", j)
                subprocess.run(compilegsrb)
                fn1 = f"../inout/gsrb_h{i+1}_iter{j+1}_threads{t}.bin"
                fn2 = f"../inout/gsrb_h{i+1}_iter{j+1}_threads{t}.txt"
                subprocess.run(["../exec/gsrb", fn1, fn2, str(t)])
                
    
    # configs = read_configs("../inout/config.txt")
    
    # tamz = int((configs['zf'] - configs['z0']) / configs['h']) + 1
    # tamx = int((configs['xf'] - configs['x0']) / configs['h']) + 1
    
    # steady_state = read_matrix("../inout/steady_state.bin", tamz, tamx)
    
    # plot_steady_state("steady_state", steady_state, configs['x0'], configs['xf'], configs['z0'], configs['zf'], True, f"h = {configs['h']} e tol = {configs['tol']}")
    