import pandas as pd
import matplotlib.pyplot as plt

# Carrega o CSV
df = pd.read_csv("./task-4.memory-or-cpu--bound-applications/data/results.csv")

# Tempo inicial (1 thread) para speedup
base_memory_time = df["Memory-Bound Time (s)"].iloc[0]
base_compute_time = df["Compute-Bound Time (s)"].iloc[0]

# Calcula speedup
df["Memory Speedup"] = base_memory_time / df["Memory-Bound Time (s)"]
df["Compute Speedup"] = base_compute_time / df["Compute-Bound Time (s)"]

# Eficiência
df["Memory Efficiency"] = df["Memory Speedup"] / df["Threads"]
df["Compute Efficiency"] = df["Compute Speedup"] / df["Threads"]

# === PLOTS ===

plt.figure(figsize=(12, 8))

# Tempo
plt.subplot(2, 2, 1)
plt.plot(df["Threads"], df["Memory-Bound Time (s)"], label="Memory-Bound", marker="o")
plt.plot(df["Threads"], df["Compute-Bound Time (s)"], label="Compute-Bound", marker="o")
plt.title("Tempo de Execução vs Número de Threads")
plt.xlabel("Threads")
plt.ylabel("Tempo (s)")
plt.legend()
plt.grid(True)

# Speedup
plt.subplot(2, 2, 2)
plt.plot(df["Threads"], df["Memory Speedup"], label="Memory-Bound", marker="o")
plt.plot(df["Threads"], df["Compute Speedup"], label="Compute-Bound", marker="o")
plt.plot(df["Threads"], df["Threads"], 'k--', label="Speedup Ideal")
plt.title("Speedup vs Threads")
plt.xlabel("Threads")
plt.ylabel("Speedup")
plt.legend()
plt.grid(True)

# Eficiência
plt.subplot(2, 2, 3)
plt.plot(df["Threads"], df["Memory Efficiency"], label="Memory-Bound", marker="o")
plt.plot(df["Threads"], df["Compute Efficiency"], label="Compute-Bound", marker="o")
plt.title("Eficiência vs Threads")
plt.xlabel("Threads")
plt.ylabel("Eficiência")
plt.legend()
plt.grid(True)

# Speedup (zoom até 16 threads)
plt.subplot(2, 2, 4)
df_zoom = df[df["Threads"] <= 16]
plt.plot(df_zoom["Threads"], df_zoom["Memory Speedup"], label="Memory-Bound", marker="o")
plt.plot(df_zoom["Threads"], df_zoom["Compute Speedup"], label="Compute-Bound", marker="o")
plt.plot(df_zoom["Threads"], df_zoom["Threads"], 'k--', label="Ideal")
plt.title("Zoom Speedup (até 16 threads)")
plt.xlabel("Threads")
plt.ylabel("Speedup")
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.savefig("./task-4.memory-or-cpu--bound-applications/data/performance_analysis.png", dpi=300)
plt.show()
