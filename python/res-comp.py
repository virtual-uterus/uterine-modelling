#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# res-comp.py: Script that compares the outputs at different mesh resolutions
# Author: Mathias Roesler
# Last modified: 11/24

import argparse
import os

import utils
import sklearn.metrics as skm


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

    # Dictionnary to store different resolutions
    data = {}

    for resolution in utils.RES:
        # Iterate over each resolution
        data_path = file_path + "_{}_res.csv".format(resolution)
        log_path = file_path + "_{}_res.log".format(resolution)

        V, t = utils.load_data(data_path, log_path, args.delimiter)

        data[resolution] = V[:, 0]

        skm.mean_squared_error(y_true, y_pred, squared=False)
