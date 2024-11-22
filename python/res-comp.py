#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
res-comp.py

Script that compares the outputs at different mesh resolutions
Author: Mathias Roesler
Date: 11/24
"""

import argparse
import os

import utils
import plots
import constants
import scipy.stats as stat

from itertools import combinations

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Plots single cell data exported from Paraview"
    )
    parser.add_argument(
        "dir_path",
        type=str,
        metavar="dir-path",
        help="path from BASE to the Chaste save directory",
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
    data = {}
    correl_values = {}
    sim_names = []

    for sim_nb in sim_numbers:
        # Iterate over each simulation
        current_sim_name = f"{args.sim_name}_{sim_nb:03}"
        sim_names.append(current_sim_name)

        data_path = os.path.join(
            dir_path, "extract", "{}.csv".format(current_sim_name))
        log_path = os.path.join(
            dir_path, "log", "{}.log".format(current_sim_name))

        V, t = utils.load_data(data_path, log_path, args.delimiter)

        data[current_sim_name] = V[:, 0]

    for res1, res2 in combinations(data.keys(), 2):
        # Calculate Pearson correlation
        correlation, _ = stat.pearsonr(data[res1], data[res2])
        # Store in dictionary
        correl_values[(res1, res2)] = correlation

    plots.plot_correlation_heatmap(correl_values, sim_names)
