# DCC190 - Solução Numérica de Equações Diferenciais - 2024.3

Repositório contendo o trabalho 1 da disciplina.

## Instruções para rodar o código:

1. Clone o repositório:

    ```bash
    git clone https://github.com/ricardo-ervilha/dcc190-solucao-numerica-de-equacoes-diferenciais.git
    ```

2. Navegue até o diretório do trabalho 1:

    ```bash
    cd dcc190-solucao-numerica-de-equacoes-diferenciais
    cd trabalho-1
    ```

3. Instale as dependências do python:

    ```bash
    pip install -r requirements.txt
    ```

4. Altere as permissões dos scripts para executá-los:

    ```bash
    chmod +x steady.sh && chmod +x tissue.sh
    ```

5. Execute os scripts para gerar os gráficos:

    - Para plotar o gráfico do tecido:
    
        ```bash
        ./tissue.sh
        ```

        Saída: `inout/tissue.png`

    - Para plotar a solução estacionária:
    
        ```bash
        ./steady.sh
        ```

        Saída: `inout/steady_state.png`

## Configurações

Para alterar as configurações, edite o arquivo `../inout/config.txt`.

### Estrutura do arquivo de configurações:

- `h`: Discretização
- `y_fix`: y utilizado para cortar as esferas
- `muscle_thick fat_thick dermis_thick`: Grossura dos tecidos
- `xt1 yt1 zt1 rt1 xt2 yt2 zt2 rt2`: Centro e raio das esferas
- `x0 xf z0 zf`: Intervalo do domínio
- `T_a`: Temperatura arterial
- `T_thresh1 T_thresh2`: Temperaturas limiares do omega
- `tol`: Tolerância
- `max_iter`: Máximo de iterações
