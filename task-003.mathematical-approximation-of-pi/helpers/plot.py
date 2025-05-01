import pandas as pd
import matplotlib.pyplot as plt
import os

# Carregar os dados do CSV
file_path = os.path.join(os.path.dirname(__file__), '../data/pi_approximation.csv')
data = pd.read_csv(file_path)

# Criar gráficos
plt.figure(figsize=(12, 8))

# Gráfico 1: Iterações vs Aproximação de PI
plt.subplot(2, 2, 1)
plt.plot(data['Approximation of PI'], data['Iterations'], marker='o', linestyle='-')
plt.yscale('log')
plt.ylabel("Iterações (log scale)")
plt.xlabel("Aproximação de PI")
plt.title("Iterações vs Aproximação de PI")
plt.gca().invert_xaxis()

# Gráfico 2: Iterações vs Erro Absoluto
plt.subplot(2, 2, 2)
plt.plot(data['Absolute Error'], data['Iterations'], marker='o', linestyle='-', color='g')
plt.xscale('log')
plt.yscale('log')
plt.ylabel("Iterações (log scale)")
plt.xlabel("Erro Absoluto (log scale)")
plt.title("Iterações vs Erro Absoluto")

# Gráfico 3: Iterações vs Tempo de Execução
plt.subplot(2, 2, 3)
plt.plot(data['Execution Time (s)'], data['Iterations'], marker='o', linestyle='-', color='m')
plt.xscale('log')
plt.yscale('log')
plt.ylabel("Iterações (log scale)")
plt.xlabel("Tempo de Execução (s)")
plt.title("Iterações vs Tempo de Execução")

# Gráfico 4: Iterações vs Casas Decimais Corretas
plt.subplot(2, 2, 4)
plt.plot(data['Correct Decimal Places'], data['Iterations'], marker='o', linestyle='-', color='c')
plt.yscale('log')
plt.ylabel("Iterações (log scale)")
plt.xlabel("Casas Decimais Corretas")
plt.title("Iterações vs Casas Decimais Corretas")

# Ajustar espaçamentos
plt.tight_layout()
plt.show()