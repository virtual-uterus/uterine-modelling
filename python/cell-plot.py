#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# cell-plot.py: Script that plots single cell data from Paraview
# Author: Mathias Roesler
# Last modified: 11/24

import argparse
import os

import utils
import matplotlib.pyplot as plt


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Plots single cell data exported from Paraview"
    )
    parser.add_argument(
        "dir_path",
        type=str,
        metavar="dir-path",
        help="path from BASE to the data",
    )
    parser.add_argument(
        "file_name",
        type=str,
        metavar="file-name",
        help="name of the file to load",
    )
    parser.add_argument(
        "--delimiter",
        type=str,
        help="delimiter for the reading the data",
        default=",",
    )

    # Parse input arguments
    args = parser.parse_args()

    # Create file path
    file_path = os.path.join(
        utils.HOME,
        utils.BASE,
        args.dir_path,
        args.file_name,
    )
    data_path = file_path + ".csv"
    log_path = file_path + ".log"

    V, t = utils.load_data(data_path, log_path, args.delimiter)
    # Create figure and plot
    fig, ax = plt.subplots(dpi=300)

    for j in range(V.shape[1]):
        plt.plot(t, V[:, j])

    plt.xlabel("Time (s)")
    plt.ylabel("Amplitude (mV)")

    plt.xlim([0, max(t)])

    # Assume that cells are organised from ovaries to cervix
    plt.legend(["Ovarian end", "Centre", "Cervical end"])
    plt.tight_layout()
    plt.show()
