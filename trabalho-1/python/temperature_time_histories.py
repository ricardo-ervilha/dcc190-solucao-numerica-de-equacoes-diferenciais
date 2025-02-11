from utils import read_config_txt, read_matrix
from plots import format_ticks
import sys
import numpy as np

from matplotlib import pyplot as plt
from matplotlib.ticker import FuncFormatter

import matplotlib.cm as cm

def plot_temperature_time_histories(filename, tumor, healthy, time):
    # changing font
    plt.rcParams["font.family"] = "serif"

    # colors and custom legend
    

    #-----------------------------
    formatter = FuncFormatter(format_ticks)

    #-----------------------------

    fig, ax = plt.subplots(figsize=(10, 7))
    ax.plot(time, tumor, color=cm.Set1(0), label="$(x = 0.025, z = 0.05) \in$ Tumor")
    ax.plot(time, healthy, color=cm.Set1(1), label="$(x = 0.025, z = 0.065) \in$ Músculo")
        
    # format axis
    ax.set_xlabel("Tempo (s)", fontsize=14)
    ax.set_ylabel("Temperatura ($^\circ C$)", fontsize=14)
    ax.xaxis.set_tick_params(labelsize=14)
    ax.yaxis.set_tick_params(labelsize=14)

    #Apply formatter
    ax.xaxis.set_major_formatter(formatter)
    ax.yaxis.set_major_formatter(formatter)

    ax.set_xlim(left=0)

    #----------------------------------
    #custom Legend
    ax.legend(
        loc="upper left",
        fontsize=12,
        frameon=True, #moldura
        framealpha=0.7, #transparencia
        borderpad=0.5, #preenchimento interno
        labelspacing=0.4, #espaçamento entre itens
    )

    ax.grid(True, which='both', linestyle='--', color='gray', alpha=0.5)
    plt.tight_layout()
    plt.savefig(f"../inout/{filename}.png", dpi=300)
    plt.close()

if __name__ == "__main__":  

    #args
    folderbin = sys.argv[1] #receive bin folder (e.g: ../parabolic/seq/h1)
    plotname = sys.argv[2] # name of the plot

    configs = read_config_txt("../inout/config.txt")
    
    ht = 0.01
    tamz = int((configs['zf'] - configs['z0']) / configs['h']) + 1
    tamx = int((configs['xf'] - configs['x0']) / configs['h']) + 1
    snapshot_step = configs['snapshot_interval']
    tamt = (int((configs['tf']) / ht) + 1) // snapshot_step 

    xt = int((0.025 - configs['x0']) / configs['h'])
    zt = int((0.05 - configs['z0']) / configs['h'])

    xh = int((0.025 - configs['x0']) / configs['h'])
    zh = int((0.065 - configs['z0']) / configs['h'])

    list_tumor = []
    list_healthy = []

    for i in range(tamt):
        snapshot = read_matrix(f"{folderbin}/snapshot_{i * snapshot_step}.bin", tamz, tamx, type=np.float64)
        list_tumor.append(snapshot[zt][xt])
        list_healthy.append(snapshot[zh][xh])

    times = np.arange(0, 3000, 10)
    # print(times)
    plot_temperature_time_histories(plotname, list_tumor, list_healthy, times)