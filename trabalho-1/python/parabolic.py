import numpy as np
from utils import read_config_txt
from plots import parabolic_animation
import sys

if __name__ == "__main__":  

    #args
    folderbin = sys.argv[1] #receive bin folder (e.g: ../parabolic/seq/h1)
    plotname = sys.argv[2] # name of the plot

    configs = read_config_txt("../inout/config.txt")
    
    tamz = int((configs['zf'] - configs['z0']) / configs['h']) + 1
    tamx = int((configs['xf'] - configs['x0']) / configs['h']) + 1
    tamt = 51
    snapshot_step = 100000

    parabolic_animation(folderbin, plotname, configs['x0'], configs['xf'], configs['z0'], configs['zf'] ,tamz, tamx, tamt, snapshot_step)
    
    