import pandas as pd

if __name__ == "__main__":
    data_time = {}
    data_iter = {}

    # Processando os arquivos GSS
    for i in range(1, 4):
        with open(f'../inout/data/gss/h_{i}.txt') as f:
            content = f.readlines()
            time = float(content[1].strip())
            iter_count = int(content[2].strip())

        data_time[f'h_{i}'] = {"tempo_gss": time}
        data_iter[f'h_{i}'] = {"num_iteracoes": iter_count}

    # Processando os arquivos GSRB
    for i in range(1, 4):
        for t in [2, 3, 4, 5, 6]:
            with open(f'../inout/data/gsrb/h_{i}_threads_{t}.txt') as f:
                content = f.readlines()
                time = float(content[1].strip())
                iter_count = int(content[2].strip())

            data_time[f'h_{i}'][f'tempo_gsrb_com_{t}_threads'] = time
            data_iter[f'h_{i}'][f'num_iteracoes_gsrb_com_{t}_threads'] = iter_count

    # Convertendo os dicionários para dataframes
    df_time = pd.DataFrame.from_dict(data_time, orient='index')
    df_iter = pd.DataFrame.from_dict(data_iter, orient='index')

    # Salvando as tabelas em LaTeX
    with open('../inout/metrics.tex', 'w') as f:
        f.write("% Tabela de Tempos\n")
        f.write(df_time.to_latex(index=True, header=True))
        f.write("\n% Tabela de Iterações\n")
        f.write(df_iter.to_latex(index=True, header=True))

    print("Tabelas salvas em ../inout/metrics.tex")
