import matplotlib.pyplot as plt

# Dados de tempo de execução para cada compilador e tipo de soma
optimizations = ['-O0', '-O2', '-O3']
initialization_times = [0.127321, 0.041316, 0.040452]
dependent_times = [0.118464, 0.023662, 0.022007]
independent_2_accum_times = [0.055164, 0.007997, 0.006915]
independent_4_accum_times = [0.051404, 0.006590, 0.007453]
independent_8_accum_times = [0.054173, 0.009964, 0.009818]

# Criação de um gráfico para cada tipo de soma
fig, ax = plt.subplots(figsize=(10, 6))

# Plots para cada tipo de soma
ax.plot(optimizations, initialization_times, label='Inicialização', marker='o')
ax.plot(optimizations, dependent_times, label='Dependente', marker='o')
ax.plot(optimizations, independent_2_accum_times, label='Independente - 2 Acumuladores', marker='o')
ax.plot(optimizations, independent_4_accum_times, label='Independente - 4 Acumuladores', marker='o')
ax.plot(optimizations, independent_8_accum_times, label='Independente - 8 Acumuladores', marker='o')

# Customização do gráfico
ax.set_xlabel('Níveis de Otimização')
ax.set_ylabel('Tempo de Execução (segundos)')
ax.set_title('Impacto das Otimizações no Tempo de Execução')
ax.legend()

# Exibindo o gráfico
plt.grid(True)
plt.show()
