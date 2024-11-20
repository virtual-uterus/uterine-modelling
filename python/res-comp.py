#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# res-comp.py: Script that compares the outputs at different mesh resolutions
# Author: Mathias Roesler
# Last modified: 11/24

import argparse
import os

import utils
import plots
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

    # Create path to main directory
    dir_path = os.path.join(
        utils.HOME,
        utils.BASE,
        args.dir_path,
    )

    # Dictionnary to store data and correlation values
    data = {}
    correl_values = {}

    for resolution in utils.RES:
        # Iterate over each resolution
        data_path = dir_path + "extract/{}_{}_res.csv".format(
            args.file_name,
            resolution,
        )
        log_path = dir_path + "log/{}_{}_res.log".format(
            args.file_name,
            resolution,
        )

        V, t = utils.load_data(data_path, log_path, args.delimiter)

        data[resolution] = V[:, 0]

    for res1, res2 in combinations(data.keys(), 2):
        # Calculate Pearson correlation
        correlation, _ = stat.pearsonr(data[res1], data[res2])
        # Store in dictionary
        correl_values[(res1, res2)] = correlation

    plots.plot_correlation_heatmap(correl_values)
