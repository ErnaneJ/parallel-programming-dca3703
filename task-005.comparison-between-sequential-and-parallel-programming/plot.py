import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

sns.set(style="whitegrid")
plt.rcParams["figure.figsize"] = (10, 5)

df = pd.read_csv("./out/results.csv")

# Gráfico - Versão Sequencial - Tempo vs n
df_seq = df[(df["version"] == "sequential") & (df["threads"] == 2)] 

plt.figure()
sns.lineplot(data=df_seq, x="n", y="time_seconds", marker="o", color="steelblue")

plt.title("Versão Sequencial - Tempo vs n")
plt.xlabel("Valor de n")
plt.ylabel("Tempo (segundos)")
plt.xscale("log")
plt.yscale("log")
plt.tight_layout()
plt.savefig("./out/grafico_sequencial.png")
plt.show()

# Gráfico - Versão Paralela com Reduction - Tempo vs n
df_par = df[df["version"] == "parallel_reduction"]

plt.figure()
sns.lineplot(data=df_par, x="n", y="time_seconds", hue="threads", marker="o", palette="Set2")

plt.title("Versão Paralela com Reduction - Tempo vs n")
plt.xlabel("Valor de n")
plt.ylabel("Tempo (segundos)")
plt.xscale("log")
plt.yscale("log")
plt.legend(title="Threads")
plt.tight_layout()
plt.savefig("./out/grafico_paralela_reduction_threads.png")
plt.show()

# Gráfico - Comparação: Tempo Sequencial vs Paralelo (com Reduction)
df_comp = df[df["version"].isin(["sequential", "parallel_reduction"])]

plt.figure(figsize=(12, 6))
sns.barplot(
    data=df_comp,
    x="n",
    y="time_seconds",
    hue="threads",
    palette="magma"
)

plt.title("Comparação: Tempo Sequencial vs Paralelo (com Reduction)")
plt.xlabel("n")
plt.ylabel("Tempo (s)")
plt.yscale("log")
plt.legend(title="Threads", bbox_to_anchor=(1.05, 1), loc='upper left')
plt.tight_layout()
plt.savefig("./out/grafico2_comparacao_barras.png")
plt.show()

# Gráfico - Tempo de Execução: Sequencial vs Paralelo (8 threads)
df_filtered = df[
    ((df["version"] == "sequential") & (df["threads"] == 1)) |
    ((df["version"] == "parallel_reduction") & (df["threads"] == 8))
]

df_filtered["label"] = df_filtered.apply(
    lambda row: "Sequencial" if row["version"] == "sequential" else "Paralelo (8 threads)",
    axis=1
)

df_filtered = df_filtered.sort_values(by="n")

plt.figure(figsize=(12, 6))
sns.barplot(
    data=df_filtered,
    x="n",
    y="time_seconds",
    hue="label",
    palette=["gray", "steelblue"]
)
plt.title("Tempo de Execução: Sequencial vs Paralelo (8 threads)")
plt.xlabel("n")
plt.ylabel("Tempo (s)")
plt.yscale("log")
plt.legend(title="Versão")
plt.tight_layout()
plt.savefig("./out/grafico3_seq_vs_parallel8.png")
plt.show()