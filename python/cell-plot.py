#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# cell-plot.py: Script that plots single cell data from Paraview
# Author: Mathias Roesler
# Last modified: 10/24

import argparse
import os

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# Define global constants
HOME = os.path.expanduser("~")
BASE = "Documents/phd"

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
    file_path = os.path.join(HOME, BASE, args.dir_path, args.file_name)

    try:
        # Read the file using pandas, specifying the delimiter
        df = pd.read_csv(file_path, delimiter=args.delimiter)

    # Catch common erros
    except FileNotFoundError:
        print("Error: file at {} not found".format(file_path))
    except pd.errors.EmptyDataError:
        print("Error: file is empty.")
    except pd.errors.ParserError:
        print("Error: could not parse file.")
    except Exception as e:
        print("Error: {}".format(e))

    # Get column names
    columns = df.columns

    # Extract time and membrane potential
    t = df[columns[0]].to_numpy()
    V = df[columns[1]].to_numpy()

    if columns[2] == "vtkOriginalPointIds":
        # Paraview export of single cell data
        cell_ids = np.unique(df[columns[2]].to_numpy())
        nb_cells = len(cell_ids)
        nb_timesteps = t.size // nb_cells
        tmp_V = np.zeros((nb_timesteps, nb_cells))  # Temporary placeholder

        for i in range(nb_cells):
            # Get the output of each cell
            idx = np.arange(i, V.size, nb_cells)
            tmp_V[:, i] = V[idx]

        V = tmp_V  # Reset V for plotting
        t = t[idx]  # Reset t for plotting

    for j in range(V.shape[1]):
        plt.plot(t, V[:, j])

    plt.xlabel("Time (ms)")
    plt.ylabel("Amplitude (mV)")
    plt.show()
