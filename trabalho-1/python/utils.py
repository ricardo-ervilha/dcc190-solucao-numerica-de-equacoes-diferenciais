import matplotlib as mpl
import matplotlib.pyplot as plt
import seaborn as sns
import sys
import numpy as np
mpl.use('Agg')

def plot_graph(filename, matrix, x_min, x_max, y_min, y_max, colorbar, title):
    fig, ax = plt.subplots(figsize=(8,8))
    im = ax.imshow(matrix, extent=[x_min, x_max, y_min, y_max], origin='lower', cmap='coolwarm')
    if colorbar:
        fig.colorbar(im, ax=ax, orientation='vertical')
    plt.title(f"{title}")
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