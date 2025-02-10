import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.cm as cm

"""Generates Latex Table and Some Useful Plots based on Experiments Results"""

def plot_speedup(speedups, hs, threads):
    plt.rcParams["font.family"] = "serif"
    fig, ax = plt.subplots(figsize=(7, 7))

    for i, h in enumerate(hs):
        plt.plot(threads[1:],  speedups.loc[h][1:], marker='o', color=cm.Set1(i), label=f'$h = {h}$')

    plt.plot(threads[1:], threads[1:], linestyle="dashed", color=cm.Set1(len(hs)), label="Speedup Ideal")

    ax.xaxis.set_tick_params(labelsize=14)
    ax.yaxis.set_tick_params(labelsize=14)
    ax.set_xticks(threads[1:])
    ax.set_xlabel('Número de Threads', fontsize=14)
    ax.set_ylabel('Speedup', fontsize=14)

    ax.legend(
        loc="upper right",
        fontsize=12,
        frameon=True,
        framealpha=0.7,
        borderpad=0.5,
        labelspacing=0.2,
    )

    ax.grid(True, which='both', linestyle='--', color='gray', alpha=0.5)
    plt.tight_layout()
    plt.savefig(f"../inout/speedup.png", dpi=300)
    plt.close()

if __name__ == "__main__":
    hs = [0.001, 0.0005, 0.00025]

    threads = [1, 2, 4, 8, 12, 16, 20, 24, 28, 32] # RODAR NO LIGHTNING
    # threads = [1, 2, 3, 4, 5, 6] # rodando no meu desktop

    times = {} # array to retriever times

    for h in hs:

        times[h] = {} #Initializes intern dict

        for thread in threads:
            with open(f"../inout/parabolic/{thread}/{h}.dat") as file:
                content = file.readlines()
                time = float(content[0].strip())

            times[h][thread] = time

    df = pd.DataFrame.from_dict(times, orient='index') 
    
    # Calculates speed up
    speedups = df.copy(deep=True)
    for h in hs:
        for thread in threads:
            speedups.loc[h, thread] = df.loc[h, 1] / df.loc[h, thread]

    plot_speedup(speedups, hs, threads)