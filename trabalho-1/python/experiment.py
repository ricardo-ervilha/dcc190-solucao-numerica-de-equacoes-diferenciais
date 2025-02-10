import os
import numpy as np
import subprocess

if __name__ == "__main__":
    hs = [0.001, 0.0005, 0.00025]
    
    threads = [1, 2, 4, 8, 12, 16, 20, 24, 28, 32] # RODAR NO LIGHTNING
    # threads = [1, 2, 3, 4, 5, 6] # rodando no meu desktop

    compile = ["gcc", "-Wall", "-o", "../exec/pennes_parabolic", "../c/pennes_parabolic.c", "-Ofast", "-lm", "-fopenmp"]

    for h in hs:
        print("# h = : ", h)
        
        # Sobrescreve arquivo config.txt para alterar o valor de h nele
        with open("../inout/config.txt", "r+") as file:
            content = file.readlines()
            content[0] = str(h) + '\n'
            file.seek(0)
            file.writelines(content)

        for thread in threads:
            print("   # Thread = ", thread)
            
            # Define variável de ambiente para o número de threads
            env = os.environ.copy()
            env["OMP_NUM_THREADS"] = str(thread)

            # Compila e executa
            subprocess.run(compile, env=env)
            subprocess.run(["../exec/pennes_parabolic"], env=env)
