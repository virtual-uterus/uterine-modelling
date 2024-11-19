#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# mesh-quality.py: Script that inspects the quality of a mesh
# Author: Mathias Roesler
# Last modified: 11/24

import argparse
import os

import utils


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Inspects quality of a mesh",
    )
    parser.add_argument(
        "dir_path",
        type=str,
        metavar="dir-path",
        help="path from BASE to the data",
    )
    parser.add_argument(
        "mesh_name",
        type=str,
        metavar="mesh-name",
        help="name of the mesh to load without extension",
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
    file_path = os.path.join(
        utils.HOME,
        utils.BASE,
        args.dir_path,
        args.mesh_name,
    )
    mesh_path = "{}.{}".format(file_path, args.extension)

    utils.paraview_quality(mesh_path)