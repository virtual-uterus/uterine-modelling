#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# utils.py: Utility functions used for Python code
# Author: Mathias Roesler
# Last modified: 11/24

import os
import re

import numpy as np
import pandas as pd

# Define global constants
HOME = os.path.expanduser("~")
BASE = "Documents/phd"
RES = ["low", "mid", "high"]  # Different resolution strings


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


def load_data(data_path, log_path, delimiter=","):
    try:
        # Read the file using pandas, specifying the delimiter
        df = pd.read_csv(data_path, delimiter=delimiter)

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

    else:
        print("Error: missing point IDs in vtu file.")
        exit(1)

    V = tmp_V  # Reset V for plotting
    # Create correct timesteps in seconds
    t = np.linspace(0, nb_timesteps * timestep * 1e-3, nb_timesteps)

    return V, t
