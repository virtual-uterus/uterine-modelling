#!/usr/bin/env python3

import os
import subprocess
import argparse


def parameter_sweep(dim, param, start_val, end_val, step):
    """Performs several simulations with different values of a parameter

    Arguments:
    dim -- int, dimension of the simulation {0, 2, 3}.
    param -- str, name of the parameter to change in the configuration file.
    start_val -- float, start value for the parameter.
    end_val -- float, end value for the parameter.
    step -- float, increase step between two simulations.

    Return:

    """
    # Initial setup
    value = float(start_val)
    end = float(end_val)
    step = float(step)

    # Get the config directory and files
    config_dir = os.getenv("CHASTE_MODELLING_CONFIG_DIR")
    if not config_dir:
        print("Error: CHASTE_MODELLING_CONFIG_DIR env variable is not set")
        return

    cell_type_file = os.path.join(config_dir, f"{dim}d_params.toml")
    with open(cell_type_file, "r") as f:
        for line in f:
            if "cell_type" in line:
                cell_type = line.split('"')[1]
                break
        else:
            print("Error: cell_type not found in config file")
            return

    config_file = os.path.join(config_dir, f"{cell_type}.toml")

    # Check if start value is greater than end value
    if value > end:
        print("Error: the start value is greater than the end value")
        return

    while True:
        # Read and modify config file
        with open(config_file, "r") as f:
            lines = f.readlines()

        for i, line in enumerate(lines):
            if param in line:
                if param == "conductivities_2d":
                    lines[i] = f"{param} = [{value}, {value}] \n"
                elif param == "conductivities_3d":
                    lines[i] = f"{param} = [{value}, {value}, {value}] \n"
                elif param == "magnitude":
                    lines[i] = f"{param} = {value} \n"
                else:
                    lines[i] = f"   {param} = {value} \n"

        with open(config_file, "w") as f:
            f.writelines(lines)

        # Run the chaste simulation
        subprocess.run(["uterine-simulation", str(dim)])

        # Update value for next iteration
        value += step

        # Check termination condition
        if value > end:
            break


if __name__ == "__main__":
    # Argument parser setup
    parser = argparse.ArgumentParser(
        description="Run a parameter sweep for the given parameter"
    )
    parser.add_argument("dim", type=int, help="dimension (2 or 3)")
    parser.add_argument("param", type=str, help="parameter to sweep")
    parser.add_argument(
        "start_val",
        type=float,
        help="start value of the parameter",
    )
    parser.add_argument(
        "end_val",
        type=float,
        help="end value of the parameter",
    )
    parser.add_argument(
        "step",
        type=float,
        help="step value for the parameter sweep",
    )

    args = parser.parse_args()

    if args.param == "conductivities":
        param = args.param + "_" + str(args.dim) + "d"
    else:
        param = args.param

    # Run the parameter sweep with the parsed arguments
    parameter_sweep(args.dim, param, args.start_val, args.end_val, args.step)
