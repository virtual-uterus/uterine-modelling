#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
sweeps.py

Contains function used for changing parameters when running multiple
simulations
Author: Mathias Roesler
Date: 11/24
"""

import os
import subprocess


def resolution_sweep(dim, mesh_name, start_val, end_val):
    """Performs several simulations with different resolution meshes

    The meshes should be named like mesh_name_X.{ele, node, face}, where
    X is a number.

    Arguments:
    dim -- int, dimension of the simulation {0, 2, 3}.
    mesh_name -- str, base name of the mesh.
    start_val -- float, start value for X.
    end_val -- float, end value for X.

    Return:

    """
    # Get the config directory and files
    config_dir = os.getenv("CHASTE_MODELLING_CONFIG_DIR")
    if not config_dir:
        print("Error: CHASTE_MODELLING_CONFIG_DIR env variable is not set")
        return

    config_file = os.path.join(config_dir, f"{dim}d_params.toml")

    # Check if start value is greater than end value
    try:
        assert start_val < end_val

    except AssertionError:
        print("Error: the start value is greater than the end value")
        return

    for j in range(start_val, end_val + 1):
        # Read and modify config file
        with open(config_file, "r") as f:
            lines = f.readlines()

        for i, line in enumerate(lines):
            if "mesh_name" in line:
                # Replace the mesh name
                lines[i] = f'mesh_name = "{mesh_name}_{j}"\n'
                break

        with open(config_file, "w") as f:
            f.writelines(lines)

        # Run the chaste simulation
        subprocess.run(["uterine-simulation", str(dim)])


def parameter_sweep(dim, param, start_val, end_val, step):
    """Performs several simulations with different values of a parameter

    Arguments:
    dim -- int, dimension of the simulation {0, 2, 3}.
    param -- str, name of the parameter to change in the configuration file.
    start_val -- float, start value for the parameter.
    end_val -- float, end value for the parameter.
    step -- float, increase step between two simulations.

    Return:

    """
    # Initial setup
    value = float(start_val)
    end = float(end_val)
    step = float(step)

    # Get the config directory and files
    config_dir = os.getenv("CHASTE_MODELLING_CONFIG_DIR")
    if not config_dir:
        print("Error: CHASTE_MODELLING_CONFIG_DIR env variable is not set")
        return

    cell_type_file = os.path.join(config_dir, f"{dim}d_params.toml")
    with open(cell_type_file, "r") as f:
        for line in f:
            if "cell_type" in line:
                cell_type = line.split('"')[1]
                break
        else:
            print("Error: cell_type not found in config file")
            return

    config_file = os.path.join(config_dir, f"{cell_type}.toml")

    # Check if start value is greater than end value
    if value > end:
        print("Error: the start value is greater than the end value")
        return

    while True:
        # Read and modify config file
        with open(config_file, "r") as f:
            lines = f.readlines()

        for i, line in enumerate(lines):
            if param in line:
                if param == "conductivities_2d":
                    lines[i] = f"{param} = [{value}, {value}] \n"
                elif param == "conductivities_3d":
                    lines[i] = f"{param} = [{value}, {value}, {value}] \n"
                elif param == "magnitude":
                    lines[i] = f"{param} = {value} \n"
                else:
                    lines[i] = f"   {param} = {value} \n"

        with open(config_file, "w") as f:
            f.writelines(lines)

        # Run the chaste simulation
        subprocess.run(["uterine-simulation", str(dim)])

        # Update value for next iteration
        value += step

        # Check termination condition
        if value > end:
            break
