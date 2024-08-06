
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


def plot_image_performance(path: str) -> None:
    df = pd.read_csv(path)
    cols = df.columns

    fig, ax1 = plt.subplots(1, 1, figsize=(10, 6))
    ax1.grid(True)

    ax1.set_title("Image size runtime plot")
    ax1.set_xlabel("Image size in pixels")
    ax1.set_ylabel("Duration{microseconds}")

    colors = ['C0', 'C1', 'C2']

    for i, col in enumerate(cols[-3:], start=1):
        x_data = np.log(df['Size'])
        y_data = np.log(df[col])

        coeffs = np.polyfit(x_data, y_data, 1)
        poly = np.poly1d(coeffs)

        slope = coeffs[0]
        ax1.loglog(df['Size'], df[col], '.-', c=colors[i-1], label=f'{col} (grad: {slope:.2f})')

    ax1.legend()
    plt.show()


def plot_volume_performance(path: str) -> None:
    df = pd.read_csv(path)
    cols = df.columns

    fig, ax1 = plt.subplots(1, 1, figsize=(10, 6))
    ax1.grid(True)

    ax1.set_title("Volume size runtime plot")
    ax1.set_xlabel("Volume depth")
    ax1.set_ylabel("Duration{microseconds}")

    colors = ['C0', 'C1', 'C2', 'C3']

    for i, col in enumerate(cols[-4:], start=1):
        x_data = np.log(df['Size'])
        y_data = np.log(df[col])

        coeffs = np.polyfit(x_data, y_data, 1)
        poly = np.poly1d(coeffs)

        slope = coeffs[0]
        ax1.loglog(df['Size'], df[col], '.-', c=colors[i-1], label=f'{col} (grad: {slope:.2f})')

    ax1.legend()
    plt.show()


def plot_kernel_performance(path: str) -> None:
    df = pd.read_csv(path)
    cols = df.columns

    fig, ax1 = plt.subplots(1, 1, figsize=(10, 6))
    ax1.grid(True)

    ax1.set_title("Kernel size runtime plot")
    ax1.set_xlabel("Kernel size")
    ax1.set_ylabel("Duration{microseconds}")

    colors = ['C0', 'C1', 'C2']

    for i, col in enumerate(cols[-3:], start=1):
        x_data = np.log(df['Size'])
        y_data = np.log(df[col])

        coeffs = np.polyfit(x_data, y_data, 1)
        poly = np.poly1d(coeffs)
        slope = coeffs[0]
        ax1.loglog(df['Size'], df[col], '.-', c=colors[i-1], label=f'{col} (grad: {slope:.2f})')

    ax1.legend()
    plt.show()
