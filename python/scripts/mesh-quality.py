#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
mesh-quality.py

Script that inspects the quality of a mesh
Author: Mathias Roesler
Date: 11/24
"""

import os
import sys
import argparse

from symprobe import constants, utils, plots
import symprobe.paraview_fct as pf


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
        "metric",
        type=str,
        choices={"Aspect ratio", "Jacobian"},
        help="quality metric to use",
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
        constants.HOME,
        constants.BASE,
        args.dir_path,
        args.mesh_name,
    )

    if not len(args.range) == 0:
        sim_numbers = utils.get_range(args.range)

        quality_dict = {}

        for sim_nb in sim_numbers:
            mesh_path = "{}_{}.{}".format(
                file_path,
                sim_nb,
                args.extension,
            )

            try:
                quality_dict[sim_nb] = pf.paraview_quality(
                    mesh_path, args.metric)
            except Exception as e:
                sys.stderr.write("Error: {}\n".format(e))
                exit()

        plots.plot_mesh_quality(quality_dict, args.metric)
