from matplotlib import pyplot as plt
from matplotlib.colors import ListedColormap
from matplotlib.patches import Patch
from matplotlib.ticker import FuncFormatter
from matplotlib.animation import FuncAnimation
from utils import read_matrix
import numpy as np

#format 0.00 => 0
def format_ticks(value, _):
    return f"{value:.2f}".rstrip("0").rstrip(".")

def plot_steady_state(filename, matrix, x_min, x_max, y_min, y_max):
    # changing font
    plt.rcParams["font.family"] = "serif"

    # injection points
    xs = [0.02, 0.025, 0.015]
    zs = [0.055, 0.045, 0.040]

    formatter = FuncFormatter(format_ticks)

    #------------------------------------
    fig, ax = plt.subplots(figsize=(7, 7))

    im = ax.imshow(matrix, extent=[x_min, x_max, y_min, y_max], origin='lower', cmap='coolwarm')

    ax.scatter(xs, zs, color='black', marker='x', s=25, label='Locais de Injeção')

    #colorbar
    cbar = fig.colorbar(im, ax=ax, orientation='vertical')
    cbar.ax.tick_params(labelsize=14)
    cbar.set_label(f'Temperatura ($^\circ$C)', rotation=90, labelpad=15, fontsize=14)

    #-------------------------------------
    ax.xaxis.set_major_formatter(formatter)
    ax.yaxis.set_major_formatter(formatter)

    #-------------------------------------
    ax.xaxis.set_tick_params(labelsize=14)
    ax.yaxis.set_tick_params(labelsize=14)
    ax.set_xlabel('eixo-x (m)', fontsize=14)
    ax.set_ylabel('eixo-z (m)', fontsize=14)

    #legend
    ax.legend(
        loc="upper right",
        fontsize=12,
        frameon=True,
        framealpha=0.7,
        borderpad=0.5,
        labelspacing=0.2,
    )
    
    plt.tight_layout()
    plt.savefig(f"../inout/{filename}.png", dpi=300)
    plt.close()


def plot_tissue(filename, matrix, x_min, x_max, y_min, y_max):
    # changing font
    plt.rcParams["font.family"] = "serif"

    # colors and custom legend
    colors = ["#6d3f80", "#b81f20", "#edd12f", "#ed8479"] #colors for tissue
    custom_cmap = ListedColormap(colors, name="Custom")
    categories = ["Tumor", "Músculo", "Gordura", "Derme"]
    legend_patches = [
        Patch(facecolor=color, label=category)
        for color, category in zip(colors, categories)
    ]

    #-----------------------------
    formatter = FuncFormatter(format_ticks)

    #-----------------------------

    fig, ax = plt.subplots(figsize=(7, 7))
    im = ax.imshow(matrix, extent=[x_min, x_max, y_min, y_max], origin='lower', cmap=custom_cmap)
        
    # format axis
    ax.set_xlabel("eixo-x (m)", fontsize=14)
    ax.set_ylabel("eixo-z (m)", fontsize=14)
    ax.xaxis.set_tick_params(labelsize=14)
    ax.yaxis.set_tick_params(labelsize=14)

    #Apply formatter
    ax.xaxis.set_major_formatter(formatter)
    ax.yaxis.set_major_formatter(formatter)

    #----------------------------------
    #custom Legend
    ax.legend(
        handles=legend_patches, #using the mapping
        loc="upper right",
        fontsize=12,
        frameon=True, #moldura
        framealpha=0.7, #transparencia
        borderpad=0.5, #preenchimento interno
        labelspacing=0.4, #espaçamento entre itens
    )

    plt.tight_layout()
    plt.savefig(f"../inout/{filename}.png", dpi=300)
    plt.close()


def parabolic_animation(folderbin, filename, x_min, x_max, y_min, y_max, tamz, tamx, tamt, snapshot_step):
    plt.rcParams["font.family"] = "serif"

    # injection points
    xs = [0.02, 0.025, 0.015]
    zs = [0.055, 0.045, 0.040]

    formatter = FuncFormatter(format_ticks)

    #------------------------------------
    fig, ax = plt.subplots(figsize=(7, 7))

    # Plot inicial
    snapshot0 = read_matrix(f"{folderbin}/snapshot_0.bin", tamz, tamx, type=np.float64)
    im = ax.imshow(snapshot0, extent=[x_min, x_max, y_min, y_max], origin='lower', cmap='coolwarm')

    ax.scatter(xs, zs, color='black', marker='x', s=25, label='Locais de Injeção')

    # Colorbar
    cbar = fig.colorbar(im, ax=ax, orientation='vertical')
    cbar.ax.tick_params(labelsize=14)
    cbar.set_label(f'Temperatura ($^\circ$C)', rotation=90, labelpad=15, fontsize=14)

    #-------------------------------------
    ax.xaxis.set_major_formatter(formatter)
    ax.yaxis.set_major_formatter(formatter)

    ax.xaxis.set_tick_params(labelsize=14)
    ax.yaxis.set_tick_params(labelsize=14)
    ax.set_xlabel('eixo-x (m)', fontsize=14)
    ax.set_ylabel('eixo-z (m)', fontsize=14)

    # Legenda
    ax.legend(
        loc="upper right",
        fontsize=12,
        frameon=True,
        framealpha=0.7,
        borderpad=0.5,
        labelspacing=0.2,
    )

    # Função de atualização para a animação
    def update(frame):
        print(frame * snapshot_step)
        snapshot = read_matrix(f"{folderbin}/snapshot_{frame * snapshot_step}.bin", tamz, tamx, type=np.float64)
        im.set_data(snapshot)  # Atualiza a imagem
        im.set_clim(np.min(snapshot), np.max(snapshot))
        return [im]

    # Criando a animação
    ani = FuncAnimation(fig, update, frames=tamt, interval=200, blit=True)

    # Salvando o vídeo
    ani.save(f"../inout/{filename}.mp4", writer='ffmpeg', dpi=300)

    plt.close()