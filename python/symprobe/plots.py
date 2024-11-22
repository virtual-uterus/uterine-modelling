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


def plot_correlation_heatmap(correl_values, sim_names):
    """Creates a heatmap to display the correlation between different
    resolutions

    Arguments:
    correl_values -- dict, dictionnary of correlation values with
    resolution pairs as keys.
    sim_names -- list[str], names of the simulations.

    Return:

    """
    # Initialize a matrix for the heatmap
    nb_sims = len(sim_names)
    correl_matrix = np.ones((nb_sims, nb_sims))

    # Fill only the upper triangle of the matrix
    for i, res1 in enumerate(sim_names):
        for j in range(i + 1, nb_sims):
            res2 = sim_names[j]
            # Retrieve the correlaltion value from the dictionary
            correl = correl_values.get((res1, res2)) or correl_values.get(
                (res2, res1),
                0,
            )

            correl_matrix[i, j] = correl
            # Mirror the value to the lower triangle
            correl_matrix[j, i] = correl

    labels = [x for x in range(nb_sims)]

    # Create the heatmap
    plt.figure(dpi=300)
    ax = sns.heatmap(
        correl_matrix,
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
