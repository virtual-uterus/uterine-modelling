#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# singleCellTestPlot.py: Script to plot single cell test data from Chaste
# Author: Mathias Roesler
# Last modified: 11/23

import os
import sys
import argparse
import numpy as np
import matplotlib.pyplot as plt

BASE_DIR = os.path.expanduser('~')
DATA_LOCATION = BASE_DIR + "/Documents/phd/testoutput/SingleCellSimulationTest"

if __name__ == "__main__":
	parser = argparse.ArgumentParser(description="Visualise Chaste simulation data")
	
	parser.add_argument("file_name", metavar="file-name",
		help="name of the file", type=str)

	args = parser.parse_args()	
	filename = args.file_name
	
	# Check extension
	if os.path.splitext(filename)[1] == '':
		filename = os.path.join(filename, ".dat")

	elif os.path.splitext(filename)[1] != ".dat":
		sys.stderr.write("Error: extension should be .dat\n")
		exit(1)

	path = os.path.join(DATA_LOCATION, filename)

	data = np.loadtxt(path, unpack=True, skiprows=1)

	plt.plot(data[0, :], data[1, :]) 
	plt.xlabel("Time (s)")
	plt.ylabel("Membrane potential (mV)")

	plt.show()
