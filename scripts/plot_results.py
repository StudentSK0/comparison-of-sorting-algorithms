from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd


RESULTS_PATH = Path("results/sort_results_avg.csv")
PLOTS_DIR = Path("results/plots")

REQUIRED_COLUMNS = {
    "algorithm",
    "data_type",
    "array_size",
    "avg_time_us",
    "avg_char_comparisons",
}

ALL_ALGORITHMS = [
    "standard_quicksort",
    "standard_mergesort",
    "ternary_string_quicksort",
    "lcp_mergesort",
    "msd_radix_sort",
    "msd_radix_sort_with_quicksort",
]

STANDARD_ALGORITHMS = [
    "standard_quicksort",
    "standard_mergesort",
]

REQUIRED_DATASETS = [
    "random",
    "reverse_sorted",
    "nearly_sorted",
]

COMMON_PREFIX_DATASET = "common_prefix"

ALGORITHM_LABELS = {
    "standard_quicksort": "Стандартный QuickSort",
    "standard_mergesort": "Стандартный MergeSort",
    "ternary_string_quicksort": "Тернарный String QuickSort",
    "lcp_mergesort": "LCP MergeSort",
    "msd_radix_sort": "MSD Radix Sort",
    "msd_radix_sort_with_quicksort": "MSD Radix Sort + QuickSort",
}

DATASET_LABELS = {
    "random": "Случайные строки",
    "reverse_sorted": "Строки в обратном порядке",
    "nearly_sorted": "Почти отсортированные строки",
    "common_prefix": "Строки с общим префиксом",
}

TITLE_PAD = 8
LEGEND_FONT_SIZE = 10
LEGEND_BBOX_Y = 0.99
LAYOUT_TOP = 0.88


def prepare_output_dir() -> None:
    PLOTS_DIR.mkdir(parents=True, exist_ok=True)


def setup_plot_style() -> None:
    plt.rcParams["font.family"] = ["DejaVu Sans", "Arial"]
    plt.rcParams["font.size"] = 10
    plt.rcParams["axes.labelsize"] = 10
    plt.rcParams["axes.titlesize"] = 11
    plt.rcParams["legend.fontsize"] = LEGEND_FONT_SIZE
    plt.rcParams["xtick.labelsize"] = 9
    plt.rcParams["ytick.labelsize"] = 9
    plt.rcParams["figure.dpi"] = 150
    plt.rcParams["savefig.dpi"] = 300
    plt.rcParams["axes.grid"] = True
    plt.rcParams["grid.linestyle"] = "--"
    plt.rcParams["grid.alpha"] = 0.45


def legend_columns_count(labels_count: int) -> int:
    return 2 if labels_count <= 2 else 3


def add_figure_legend(fig: plt.Figure, handles: list, labels: list[str]) -> None:
    fig.legend(
        handles,
        labels,
        loc="upper center",
        bbox_to_anchor=(0.5, LEGEND_BBOX_Y),
        ncol=legend_columns_count(len(labels)),
        frameon=False,
        fontsize=LEGEND_FONT_SIZE,
        handlelength=2.0,
        columnspacing=1.2,
    )


def save_figure(fig: plt.Figure, filename: str) -> None:
    fig.tight_layout(rect=(0, 0, 1, LAYOUT_TOP))
    fig.savefig(PLOTS_DIR / filename)
    plt.close(fig)


def load_results() -> pd.DataFrame:
    if not RESULTS_PATH.exists():
        raise FileNotFoundError(f"Входной CSV-файл не найден: {RESULTS_PATH}")

    df = pd.read_csv(RESULTS_PATH)

    missing_columns = REQUIRED_COLUMNS - set(df.columns)
    if missing_columns:
        raise ValueError(f"В CSV отсутствуют обязательные столбцы: {sorted(missing_columns)}")

    df = df.copy()
    df["algorithm"] = pd.Categorical(df["algorithm"].astype(str), categories=ALL_ALGORITHMS, ordered=True)
    df["data_type"] = df["data_type"].astype(str)
    df = df.sort_values(["data_type", "algorithm", "array_size"])

    return df


def filter_results(
    df: pd.DataFrame,
    algorithms: list[str],
    datasets: list[str],
    graph_name: str,
) -> pd.DataFrame:
    subset = df[df["algorithm"].isin(algorithms) & df["data_type"].isin(datasets)].copy()

    if subset.empty:
        raise ValueError(
            f"Нет данных для графика '{graph_name}' "
            f"(алгоритмы={algorithms}, наборы={datasets})."
        )

    subset["algorithm"] = pd.Categorical(
        subset["algorithm"].astype(str),
        categories=algorithms,
        ordered=True,
    )
    subset = subset.sort_values(["data_type", "algorithm", "array_size"])

    return subset


def validate_combinations(
    df: pd.DataFrame,
    algorithms: list[str],
    datasets: list[str],
    graph_name: str,
) -> None:
    missing_pairs: list[str] = []

    for data_type in datasets:
        for algorithm in algorithms:
            has_rows = not df[(df["data_type"] == data_type) & (df["algorithm"] == algorithm)].empty
            if not has_rows:
                missing_pairs.append(f"({data_type}, {algorithm})")

    if missing_pairs:
        raise ValueError(
            f"Для графика '{graph_name}' нет данных для комбинаций: {', '.join(missing_pairs)}"
        )


def build_line_subplots(
    df: pd.DataFrame,
    algorithms: list[str],
    datasets: list[str],
    metric: str,
    y_label: str,
    filename: str,
) -> None:
    graph_name = filename
    subset = filter_results(df, algorithms, datasets, graph_name)
    validate_combinations(subset, algorithms, datasets, graph_name)

    fig, axes = plt.subplots(1, len(datasets), figsize=(16.0, 4.8), sharex=True)

    if len(datasets) == 1:
        axes = [axes]

    handles = None
    labels = None

    for idx, data_type in enumerate(datasets):
        ax = axes[idx]
        part = subset[subset["data_type"] == data_type]

        for algorithm in algorithms:
            current = part[part["algorithm"] == algorithm]
            ax.plot(
                current["array_size"],
                current[metric],
                marker="o",
                linewidth=1.6,
                markersize=3.6,
                label=ALGORITHM_LABELS[algorithm],
            )

        ax.set_title(DATASET_LABELS[data_type])
        ax.set_xlabel("Размер массива, n")

        if idx == 0:
            ax.set_ylabel(y_label)

        if handles is None:
            handles, labels = ax.get_legend_handles_labels()

    add_figure_legend(fig, handles, labels)
    save_figure(fig, filename)


def build_line_plot_single_dataset(
    df: pd.DataFrame,
    algorithms: list[str],
    data_type: str,
    metric: str,
    y_label: str,
    title: str,
    filename: str,
) -> None:
    graph_name = filename
    subset = filter_results(df, algorithms, [data_type], graph_name)
    validate_combinations(subset, algorithms, [data_type], graph_name)

    fig, ax = plt.subplots(figsize=(7.8, 4.8))

    for algorithm in algorithms:
        current = subset[subset["algorithm"] == algorithm]

        ax.plot(
            current["array_size"],
            current[metric],
            marker="o",
            linewidth=1.7,
            markersize=3.8,
            label=ALGORITHM_LABELS[algorithm],
        )

    ax.set_title(title, pad=TITLE_PAD)
    ax.set_xlabel("Размер массива, n")
    ax.set_ylabel(y_label)

    handles, labels = ax.get_legend_handles_labels()
    add_figure_legend(fig, handles, labels)
    save_figure(fig, filename)


def build_speedup_vs_standard_quicksort_max_size(df: pd.DataFrame) -> None:
    graph_name = "speedup_time_vs_standard_quicksort_max_size.png"
    subset = filter_results(df, ALL_ALGORITHMS, REQUIRED_DATASETS + [COMMON_PREFIX_DATASET], graph_name)

    max_size = int(subset["array_size"].max())
    subset_max = subset[subset["array_size"] == max_size].copy()

    if subset_max.empty:
        raise ValueError(f"Нет данных максимального размера массива для графика '{graph_name}'.")

    validate_combinations(subset_max, ALL_ALGORITHMS, REQUIRED_DATASETS + [COMMON_PREFIX_DATASET], graph_name)

    compared_algorithms = [a for a in ALL_ALGORITHMS if a != "standard_quicksort"]
    dataset_order = REQUIRED_DATASETS + [COMMON_PREFIX_DATASET]

    fig, ax = plt.subplots(figsize=(11.2, 5.3))

    x = np.arange(len(dataset_order))
    width = 0.15

    for i, algorithm in enumerate(compared_algorithms):
        values: list[float] = []

        for data_type in dataset_order:
            baseline_row = subset_max[
                (subset_max["data_type"] == data_type)
                & (subset_max["algorithm"] == "standard_quicksort")
            ]
            current_row = subset_max[
                (subset_max["data_type"] == data_type)
                & (subset_max["algorithm"] == algorithm)
            ]

            if baseline_row.empty or current_row.empty:
                raise ValueError(
                    f"Недостаточно данных для speedup-графика: набор={data_type}, алгоритм={algorithm}."
                )

            baseline_time = float(baseline_row["avg_time_us"].iloc[0])
            current_time = float(current_row["avg_time_us"].iloc[0])

            if current_time == 0:
                raise ValueError(
                    f"avg_time_us == 0 для алгоритма {algorithm} на наборе {data_type}; speedup вычислить нельзя."
                )

            values.append(baseline_time / current_time)

        offset = (i - (len(compared_algorithms) - 1) / 2) * width
        ax.bar(x + offset, values, width, label=ALGORITHM_LABELS[algorithm])

    ax.axhline(1.0, linewidth=1.0)
    ax.set_xlabel("Тип набора данных")
    ax.set_ylabel("Ускорение относительно Standard QuickSort")
    ax.set_xticks(x)
    ax.set_xticklabels([DATASET_LABELS[name] for name in dataset_order], rotation=12)
    ax.set_title(f"Ускорение алгоритмов относительно Standard QuickSort при n = {max_size}", pad=TITLE_PAD)

    handles, labels = ax.get_legend_handles_labels()
    add_figure_legend(fig, handles, labels)
    save_figure(fig, graph_name)


def main() -> None:
    prepare_output_dir()
    setup_plot_style()
    df = load_results()

    build_line_subplots(
        df=df,
        algorithms=STANDARD_ALGORITHMS,
        datasets=REQUIRED_DATASETS,
        metric="avg_time_us",
        y_label="Среднее время, мкс",
        filename="standard_time_required_datasets.png",
    )

    build_line_subplots(
        df=df,
        algorithms=STANDARD_ALGORITHMS,
        datasets=REQUIRED_DATASETS,
        metric="avg_char_comparisons",
        y_label="Среднее число посимвольных сравнений",
        filename="standard_char_comparisons_required_datasets.png",
    )

    build_line_plot_single_dataset(
        df=df,
        algorithms=STANDARD_ALGORITHMS,
        data_type=COMMON_PREFIX_DATASET,
        metric="avg_time_us",
        y_label="Среднее время, мкс",
        title="Время стандартных алгоритмов на строках с общим префиксом",
        filename="standard_time_common_prefix.png",
    )

    build_line_plot_single_dataset(
        df=df,
        algorithms=STANDARD_ALGORITHMS,
        data_type=COMMON_PREFIX_DATASET,
        metric="avg_char_comparisons",
        y_label="Среднее число посимвольных сравнений",
        title="Посимвольные сравнения стандартных алгоритмов на строках с общим префиксом",
        filename="standard_char_comparisons_common_prefix.png",
    )

    build_line_subplots(
        df=df,
        algorithms=ALL_ALGORITHMS,
        datasets=REQUIRED_DATASETS,
        metric="avg_time_us",
        y_label="Среднее время, мкс",
        filename="all_algorithms_time_required_datasets.png",
    )

    build_line_subplots(
        df=df,
        algorithms=ALL_ALGORITHMS,
        datasets=REQUIRED_DATASETS,
        metric="avg_char_comparisons",
        y_label="Среднее число посимвольных сравнений/операций",
        filename="all_algorithms_char_comparisons_required_datasets.png",
    )

    build_line_plot_single_dataset(
        df=df,
        algorithms=ALL_ALGORITHMS,
        data_type=COMMON_PREFIX_DATASET,
        metric="avg_time_us",
        y_label="Среднее время, мкс",
        title="Время всех алгоритмов на строках с общим префиксом",
        filename="all_algorithms_time_common_prefix.png",
    )

    build_line_plot_single_dataset(
        df=df,
        algorithms=ALL_ALGORITHMS,
        data_type=COMMON_PREFIX_DATASET,
        metric="avg_char_comparisons",
        y_label="Среднее число посимвольных сравнений/операций",
        title="Посимвольные сравнения всех алгоритмов на строках с общим префиксом",
        filename="all_algorithms_char_comparisons_common_prefix.png",
    )

    build_speedup_vs_standard_quicksort_max_size(df)

    print(f"Графики сохранены в: {PLOTS_DIR}")


if __name__ == "__main__":
    main()
