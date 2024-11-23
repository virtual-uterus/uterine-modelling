#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
plots.py

Plot functions used in the symprobe package
Author: Mathias Roesler
Date: 11/24
"""

import matplotlib.pyplot as plt
import numpy as np


def plot_cell_data(V, t):
    """Plots the membrane potential of cells

    Arguments:
    V -- ndarray, array with data from N cells to be plotted.
    t -- ndarray, time vector.

    Return:

    Raises:
    ValueError -- if the shapes do not agree.

    """
    # Create figure and plot
    fig, ax = plt.subplots(dpi=300)

    if not V.shape[0] == t.shape[0]:
        raise ValueError("dimensions must agree.")

    for j in range(V.shape[1]):
        plt.plot(t, V[:, j])

    plt.xlabel("Time (s)")
    plt.ylabel("Amplitude (mV)")

    plt.xlim([0, max(t)])

    # Assume that cells are organised from ovaries to cervix
    plt.legend(["Ovarian end", "Centre", "Cervical end"])
    plt.tight_layout()
    plt.show()


def plot_resolution_convergence(comp_data, metric):
    """Plots the convergence for different resolution meshes

    Arguments:
    comp_data -- np.array, comparison data.
    metric -- str, metric used for the comparison

    Return:

    Raises:

    """
    # Create figure and plot
    fig, ax = plt.subplots(dpi=300)

    x_axis = np.arange(len(comp_data))

    plt.plot(x_axis, comp_data)

    plt.xlabel("Mesh number")
    plt.ylabel("{}".format(metric.upper()))

    plt.show()
