#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
mesh-distance.py

Script that determines the average distance between elements
Author: Mathias Roesler
Date: 11/24
"""

import os
import sys
import argparse

import numpy as np

from symprobe import utils
from symprobe.constants import HOME, BASE


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
        "--range",
        "-r",
        nargs="+",
        help="range of mesh numbers for comparison",
    )
    parser.add_argument(
        "--extension",
        type=str,
        help="mesh extension",
        default="vtk",
    )

    # Parse input arguments
    args = parser.parse_args()

    # Create file path
    file_path = os.path.join(
        HOME,
        BASE,
        args.dir_path,
        args.mesh_name,
    )

    if args.range is not None:
        sim_numbers = utils.get_range(args.range)

        for sim_nb in sim_numbers:
            mesh_path = "{}_{}.{}".format(
                file_path,
                sim_nb,
                args.extension,
            )

            try:
                distance = utils.neighbour_distance(mesh_path)
            except Exception as e:
                sys.stderr.write("Error: {}\n".format(e))
                exit()

            print(
                "{}_{}: mean distance {:.2f} \u00b1 {:.2f}".format(
                    args.mesh_name,
                    sim_nb,
                    np.mean(distance),
                    np.std(distance),
                )
            )

    else:
        mesh_path = "{}.{}".format(file_path, args.extension)
        distance = utils.neighbour_distance(mesh_path)
        print(
            "{}: mean distance {:.2f} \u00b1 {:.2f}".format(
                args.mesh_name,
                np.mean(distance),
                np.std(distance),
            )
        )
