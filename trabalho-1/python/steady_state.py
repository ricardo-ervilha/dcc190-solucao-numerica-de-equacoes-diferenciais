import numpy as np
from utils import read_configs, read_matrix, plot_steady_state
import sys
 
if __name__ == "__main__":    
    simulation = sys.argv[1]
    filename = sys.argv[2] 

    configs = read_configs("../inout/config.txt")
    
    tamz = int((configs['zf'] - configs['z0']) / configs['h']) + 1
    tamx = int((configs['xf'] - configs['x0']) / configs['h']) + 1
    
    tissue = read_matrix(f"{simulation}", tamz, tamx)
    tumor_mask = np.zeros_like(tissue)
    x = np.linspace(configs['x0'], configs['xf'], tissue.shape[1])
    z = np.linspace(configs['z0'], configs['zf'], tissue.shape[0])
    x, z = np.meshgrid(x, z)
    
    xt1, zt1, yt1, rt1 = 0.02, 0.05, 0.05, 0.013
    xt2, zt2, yt2, rt2 = 0.015, 0.04, 0.04, 0.01
    y_fix = 0.045

    dist_t1 = (x - xt1)**2 + (z - zt1)**2 + (y_fix - yt1)**2
    dist_t2 = (x - xt2)**2 + (z - zt2)**2 + (y_fix - yt2)**2
    tumor_mask[(dist_t1 <= rt1**2) | (dist_t2 <= rt2**2)] = 1
    
    # print(tumor_mask)
    
    plot_steady_state(f"{filename}", tissue, configs['x0'], configs['xf'], configs['z0'], configs['zf'], True, "", tumor_mask)
    