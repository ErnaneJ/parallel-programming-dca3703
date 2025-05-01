import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

CSV_NO_DISTURBANCE = 'task-11.impact-of-schedule-and-collapse-clauses/data/benchmarks-no-disturbance.csv'
CSV_WITH_DISTURBANCE = 'task-11.impact-of-schedule-and-collapse-clauses/data/benchmarks.csv'

def load_and_merge_data(csv_no_disturbance: str, csv_with_disturbance: str) -> pd.DataFrame:
    """
    Loads and merges benchmark CSV files with and without disturbance.

    Parameters:
        csv_no_disturbance (str): Path to CSV file without disturbance.
        csv_with_disturbance (str): Path to CSV file with disturbance.

    Returns:
        pd.DataFrame: Combined DataFrame with an added 'disturbance' column.
    """
    df_no = pd.read_csv(csv_no_disturbance)
    df_with = pd.read_csv(csv_with_disturbance)

    df_no["disturbance"] = "no"
    df_with["disturbance"] = "yes"

    return pd.concat([df_no, df_with], ignore_index=True)

def plot_benchmarks(df: pd.DataFrame, title_suffix: str, output_filename_prefix: str):
    """
    Creates bar plots for each schedule type and saves them to PNG files.

    Parameters:
        df (pd.DataFrame): Filtered benchmark data.
        title_suffix (str): Text suffix for plot titles.
        output_filename_prefix (str): Prefix for output file names.
    """
    schedule_types = df['schedule_type'].unique()
    for schedule in schedule_types:
        plt.figure(figsize=(10, 6))
        subset = df[df['schedule_type'] == schedule]

        sns.barplot(
            data=subset,
            x='chunk_size',
            y='time_seconds',
            hue='collapse',
            palette='Set2'
        )

        plt.title(f'Execution Time - Schedule: {schedule} ({title_suffix})')
        plt.xlabel('Chunk Size')
        plt.ylabel('Time (s)')
        plt.legend(title='Collapse')
        plt.grid(True, linestyle='--', alpha=0.6)
        plt.tight_layout()
        output_path = f'./task-11.impact-of-schedule-and-collapse-clauses/data/{output_filename_prefix}_{schedule}.png'
        plt.savefig(output_path)
        plt.close()

if __name__ == '__main__':
    df_combined = load_and_merge_data(CSV_NO_DISTURBANCE, CSV_WITH_DISTURBANCE)
    plot_benchmarks(df_combined[df_combined["disturbance"] == "no"], "No Disturbance", "benchmark_no_disturbance")
    plot_benchmarks(df_combined[df_combined["disturbance"] == "yes"], "With Disturbance", "benchmark_with_disturbance")
    print("Plots saved as 'benchmark_no_disturbance_*.png' and 'benchmark_with_disturbance_*.png'")
