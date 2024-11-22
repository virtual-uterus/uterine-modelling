#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
extract-data.py

Script that extracts data from Paraview
Author: Mathias Roesler
Date: 11/24
"""

import argparse
import sys
import os

import utils
import constants


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
        "--extension",
        type=str,
        help="sim extension",
        default="vtu",
    )

    # Parse input arguments
    args = parser.parse_args()

    if len(args.sim_numbers) == 1:
        split = args.sim_numbers[0].split("-")
        if len(split) == 1:
            # Single number
            sim_numbers = int(args.sim_numbers[0])
        else:
            # Range
            sim_numbers = [i for i in range(int(split[0]), int(split[1]) + 1)]
    else:
        # Convert to list to int
        sim_numbers = [int(i) for i in args.sim_numbers]

    # Create file path
    dir_path = os.path.join(
        constants.HOME,
        constants.BASE,
        args.dir_path,
    )

    save_dir = os.path.join(dir_path, "extract")
    log_dir = os.path.join(dir_path, "log")

    # Create extract directory if it does not exist
    if not os.path.exists(save_dir):
        os.makedirs(save_dir)

    for sim_nb in sim_numbers:
        current_sim_name = f"{args.sim_name}_{sim_nb:03}"

        print("Extracting data from {}".format(current_sim_name))

        sim_file = "{}/results/{}.{}".format(
            dir_path,
            current_sim_name,
            args.extension,
        )

        log_file = "{}/{}.log".format(
            log_dir,
            current_sim_name,
        )
        # Get mesh name
        with open(log_file, "r") as f:
            for line in f.readlines():
                if "mesh" in line:
                    mesh_name = line.split(":")[1].strip()
                    break

        try:
            pts_list = constants.PTS_LIST[mesh_name]

        except KeyError:
            sys.stderr.write("Error: mesh {} was not found in list\n".format(mesh_name))
            exit()

        save_path = save_dir + "/{}.csv".format(current_sim_name)
        utils.paraview_extract(sim_file, save_path, pts_list)
