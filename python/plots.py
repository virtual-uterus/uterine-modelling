#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
plots.py

Plot functions used in the Python code
Author: Mathias Roesler
Date: 11/24
"""

import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns

import utils


def plot_cell_data(V, t):
    """Plots the membrane potential of cells

    Arguments:
    V -- ndarray, array with data from N cells to be plotted.
    t -- ndarray, time vector.

    Return:

    """
    # Create figure and plot
    fig, ax = plt.subplots(dpi=300)

    try:
        assert V.shape[0] == t.shape[0]

    except AssertionError:
        print("Error: dimensions must agree.")
        exit()

    for j in range(V.shape[1]):
        plt.plot(t, V[:, j])

    plt.xlabel("Time (s)")
    plt.ylabel("Amplitude (mV)")

    plt.xlim([0, max(t)])

    # Assume that cells are organised from ovaries to cervix
    plt.legend(["Ovarian end", "Centre", "Cervical end"])
    plt.tight_layout()
    plt.show()


def plot_correlation_heatmap(correl_values):
    """Creates a heatmap to display the correlation between different
    resolutions contained in utils.RES

    Arguments:
    correl_values -- dict, dictionnary of correlation values with
    resolution as keys.

    Return:

    """
    # Initialize a matrix for the heatmap
    n = len(utils.RES)
    correl_matrix = np.ones((n, n))

    # Fill only the upper triangle of the matrix
    for i, res1 in enumerate(utils.RES):
        for j in range(i + 1, n):
            res2 = utils.RES[j]
            # Retrieve the correlaltion value from the dictionary
            correl = correl_values.get((res1, res2)) or correl_values.get(
                (res2, res1),
                0,
            )
            correl_matrix[i, j] = correl
            # Mirror the value to the lower triangle
            correl_matrix[j, i] = correl

    labels = [x.capitalize() for x in utils.RES]

    # Create the heatmap
    plt.figure(dpi=300)
    ax = sns.heatmap(
        correl_matrix,
        annot=True,
        fmt=".2f",
        cmap="YlGnBu",
        vmax=1,
        vmin=0,
        xticklabels=labels,
        yticklabels=labels,
        cbar_kws={"label": "Correlation"},
    )

    ax.set_aspect("equal", "box")
    plt.xlabel("Resolution")
    plt.ylabel("Resolution")
    plt.show()
