import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Caminhos dos arquivos
csv_sem_perturbacao = 'task-11.impact-of-schedule-and-collapse-clauses/data/benchmarks-no-disturbance.csv'
csv_com_perturbacao = 'task-11.impact-of-schedule-and-collapse-clauses/data/benchmarks.csv'

# Lê os dois arquivos
df_sem = pd.read_csv(csv_sem_perturbacao)
df_com = pd.read_csv(csv_com_perturbacao)

# Adiciona uma coluna indicando o tipo
df_sem["perturbacao"] = "sem"
df_com["perturbacao"] = "com"

# Junta os dois
df = pd.concat([df_sem, df_com], ignore_index=True)

# Função para plotar
def plot_benchmark(df, title_suffix, output_filename):
    schedules = df['schedule_type'].unique()
    for sched in schedules:
        plt.figure(figsize=(10, 6))
        subset = df[df['schedule_type'] == sched]
        sns.barplot(
            data=subset,
            x='chunk_size',
            y='time_seconds',
            hue='collapse',
            palette='Set2'
        )
        plt.title(f'Tempo de execução - Schedule: {sched} ({title_suffix})')
        plt.xlabel('Chunk size')
        plt.ylabel('Tempo (s)')
        plt.legend(title='Collapse')
        plt.grid(True, linestyle='--', alpha=0.6)
        plt.tight_layout()
        plt.savefig(f'./task-11.impact-of-schedule-and-collapse-clauses/data/{output_filename}_{sched}.png')
        plt.close()

# Gera gráficos
plot_benchmark(df[df["perturbacao"] == "sem"], "sem perturbação", "benchmark_sem_perturbacao")
plot_benchmark(df[df["perturbacao"] == "com"], "com perturbação", "benchmark_com_perturbacao")

print("Gráficos salvos como 'benchmark_sem_perturbacao_*.png' e 'benchmark_com_perturbacao_*.png'")
