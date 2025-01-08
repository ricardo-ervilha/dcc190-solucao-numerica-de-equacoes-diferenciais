import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import scienceplots

def save_as_latex(df_time, df_iter):
    with open('../inout/metrics.tex', 'w') as f:
        f.write("% Tabela de Tempos\n")
        f.write(df_time.to_latex(index=True, header=True))
        f.write("\n% Tabela de Iterações\n")
        f.write(df_iter.to_latex(index=True, header=True))

def plot_bar_graph(data_time):
    bar_data = []
    for i in range(1, 4):
        # gss
        bar_data.append({
            "threads": 1,
            "discretizacao": f'h_{i}',
            "tempo": data_time[f'h_{i}']["tempo_gss"]
        })
        # gsrb
        for t in [2, 3, 4, 5, 6]:
            bar_data.append({
                "threads": t,
                "discretizacao": f'h_{i}',
                "tempo": data_time[f'h_{i}'][f'tempo_gsrb_com_{t}_threads']
            })

    bar_df = pd.DataFrame(bar_data)
    rocket = sns.color_palette('rocket', 6)
    for i in range(1, 4):
        with plt.style.context('science'):
            discretizacao_df = bar_df[bar_df["discretizacao"] == f'h_{i}']
            plt.figure(figsize=(8, 5))
            sns.barplot(
                data=discretizacao_df, 
                x="threads", 
                y="tempo", 
                palette=rocket, errorbar=None,
                edgecolor='black'
            )
            plt.xlabel("Número de Threads", fontsize=14)
            plt.ylabel("Tempo (s)", fontsize=14)
            plt.tight_layout()

            plt.savefig(f"../inout/tempo_por_threads_h_{i}.png")
            plt.show()

def plot_speedup(data_time):
    rocket = sns.color_palette('rocket', 3)
    disc = {
        'h_1': '0.001',
        'h_2': '0.0005',
        'h_3': '0.00025'
    }
    with plt.style.context('science'):
        plt.figure(figsize=(10, 6))
        markers = {"h_1": "o", "h_2": "^", "h_3": "s"}
        colors = ["#fee090", "#fdae61", "#4575b4"]  # Lista de cores para cada discretização

        for i in range(1, 4):
            discretizacao = f'h_{i}'
            speedup_data = []
            for t in [1, 2, 3, 4, 5, 6]:
                if t == 1:
                    speedup = 1.0  # Speedup 1 para gss
                else:
                    speedup = data_time[discretizacao]["tempo_gss"] / data_time[discretizacao][f'tempo_gsrb_com_{t}_threads']
                speedup_data.append({
                    "threads": t,
                    "speedup": speedup,
                    "discretizacao": discretizacao
                })

            speedup_df = pd.DataFrame(speedup_data)
            plt.plot(
                speedup_df["threads"], 
                speedup_df["speedup"], 
                marker=markers[discretizacao], 
                label=f"$h = {disc[discretizacao]}$",
                color=rocket[i-1],  # Atribuindo cor da lista para cada discretização,
            )
        plt.plot(
                speedup_df["threads"], 
                pd.Series([1,2,3,4,5,6]), 
                label=f"Ideal",
                color='black',  # Atribuindo cor da lista para cada discretização,
                linestyle='dashed',
        )
        plt.xlabel("Número de Threads", fontsize=14)
        plt.ylabel("Speedup", fontsize=14)
        plt.legend(
            loc="upper left",  # Ajuste de posição
            fontsize=12,        # Reduz o tamanho da fonte
            frameon=True,       # Adiciona uma moldura
            # framealpha=0.7,     # Define a transparência da moldura
            borderpad=0.5,      # Reduz o preenchimento interno
            labelspacing=0.4,
        )
        plt.grid(True)
        plt.tight_layout()

        # Salvando o gráfico de speedup
        plt.savefig("../inout/speedup_por_threads.png")
        plt.show()


if __name__ == "__main__":
    data_time = {}
    data_iter = {}

    #read gss values
    for i in range(1, 4):
        with open(f'../inout/data/gss/h_{i}.txt') as f:
            content = f.readlines()
            time = float(content[1].strip())
            iter_count = int(content[2].strip())

        data_time[f'h_{i}'] = {"tempo_gss": time}
        data_iter[f'h_{i}'] = {"num_iteracoes": iter_count}

    #read gsrb values
    for i in range(1, 4):
        for t in [2, 3, 4, 5, 6]:
            with open(f'../inout/data/gsrb/h_{i}_threads_{t}.txt') as f:
                content = f.readlines()
                time = float(content[1].strip())
                iter_count = int(content[2].strip())

            data_time[f'h_{i}'][f'tempo_gsrb_com_{t}_threads'] = time
            data_iter[f'h_{i}'][f'num_iteracoes_gsrb_com_{t}_threads'] = iter_count

    # dataframes
    df_time = pd.DataFrame.from_dict(data_time, orient='index')
    df_iter = pd.DataFrame.from_dict(data_iter, orient='index')

    plot_bar_graph(data_time)
    plot_speedup(data_time)
