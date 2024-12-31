import numpy as np
from utils import read_configs, read_matrix, plot_steady_state
   
if __name__ == "__main__":    
    steady_state = read_matrix("../inout/steady_state.bin")
    configs = read_configs("../inout/config.txt")
    plot_steady_state("steady_state", steady_state, configs['x0'], configs['xf'], configs['z0'], configs['zf'], True, f"h = {configs['h']} e tol = {configs['tol']}")
    