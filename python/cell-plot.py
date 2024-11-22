#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# cell-plot.py: Script that plots single cell data from Paraview
# Author: Mathias Roesler
# Last modified: 11/24

import argparse
import os

import utils
import plots
import constants


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

    # Create file path
    dir_path = os.path.join(
        constants.HOME,
        constants.BASE,
        args.dir_path,
    )
    data_path = os.path.join(
        dir_path,
        "extract",
        "{}.csv".format(args.file_name),
    )
    log_path = os.path.join(
        dir_path,
        "log",
        "{}.log".format(args.file_name),
    )

    V, t = utils.load_data(data_path, log_path, args.delimiter)

    plots.plot_cell_data(V, t)
