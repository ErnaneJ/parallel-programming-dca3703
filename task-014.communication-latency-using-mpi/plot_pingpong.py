
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("task-014.communication-latency-using-mpi/mpi_pingpong_results.csv")

plt.figure(figsize=(10, 6))
plt.plot(df['Tamanho (bytes)'], df['Tempo médio (us)'], marker='o')
plt.xscale('log')
plt.xlabel('Tamanho da Mensagem (bytes)')
plt.ylabel('Tempo Médio (us)')
plt.title('Tempo de Comunicação MPI vs Tamanho da Mensagem')
plt.grid(True)
plt.tight_layout()
plt.savefig("pingpong_plot.png")
plt.show()
