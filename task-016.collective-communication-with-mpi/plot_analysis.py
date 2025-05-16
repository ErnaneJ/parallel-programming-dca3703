import pandas as pd
import matplotlib.pyplot as plt

# Carrega o CSV formatado manualmente pelo usuário
from io import StringIO

csv_data = """
Number of Process,Size,Execution,Time (s)
2,512 x 512,1,0.000136
2,512 x 512,2,0.000137
2,512 x 512,3,0.000130
2,1024 x 1024,1,0.000442
2,1024 x 1024,2,0.000442
2,1024 x 1024,3,0.000442
2,2048 x 2048,1,0.002082
2,2048 x 2048,2,0.002069
2,2048 x 2048,3,0.002075
2,3072 x 3072,1,0.004566
2,3072 x 3072,2,0.004595
2,3072 x 3072,3,0.004583
2,4096 x 4096,1,0.008106
2,4096 x 4096,2,0.008121
2,4096 x 4096,3,0.008124
4,512 x 512,1,0.000074
4,512 x 512,2,0.000069
4,512 x 512,3,0.000067
4,1024 x 1024,1,0.000300
4,1024 x 1024,2,0.000261
4,1024 x 1024,3,0.000244
4,2048 x 2048,1,0.001211
4,2048 x 2048,2,0.001065
4,2048 x 2048,3,0.001216
4,3072 x 3072,1,0.002710
4,3072 x 3072,2,0.002710
4,3072 x 3072,3,0.002735
4,4096 x 4096,1,0.004791
4,4096 x 4096,2,0.004825
4,4096 x 4096,3,0.004802
8,512 x 512,1,0.000039
8,512 x 512,2,0.000040
8,512 x 512,3,0.000040
8,1024 x 1024,1,0.000158
8,1024 x 1024,2,0.000156
8,1024 x 1024,3,0.000176
8,2048 x 2048,1,0.000654
8,2048 x 2048,2,0.000609
8,2048 x 2048,3,0.000586
8,3072 x 3072,1,0.001355
8,3072 x 3072,2,0.001474
8,3072 x 3072,3,0.001482
8,4096 x 4096,1,0.002689
8,4096 x 4096,2,0.002447
8,4096 x 4096,3,0.002469
16,512 x 512,1,0.000023
16,512 x 512,2,0.000026
16,512 x 512,3,0.000023
16,1024 x 1024,1,0.000079
16,1024 x 1024,2,0.000083
16,1024 x 1024,3,0.000078
16,2048 x 2048,1,0.000312
16,2048 x 2048,2,0.000315
16,2048 x 2048,3,0.000315
16,3072 x 3072,1,0.000722
16,3072 x 3072,2,0.000713
16,3072 x 3072,3,0.000709
16,4096 x 4096,1,0.001240
16,4096 x 4096,2,0.001251
16,4096 x 4096,3,0.001248
32,512 x 512,1,0.000016
32,512 x 512,2,0.000016
32,512 x 512,3,0.000016
32,1024 x 1024,1,0.000045
32,1024 x 1024,2,0.000047
32,1024 x 1024,3,0.000046
32,2048 x 2048,1,0.000174
32,2048 x 2048,2,0.000168
32,2048 x 2048,3,0.000173
32,3072 x 3072,1,0.000387
32,3072 x 3072,2,0.000377
32,3072 x 3072,3,0.000372
32,4096 x 4096,1,0.000651
32,4096 x 4096,2,0.000656
32,4096 x 4096,3,0.000638
"""

# Carregar no DataFrame
df = pd.read_csv(StringIO(csv_data))
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
