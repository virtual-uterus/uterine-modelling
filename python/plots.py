#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# plots.py: Plot functions used in the Python code
# Author: Mathias Roesler
# Last modified: 11/24

import matplotlib.pyplot as plt


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
