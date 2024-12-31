import numpy as np
from utils import read_configs, read_matrix, plot_tissue
   
if __name__ == "__main__":    
    tissue = read_matrix("../inout/tissue.bin")
    configs = read_configs("../inout/config.txt")
    plot_tissue("tissue", tissue, configs['x0'], configs['xf'], configs['z0'], configs['zf'], False, f"Tecido com h = {configs['h']}")
    