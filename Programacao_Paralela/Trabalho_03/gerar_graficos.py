import os
import matplotlib.pyplot as plt
import pandas as pd

# Nome do arquivo gerado pelo seu programa MPI
nome_arquivo_csv = "resultados_jacobi.csv"

# Verificar se o arquivo realmente existe antes de tentar ler
if not os.path.exists(nome_arquivo_csv):
    print(f"❌ Erro: O arquivo '{nome_arquivo_csv}' não foi encontrado!")
    print("Execute 'make run-all' primeiro para gerar os dados do teste.")
    exit(1)

# 1. Carregar os dados dinamicamente a partir do arquivo CSV
df = pd.read_csv(nome_arquivo_csv)

# Remover espaços extras que possam existir nos nomes das colunas
df.columns = df.columns.str.strip()

# Extrair os tamanhos únicos de N e ordenar
tamanhos_N = sorted(df["N"].unique())

print(f"📊 Arquivo '{nome_arquivo_csv}' carregado com sucesso!")
print(f"Tamanhos de N encontrados: {tamanhos_N}")
print(f"Processos encontrados: {sorted(df['QuantidadeProcessos'].unique())}\n")

# --- GRÁFICO 1: TEMPO DE EXECUÇÃO POR TAMANHO DA MATRIZ ---
plt.figure(figsize=(10, 6))

# Como o tempo sequencial varia ligeiramente entre os testes, calculamos a média dele por N
df_seq_medio = df.groupby("N")["TempoSequencial"].mean().reindex(tamanhos_N)
plt.plot(
    tamanhos_N,
    df_seq_medio.values,
    marker="o",
    linewidth=2,
    label="Sequencial (Média)",
)

# Plotar uma linha para cada configuração de Processos do MPI encontrada no CSV
processos_encontrados = sorted(df["QuantidadeProcessos"].unique())
for np_proc in processos_encontrados:
    df_proc = df[df["QuantidadeProcessos"] == np_proc].set_index("N").reindex(tamanhos_N)
    plt.plot(
        tamanhos_N,
        df_proc["TempoMPI"].values,
        marker="s",
        linestyle="--",
        linewidth=2,
        label=f"MPI ({np_proc} proc)",
    )

plt.title("Tempo de Execução vs Tamanho da Matriz ($N$)", fontsize=14, pad=15)
plt.xlabel("Tamanho da Matriz ($N$)", fontsize=12)
plt.ylabel("Tempo de Execução (segundos)", fontsize=12)
plt.xticks(tamanhos_N)
plt.grid(True, linestyle=":", alpha=0.6)
plt.legend(fontsize=11)
plt.tight_layout()

# Salva o gráfico em formato PNG de alta resolução
nome_grafico_tempo = "grafico_tempo_execucao.png"
plt.savefig(nome_grafico_tempo, dpi=300)
print(f"💾 Gráfico salvo: {nome_grafico_tempo}")
plt.show()


# --- GRÁFICO 2: SPEEDUP POR QUANTIDADE DE PROCESSOS ---
plt.figure(figsize=(10, 6))

# Linha de Speedup Ideal (Linear) baseada nos processos reais que você rodou
plt.plot(
    processos_encontrados,
    processos_encontrados,
    color="black",
    linestyle=":",
    linewidth=2,
    label="Speedup Ideal (Linear)",
)

# Plotar a curva de speedup obtida para cada tamanho N
for n_tamanho in tamanhos_N:
    df_n = df[df["N"] == n_tamanho].set_index("QuantidadeProcessos").reindex(processos_encontrados)
    plt.plot(
        processos_encontrados,
        df_n["Speedup"].values,
        marker="^",
        linewidth=2,
        label=f"N = {n_tamanho}",
    )

plt.title("Speedup obtido vs Quantidade de Processos", fontsize=14, pad=15)
plt.xlabel("Quantidade de Processos (MPI)", fontsize=12)
plt.ylabel("Speedup", fontsize=12)
plt.xticks(processos_encontrados)
plt.grid(True, linestyle=":", alpha=0.6)
plt.legend(fontsize=11)
plt.tight_layout()

# Salva o gráfico em formato PNG de alta resolução
nome_grafico_speedup = "grafico_speedup.png"
plt.savefig(nome_grafico_speedup, dpi=300)
print(f"💾 Gráfico salvo: {nome_grafico_speedup}")
plt.show()
