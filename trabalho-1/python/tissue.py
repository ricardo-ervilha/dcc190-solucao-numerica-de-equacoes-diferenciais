import numpy as np
from utils import read_matrix, read_config_txt
from plots import plot_tissue
   
if __name__ == "__main__":    
    configs = read_config_txt("../inout/config.txt") #recover config values

    #mount tamz and tamx
    tamz = int((configs['zf'] - configs['z0']) / configs['h']) + 1
    tamx = int((configs['xf'] - configs['x0']) / configs['h']) + 1
    
    #recover tissue based on tamz and tamx
    tissue = read_matrix("../inout/tissue/tissue.bin", tamz, tamx, type=np.int32)

    plot_tissue("tissue", tissue, configs['x0'], configs['xf'], configs['z0'], configs['zf'])
    