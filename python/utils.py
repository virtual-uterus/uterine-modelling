#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# utils.py: Utility functions used for Python code
# Author: Mathias Roesler
# Last modified: 11/24

import os
import re
import sys

import numpy as np
import pandas as pd
import paraview.simple as ps

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
        sys.stderr.write("Error: log file at {} not found\n".format(log_path))
        exit(1)
    except Exception as e:
        sys.stderr.write("Error reading log file: {}\n".format(e))
        exit(1)

    sys.stderr.write("Error: print timestep not found in log file\n")
    exit(1)


def load_data(data_path, log_path, delimiter=","):
    """Loads the data from a csv file

    Arguments:
    data_path -- str, path to the data file.
    log_path -- str, path to the log file of the simulation.
    delimiter -- str, delimiter for the csv file, default value ,.

    Return:
    V -- ndarray, membrane potential values.
    t -- ndarray, timestep values.
    """
    try:
        # Read the file using pandas, specifying the delimiter
        df = pd.read_csv(data_path, delimiter=delimiter)

    # Catch common erros
    except FileNotFoundError:
        sys.stderr.write("Error: file at {} not found\n".format(data_path))
        exit(1)
    except pd.errors.EmptyDataError:
        sys.stderr.write("Error: file is empty.\n")
        exit(1)
    except pd.errors.ParserError:
        sys.stderr.write("Error: could not parse file\n")
        exit(1)
    except Exception as e:
        sys.stderr.write("Error: {}\n".format(e))
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
        sys.stderr.write("Error: missing point IDs in vtu file\n")
        exit(1)

    V = tmp_V  # Reset V for plotting
    # Create correct timesteps in seconds
    t = np.linspace(0, nb_timesteps * timestep * 1e-3, nb_timesteps)

    return V, t


def paraview_extract(mesh_path, save_path, pts_list):
    """Extracts the data from the desired points in the mesh and
    saves in a csv file

    Arguments:
    mesh_path -- str, path to the mesh vtu file.
    save_path -- str, path to the export save file.
    pts_list -- list(int), list of points to extract data from in the mesh.

    Return:

    """
    # Create a new 'XML Unstructured Grid Reader'
    mesh = ps.XMLUnstructuredGridReader(
        registrationName="mesh.vtu",
        FileName=[mesh_path],
    )
    mesh.PointArrayStatus = ["V"]

    # Properties modified on mesh
    mesh.TimeArray = "None"

    # Get active view
    view = ps.GetActiveViewOrCreate("RenderView")

    # Update the view to ensure updated data information
    view.Update()

    # create a query selection
    ps.QuerySelect(
        QueryString="(in1d(id, {}))".format(pts_list),
        FieldType="POINT",
        InsideOut=0,
    )

    # Update the view to ensure updated data information
    view.Update()

    # Create a new 'Extract Selection'
    selected_pts = ps.ExtractSelection(
        registrationName="Selected_pts",
        Input=mesh,
    )

    # Save data
    ps.SaveData(
        save_path,
        proxy=selected_pts,
        WriteTimeSteps=1,
        PointDataArrays=["V"],
        AddMetaData=0,
        AddTime=1,
    )
