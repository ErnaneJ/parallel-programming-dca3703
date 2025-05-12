import pandas as pd
import matplotlib.pyplot as plt

# Dados de strong scaling
data_strong = {
    "N": [256]*6,
    "NSTEPS": [1000]*6,
    "Threads": [1, 2, 4, 8, 16, 24],
    "Time (s)": [206.182, 181.725, 183.169, 183.608, 184.781, 185.823]
}

# Dados de weak scaling
data_weak = {
    "N": [32, 40, 51, 64, 80, 91],
    "NSTEPS": [1000]*6,
    "Threads": [1, 2, 4, 8, 16, 24],
    "Time (s)": [0.610, 1.197, 2.641, 4.764, 8.399, 11.920]
}

# Criando dataframes
df_strong = pd.DataFrame(data_strong)
df_weak = pd.DataFrame(data_weak)

# Plot - Strong Scaling
plt.figure(figsize=(10, 5))
plt.plot(df_strong["Threads"], df_strong["Time (s)"], marker='o', linestyle='-', color='blue', label="Strong Scaling")
plt.title("Strong Scaling: Tempo vs Número de Threads (N=256)")
plt.xlabel("Número de Threads")
plt.ylabel("Tempo (s)")
plt.grid(True)
plt.xticks(df_strong["Threads"])
plt.legend()
plt.tight_layout()
plt.savefig("strong_scaling.png")
plt.show()

# Plot - Weak Scaling
plt.figure(figsize=(10, 5))
plt.plot(df_weak["Threads"], df_weak["Time (s)"], marker='o', linestyle='-', color='green', label="Weak Scaling")
plt.title("Weak Scaling: Tempo vs Número de Threads (N proporcional)")
plt.xlabel("Número de Threads")
plt.ylabel("Tempo (s)")
plt.grid(True)
plt.xticks(df_weak["Threads"])
plt.legend()
plt.tight_layout()
plt.savefig("weak_scaling.png")
plt.show()
