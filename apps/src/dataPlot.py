#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import argparse
import numpy as np
import matplotlib.pyplot as plt

DATA_LOCATION = "/tmp/mathias/testoutput/"

if __name__ == "__main__":
	parser = argparse.ArgumentParser(description="Visualise Chaste simulation data")
	
	parser.add_argument("--path", default="chaste_modelling/ode_solution.dat", 
		help="path to the data", type=str)

	args = parser.parse_args()	
	filename = args.path
	
	# Check extension
	if os.path.splitext(filename)[1] == '':
		filename = os.path.join(filename, ".dat")

	elif os.path.splitext(filename)[1] != ".dat":
		sys.stderr.write("Error: extension should be .dat\n")
		exit(1)

	path = os.path.join(DATA_LOCATION, filename)

	time, u, r = np.loadtxt(path, unpack=True, skiprows=1)

	plt.plot(time, u) 
	plt.title("FHN simulation")
	plt.xlabel("Time (s)")
	plt.ylabel("Membrane potential (mV)")

	plt.show()
