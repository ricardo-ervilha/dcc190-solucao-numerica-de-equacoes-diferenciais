import numpy as np
from utils import read_config_txt, read_matrix
from plots import plot_steady_state
import sys

if __name__ == "__main__":  

    #args
    filepathbin = sys.argv[1] #receive bin path
    plotname = sys.argv[2] # name of the plot

    configs = read_config_txt("../inout/config.txt")
    
    tamz = int((configs['zf'] - configs['z0']) / configs['h']) + 1
    tamx = int((configs['xf'] - configs['x0']) / configs['h']) + 1
    
    steady = read_matrix(f"{filepathbin}", tamz, tamx, type=np.float64)

    plot_steady_state(f"{plotname}", steady, configs['x0'], configs['xf'], configs['z0'], configs['zf'])
    