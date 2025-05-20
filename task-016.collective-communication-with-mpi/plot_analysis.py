import pandas as pd
import matplotlib.pyplot as plt
import os

df = pd.read_csv(f"{os.path.dirname(__file__)}/result.csv", sep=",", header=0)
df["Size"] = df["Size"].str.extract(r"(\d+)").astype(int)  # converte '512 x 512' para 512
df.rename(columns={"Number of Process": "np", "Size": "size", "Execution": "execucao", "Time (s)": "tempo"}, inplace=True)

# Calcular média por (np, size)
grouped = df.groupby(["np", "size"])["tempo"].agg(["mean", "std"]).reset_index()

# Gráfico 1: Tempo médio vs. tamanho da matriz para diferentes números de processos
plt.figure(figsize=(10, 6))
for np in sorted(grouped["np"].unique()):
    subset = grouped[grouped["np"] == np]
    plt.plot(subset["size"], subset["mean"], marker="o", label=f"{np} processos")
plt.xlabel("Tamanho da matriz (N x N)")
plt.ylabel("Tempo médio (s)")
plt.title("Tempo de execução vs. Tamanho da matriz")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

# Gráfico 2: Tempo médio vs. número de processos para tamanhos fixos
plt.figure(figsize=(10, 6))
for size in sorted(grouped["size"].unique()):
    subset = grouped[grouped["size"] == size]
    plt.plot(subset["np"], subset["mean"], marker="o", label=f"{size} x {size}")
plt.xlabel("Número de processos MPI")
plt.ylabel("Tempo médio (s)")
plt.title("Tempo de execução vs. Número de processos")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

# Gráfico 3: Speedup para cada tamanho de matriz
# Speedup = tempo com 2 processos / tempo com N processos
speedup_data = []
for size in grouped["size"].unique():
    base_time = grouped[(grouped["np"] == 2) & (grouped["size"] == size)]["mean"].values[0]
    for _, row in grouped[grouped["size"] == size].iterrows():
        speedup = base_time / row["mean"]
        speedup_data.append({"np": row["np"], "size": size, "speedup": speedup})
speedup_df = pd.DataFrame(speedup_data)

plt.figure(figsize=(10, 6))
for size in sorted(speedup_df["size"].unique()):
    subset = speedup_df[speedup_df["size"] == size]
    plt.plot(subset["np"], subset["speedup"], marker="o", label=f"{size} x {size}")
plt.xlabel("Número de processos MPI")
plt.ylabel("Speedup")
plt.title("Speedup vs. Número de processos")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()
