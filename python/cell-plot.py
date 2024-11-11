#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# cell-plot.py: Script that plots single cell data from Paraview
# Author: Mathias Roesler
# Last modified: 11/24

import argparse
import os
import re

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# Define global constants
HOME = os.path.expanduser("~")
BASE = "Documents/phd"


def get_print_timestep(log_path):
    """Extracts the print timestep value from the log file

    Arguments:
    log_path -- str, path to the log file.

    Return:
    timestep -- float, print timestep value in ms.

    """
    try:
        with open(log_path, "r") as log_file:
            for line in log_file:
                if "print timestep" in line:
                    # Use regex to extract numerical value
                    match = re.search(
                        r"print timestep:\s*(\d+\.*\d*)\s*ms",
                        line,
                    )
                    if match:
                        return float(match.group(1))
    except FileNotFoundError:
        print("Error: log file at {} not found".format(log_path))
        exit(1)
    except Exception as e:
        print("Error reading log file: {}".format(e))
        exit(1)

    print("Error: print timestep not found in log file")
    exit(1)


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
    data_path = file_path + ".csv"
    log_path = file_path + ".log"

    try:
        # Read the file using pandas, specifying the delimiter
        df = pd.read_csv(data_path, delimiter=args.delimiter)

    # Catch common erros
    except FileNotFoundError:
        print("Error: file at {} not found".format(data_path))
        exit(1)
    except pd.errors.EmptyDataError:
        print("Error: file is empty.")
        exit(1)
    except pd.errors.ParserError:
        print("Error: could not parse file.")
        exit(1)
    except Exception as e:
        print("Error: {}".format(e))
        exit(1)

    # Get print timestep
    timestep = get_print_timestep(log_path)

    # Get column names
    columns = df.columns

    # Extract time and membrane potential
    time_vals = df[columns[0]].to_numpy()
    V = df[columns[1]].to_numpy()

    if columns[2] == "vtkOriginalPointIds":
        # Paraview export of single cell data
        cell_ids = np.unique(df[columns[2]].to_numpy())
        nb_cells = len(cell_ids)
        nb_timesteps = time_vals.size // nb_cells
        tmp_V = np.zeros((nb_timesteps, nb_cells))  # Temporary placeholder

        for i in range(nb_cells):
            # Get the output of each cell
            idx = np.arange(i, V.size, nb_cells)
            tmp_V[:, i] = V[idx]

        V = tmp_V  # Reset V for plotting
        # Create correct timesteps in seconds
        t = np.linspace(0, nb_timesteps * timestep * 1e-3, nb_timesteps)

    for j in range(V.shape[1]):
        plt.plot(t, V[:, j])

    plt.xlabel("Time (ms)")
    plt.ylabel("Amplitude (mV)")
    plt.show()
