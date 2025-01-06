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
    
    plot_steady_state(f"{filename}", tissue, configs['x0'], configs['xf'], configs['z0'], configs['zf'], True, "")
    