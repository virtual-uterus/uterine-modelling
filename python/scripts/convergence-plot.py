#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
convergene-plot.py

Script that compares the outputs at different mesh resolutions
Author: Mathias Roesler
Date: 11/24
"""

import os
import sys
import argparse

import numpy as np

from symprobe import utils, plots, constants, metrics

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Plots the comparison for different resolution meshes"
    )
    parser.add_argument(
        "dir_path",
        type=str,
        metavar="dir-path",
        help="path from BASE to the Chaste save directory",
    )
    parser.add_argument(
        "metric",
        type=str,
        choices={"rmse", "mae", "mse"},
        help="metric used for comparison",
    )
    parser.add_argument(
        "sim_numbers",
        nargs="+",
        metavar="sim-numbers",
        help="numbers of the simulations to extract data from",
    )
    parser.add_argument(
        "--sim-name",
        type=str,
        default="simulation",
        help="name of the simulation prefix",
    )
    parser.add_argument(
        "--delimiter",
        type=str,
        help="delimiter for the reading the data",
        default=",",
    )

    # Parse input arguments
    args = parser.parse_args()

    sim_numbers = utils.get_range(args.sim_numbers)

    # Create path to main directory
    dir_path = os.path.join(
        constants.HOME,
        constants.BASE,
        args.dir_path,
    )

    # Dictionnary to store data and correlation values

    for i, sim_nb in enumerate(sim_numbers):
        # Iterate over each simulation
        current_sim_name = f"{args.sim_name}_{sim_nb:03}"

        data_path = os.path.join(
            dir_path,
            "extract",
            "{}.csv".format(current_sim_name),
        )
        log_path = os.path.join(
            dir_path,
            "log",
            "{}.log".format(current_sim_name),
        )

        try:
            V, t = utils.load_data(data_path, log_path, args.delimiter)
        except Exception as e:
            sys.stderr.write("Error: {}\n".format(e))
            exit()

        if i == 0:
            # Allocate space for data on the first loop
            data = np.zeros((len(V), len(sim_numbers)))

        data[:, i] = V[:, 0]

    comp_data = np.zeros((len(sim_numbers) - 1))

    for i in range(len(sim_numbers) - 1):
        comp_data[i] = metrics.compute_comparison(
            data[:, i],
            data[:, i + 1],
            args.metric,
        )

    plots.plot_resolution_convergence(comp_data, args.metric)
