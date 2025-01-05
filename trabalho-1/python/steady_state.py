import numpy as np
from utils import read_configs, read_matrix, plot_steady_state
   
if __name__ == "__main__":    
    configs = read_configs("../inout/config.txt")
    
    tamz = int((configs['zf'] - configs['z0']) / configs['h']) + 1
    tamx = int((configs['xf'] - configs['x0']) / configs['h']) + 1
    
    tissue = read_matrix("../inout/data/gss/h_2.bin", tamz, tamx)
    
    plot_steady_state("gss_steady_h2", tissue, configs['x0'], configs['xf'], configs['z0'], configs['zf'], False, "")
    