import matplotlib as mpl
from matplotlib.patches import Patch
import matplotlib.pyplot as plt
import seaborn as sns
import sys
import numpy as np
import scienceplots
from matplotlib.colors import ListedColormap

mpl.use('Agg')

import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap
from matplotlib.patches import Patch

def plot_steady_state(filename, matrix, x_min, x_max, y_min, y_max, colorbar, title):
    with plt.style.context('science'):
        fig, ax = plt.subplots(figsize=(7, 7))
        im = ax.imshow(matrix, extent=[x_min, x_max, y_min, y_max], origin='lower', cmap='viridis')
        
        if colorbar:
            cbar = fig.colorbar(im, ax=ax, orientation='vertical')
            cbar.ax.tick_params(labelsize=16)
        
        plt.title(f"{title}", fontsize=18)
        ax.xaxis.set_tick_params(labelsize=13)
        ax.yaxis.set_tick_params(labelsize=13)
        ax.set_xlabel('x-axis (m)', fontsize=15)
        ax.set_ylabel('z-axis (m)', fontsize=15)
        plt.tight_layout()
        plt.savefig(f"../inout/{filename}.png", dpi=300)
        plt.close()


def plot_tissue(filename, matrix, x_min, x_max, y_min, y_max, colorbar, title):
    colors = [
        "#E2B8B4",  # Derme
        "#FFDE21",  # Gordura
        "#c61a1b",  # Músculo
        "#5A315D",  # Tumor
    ]
    
    custom_cmap = ListedColormap(colors, name="Custom")
    categories = ["Derme", "Gordura", "Músculo", "Tumor"]
    
    legend_patches = [
        Patch(facecolor=color, label=category)
        for color, category in zip(colors, categories)
    ]
    
    with plt.style.context('science'):
        fig, ax = plt.subplots(figsize=(7, 7))
        im = ax.imshow(matrix, extent=[x_min, x_max, y_min, y_max], origin='lower', cmap=custom_cmap)
        
        if colorbar:
            fig.colorbar(im, ax=ax, orientation='vertical')
        
        plt.title(f"{title}", fontsize=18)
        ax.xaxis.set_tick_params(labelsize=13)
        ax.yaxis.set_tick_params(labelsize=13)
        
        legend = ax.legend(
            handles=legend_patches,
            loc="center left",
            bbox_to_anchor=(1.05, 0.5),
            fontsize=14,
        )
        
        plt.tight_layout()
        plt.savefig(f"../inout/{filename}.png", dpi=300)
        plt.close()


def read_configs(filename):
    data = {}
    
    with open(filename, 'r') as file:
        data['h'] = float(file.readline().strip())
        data['y_fix'] = float(file.readline().strip())
        
        data['muscle_thick'], data['fat_thick'], data['dermis_thick'] = map(
            float, file.readline().split()
        )
        
        coords = list(map(float, file.readline().split()))
        data['xt1'], data['yt1'], data['zt1'], data['rt1'], data['xt2'], data['yt2'], data['zt2'], data['rt2'] = coords
        
        data['x0'], data['xf'], data['z0'], data['zf'] = map(
            float, file.readline().split()
        )
        
        data['T_a'] = float(file.readline().strip())
        data['T_thresh1'], data['T_thresh2'] = map(
            float, file.readline().split()
        )
        
        data['tol'] = float(file.readline().strip())
        data['max_iter'] = int(file.readline().strip())
    
    return data

def read_matrix(filename):
    with open(filename, 'rb') as f:
        tamz = np.fromfile(f, dtype=np.int32, count=1)[0]
        tamx = np.fromfile(f, dtype=np.int32, count=1)[0]
        
        tissue = np.fromfile(f, dtype=np.float64).reshape((tamz, tamx))
        
    return tissue