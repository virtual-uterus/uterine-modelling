#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# extract-data.py: Script that extracts data from Paraview
# Author: Mathias Roesler
# Last modified: 11/24

import argparse
import os

import utils


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Exports data from given points from mesh in Paraview"
    )
    parser.add_argument(
        "dir_path",
        type=str,
        metavar="dir-path",
        help="path from BASE to the Chaste save directory",
    )
    parser.add_argument(
        "mesh_name",
        type=str,
        metavar="mesh-name",
        help="name of the mesh to load without extension",
    )
    parser.add_argument(
        "pts_list",
        type=int,
        nargs="+",
        metavar="pts-list",
        help="list of points to extract",
    )
    parser.add_argument(
        "--extension",
        type=str,
        help="mesh extension",
        default="vtu",
    )

    # Parse input arguments
    args = parser.parse_args()

    # Create file path
    dir_path = os.path.join(
        utils.HOME,
        utils.BASE,
        args.dir_path,
    )

    save_dir = os.path.join(dir_path, "extract")

    # Create extract directory if it does not exist
    if not os.path.exists(save_dir):
        os.makedirs(save_dir)

    mesh_path = "{}/results/{}.{}".format(
        dir_path,
        args.mesh_name,
        args.extension,
    )

    save_path = save_dir + "/{}.csv".format(args.mesh_name)
    utils.paraview_extract(mesh_path, save_path, args.pts_list)
