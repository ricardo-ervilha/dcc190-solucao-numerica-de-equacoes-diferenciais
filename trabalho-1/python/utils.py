import matplotlib as mpl
from matplotlib.patches import Patch
import matplotlib.pyplot as plt
import seaborn as sns
import sys
import numpy as np
import scienceplots
from matplotlib.colors import ListedColormap
from matplotlib.ticker import FuncFormatter

mpl.use('Agg')

import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap
from matplotlib.patches import Patch

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

def read_matrix(filename, tamz, tamx):
    
    with open(filename, 'rb') as f:
        tissue = np.fromfile(f, dtype=np.float64).reshape((tamz, tamx))
        
    return tissue

#------------------------------------------------------------------------------------------

def plot_fixed_dir(z, filename, matrix, x_min, x_max, z_min, z_max, h, title):
    rocket = sns.color_palette('rocket', 1)
    with plt.style.context('science'):
        # Criação do domínio x e z
        x_values = np.linspace(x_min, x_max, matrix.shape[1])
        z_values = np.linspace(z_min, z_max, matrix.shape[0])
        
        # Mapeamento do z para o índice correspondente
        z_index = int((z - z_min) / h)
        
        # Garantir que o índice z esteja dentro dos limites
        if z_index < 0 or z_index >= matrix.shape[0]:
            raise ValueError(f"O valor de z={z} está fora do intervalo permitido [{z_min}, {z_max}].")
        
        # Obter a linha correspondente ao z fixado
        temp_at_z = matrix[z_index, :]
        
        # Gerar o gráfico
        plt.figure(figsize=(6, 6))
        plt.plot(x_values, temp_at_z, label=f"z = {z:.3f} m", color=rocket[0])
        
        # Configurações do gráfico
        plt.title(title)
        plt.xlabel("eixo-x (m)", fontsize=14)
        plt.ylabel("Temperatura (°C)", fontsize=14)
        plt.legend(loc="upper right", fontsize=14)
        plt.grid(True)
        plt.tight_layout()
        # Salvar o gráfico e fechar
        plt.savefig(f"../inout/{filename}.png", dpi=300)
        plt.close()


def plot_steady_state(filename, matrix, x_min, x_max, y_min, y_max, colorbar, title, tumor_mask):
    points_x = [0.02, 0.025, 0.015]
    points_z = [0.055, 0.045, 0.040]

    # Custom formatter for ticks
    def format_ticks(value, _):
        return f"{value:.2f}".rstrip("0").rstrip(".")
    
    formatter = FuncFormatter(format_ticks)

    with plt.style.context('science'):
        fig, ax = plt.subplots(figsize=(7, 7))

        # Plot the steady-state matrix
        im = ax.imshow(matrix, extent=[x_min, x_max, y_min, y_max], origin='lower', cmap='coolwarm')

        # Scatter points of injection
        ax.scatter(points_x, points_z, color='black', marker='x', s=25, label='Pontos de injeção')

        # Highlight tumor contours
        tumor_contours = ax.contour(tumor_mask, levels=[0.5], colors='black', linestyles='--',
                                    extent=[x_min, x_max, y_min, y_max])

        # Add colorbar if requested
        if colorbar:
            cbar = fig.colorbar(im, ax=ax, orientation='vertical')
            cbar.ax.tick_params(labelsize=16)
            cbar.set_label(f'Temperatura ($^\circ$C)', rotation=90, labelpad=15, fontsize=15)

        # Configure axis ticks and labels
        ax.xaxis.set_major_formatter(formatter)
        ax.yaxis.set_major_formatter(formatter)

        plt.title(f"{title}", fontsize=18)
        ax.xaxis.set_tick_params(labelsize=13)
        ax.yaxis.set_tick_params(labelsize=13)
        ax.set_xlabel('eixo-x (m)', fontsize=15)
        ax.set_ylabel('eixo-z (m)', fontsize=15)

        # Add legend
        ax.legend(
            loc="upper right",  # Adjust position
            fontsize=10,        # Reduce font size
            frameon=True,       # Add frame
            framealpha=0.7,     # Set frame transparency
            borderpad=0.5,      # Reduce internal padding
            labelspacing=0.4,   # Reduce spacing between items
        )

        plt.tight_layout()
        plt.savefig(f"../inout/{filename}.png", dpi=300)
        plt.close()

def plot_tissue(filename, matrix, x_min, x_max, y_min, y_max, colorbar, title):
    colors = [
        "#ed8479",  # Derme
        "#edd12f",  # Gordura
        "#b81f20",  # Músculo
        "#6d3f70",  # Tumor
    ]
    
    custom_cmap = ListedColormap(colors, name="Custom")
    categories = ["Derme", "Gordura", "Músculo", "Tumor"]
    
    legend_patches = [
        Patch(facecolor=color, label=category)
        for color, category in zip(colors, categories)
    ]
    
    # Custom formatter for ticks
    def format_ticks(value, _):
        return f"{value:.2f}".rstrip("0").rstrip(".")
    
    formatter = FuncFormatter(format_ticks)
    
    with plt.style.context('science'):
        fig, ax = plt.subplots(figsize=(7, 7))
        im = ax.imshow(matrix, extent=[x_min, x_max, y_min, y_max], origin='lower', cmap=custom_cmap)
        
        if colorbar:
            fig.colorbar(im, ax=ax, orientation='vertical')
        
        plt.title(f"{title}", fontsize=14)
        ax.xaxis.set_tick_params(labelsize=14)
        ax.yaxis.set_tick_params(labelsize=14)
        ax.set_xlabel("eixo-x (m)", fontsize=14)
        ax.set_ylabel("eixo-z (m)", fontsize=14)
        
        # Apply the formatter
        ax.xaxis.set_major_formatter(formatter)
        ax.yaxis.set_major_formatter(formatter)
        
        legend = ax.legend(
            handles=legend_patches,
            loc="upper right",  # Ajuste de posição
            fontsize=10,        # Reduz o tamanho da fonte
            frameon=True,       # Adiciona uma moldura
            framealpha=0.7,     # Define a transparência da moldura
            borderpad=0.5,      # Reduz o preenchimento interno
            labelspacing=0.4,   # Reduz o espaçamento entre os itens
        )
        
        plt.tight_layout()
        plt.savefig(f"../inout/{filename}.png", dpi=300)
        plt.close()