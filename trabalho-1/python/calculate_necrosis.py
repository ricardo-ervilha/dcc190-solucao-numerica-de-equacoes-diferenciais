import numpy as np
from utils import read_config_txt, read_matrix
from plots import plot_steady_state
import sys

if __name__ == "__main__":  
    configs = read_config_txt("../inout/config.txt")
    
    tamz = int((configs['zf'] - configs['z0']) / configs['h']) + 1
    tamx = int((configs['xf'] - configs['x0']) / configs['h']) + 1
    
    steady = read_matrix(f"../inout/snapshots/snapshot_300000.bin", tamz, tamx, type=np.float64)
    tissue = read_matrix("../inout/tissue/tissue.bin", tamz, tamx, type=np.int32)

    necrosis_counts = {i: 0 for i in range(1, 5)}
    total_counts = {i: 0 for i in range(1, 5)}
    
    for j in range(tamz):
        for i in range(tamx):
            t_type = tissue[j, i]
            if t_type in total_counts:
                total_counts[t_type] += 1
                if steady[j, i] > 43:
                    necrosis_counts[t_type] += 1

    labels = {1: "tumor", 2: "muscle", 3: "fat", 4: "dermis"}
    
    for t_type, total in total_counts.items():
        if total > 0:
            print(f"{necrosis_counts[t_type]} / {total}")
            print(f"Total necrosis {labels[t_type]}: {necrosis_counts[t_type] / total * 100:.2f}%")
            print()

    print(tamz * tamx)
    print(str(necrosis_counts[2]/(total_counts[2] + total_counts[3] + total_counts[4]) * 100) + "%")