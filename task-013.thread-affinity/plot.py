import pandas as pd
import matplotlib.pyplot as plt

csv_path = "task-013.thread-affinity/resultados_teste_omp.csv"
df = pd.read_csv(csv_path)

# Gráfico de Tempo de Execução
plt.figure(figsize=(12, 6))
for bind in df["OMP_PROC_BIND"].unique():
    subset = df[df["OMP_PROC_BIND"] == bind]
    plt.plot(
        subset["Threads"],
        subset["Tempo (s)"],
        marker="o",
        label=f"OMP_PROC_BIND={bind}",
    )
plt.title("Tempo de Execução por Número de Threads e Afinidade")
plt.xlabel("Número de Threads")
plt.ylabel("Tempo (s)")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()

# Gráfico de Speedup
plt.figure(figsize=(12, 6))
for bind in df["OMP_PROC_BIND"].unique():
    subset = df[df["OMP_PROC_BIND"] == bind].sort_values("Threads")
    tempo_1_thread = subset[subset["Threads"] == 1]["Tempo (s)"].values[0]
    speedup = tempo_1_thread / subset["Tempo (s)"]
    plt.plot(
        subset["Threads"],
        speedup,
        marker="o",
        label=f"OMP_PROC_BIND={bind}",
    )
plt.title("Aceleração (Speedup) por Número de Threads e Afinidade")
plt.xlabel("Número de Threads")
plt.ylabel("Speedup")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()

# Gráfico de Eficiência
plt.figure(figsize=(12, 6))
for bind in df["OMP_PROC_BIND"].unique():
    subset = df[df["OMP_PROC_BIND"] == bind].sort_values("Threads")
    tempo_1_thread = subset[subset["Threads"] == 1]["Tempo (s)"].values[0]
    speedup = tempo_1_thread / subset["Tempo (s)"]
    eficiencia = speedup / subset["Threads"]
    plt.plot(
        subset["Threads"],
        eficiencia,
        marker="o",
        label=f"OMP_PROC_BIND={bind}",
    )
plt.title("Eficiência por Número de Threads e Afinidade")
plt.xlabel("Número de Threads")
plt.ylabel("Eficiência")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()
