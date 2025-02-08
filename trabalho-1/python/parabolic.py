import numpy as np
from utils import read_config_txt
from plots import parabolic_animation
import sys

if __name__ == "__main__":  

    #args
    folderbin = sys.argv[1] #receive bin folder (e.g: ../parabolic/seq/h1)
    plotname = sys.argv[2] # name of the plot

    configs = read_config_txt("../inout/config.txt")
    
    ht = (configs['h'] ** 2) / 2.2
    tamz = int((configs['zf'] - configs['z0']) / configs['h']) + 1
    tamx = int((configs['xf'] - configs['x0']) / configs['h']) + 1
    tamt = int((configs['tf']  - 0) / ht) + 1
    snapshot_step = configs['snapshot_interval']

    parabolic_animation(folderbin, plotname, configs['x0'], configs['xf'], configs['z0'], configs['zf'] ,tamz, tamx, tamt, snapshot_step)
    
    