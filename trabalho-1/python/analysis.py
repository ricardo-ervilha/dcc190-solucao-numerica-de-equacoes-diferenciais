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
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

def plot_speedup(data_time):
    # Paleta de cores
    rocket = sns.color_palette('rocket', 3)
    disc = {
        'h_1': '0.001',
        'h_2': '0.0005',
        'h_3': '0.00025'
    }
    
    with plt.style.context('science'):
        plt.figure(figsize=(10, 6))
        
        # Estilos de linha para discretizações
        linestyles = {"h_1": "solid", "h_2": "dashed", "h_3": "dotted"}
        
        # Iterar sobre as discretizações
        for i in range(1, 4):
            discretizacao = f'h_{i}'
            speedup_data = []
            
            # Calcular o speedup para diferentes números de threads
            for t in range(1, 7):
                speedup = (
                    1.0
                    if t == 1
                    else data_time[discretizacao]["tempo_gss"]
                    / data_time[discretizacao][f'tempo_gsrb_com_{t}_threads']
                )
                speedup_data.append({"threads": t, "speedup": speedup})
            
            # Criar DataFrame com os dados de speedup
            speedup_df = pd.DataFrame(speedup_data)
            
            # Plotar o speedup medido
            plt.plot(
                speedup_df["threads"],
                speedup_df["speedup"],
                label=f"$h = {disc[discretizacao]}$",
                color=rocket[i - 1],
                marker='x',
                linewidth=2,
            )
        
        # Plotar o speedup ideal
        plt.plot(
            [1, 2, 3, 4, 5, 6],
            [1, 2, 3, 4, 5, 6],
            color="black",
            linestyle="dashed",
            linewidth=2,
        )
        
        # Adicionar texto "linear (ideal)" com inclinação da linha identidade
        plt.text(
            4.5, 4.6,  # Posição do texto
            "linear (ideal)",
            fontsize=16,
            color="black",
            rotation=30,  # Inclinação da linha identidade (y=x)
            rotation_mode="anchor",
        )
        
        # Configurar o gráfico
        plt.xlabel("Num. Threads", fontsize=14)
        plt.ylabel("Speedup", fontsize=14)
        # plt.title("Speedup vs Número de Processadores", fontsize=16)
        plt.legend(
            loc="upper left",
            fontsize=14,
            frameon=True,
            borderpad=0.8,
            labelspacing=0.6,
        )
        plt.grid(True, linestyle="--")
        plt.tight_layout()
        
        # Salvar e mostrar o gráfico
        plt.savefig("../inout/speedup_por_threads.png", dpi=300)
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

    # plot_bar_graph(data_time)
    plot_speedup(data_time)
