#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# cubeToTet.py: Script to convert a cubic mesh to a tetrahedral one
# Author: Mathias Roesler
# Last modified: 09/23

import os
import sys
import argparse
		

def convertConnections(cube_node_list):
	""" Converts the connections of the cubic element to six tetrahedra
	connections

	Arguments:
	cube_node_list -- list[int], list of nodes for the cubic element.

	Return:
	tet_node_list -- list[list[int]], list of the six node lists for the 
		tetrahedral elements.
	
	"""
	tet_node_list = []
	conversion_idx = [ # List of node indices for each tetrahedra
	[0, 1, 2, 4],
	[0, 3, 2, 4],
	[1, 2, 4, 5],
	[3, 2, 4, 7],
	[2, 4, 5, 6],
	[2, 4, 7, 6]]
	
	for idx_list in conversion_idx:
		tet_list = [cube_node_list[idx] for idx in idx_list]
		tet_node_list.append(tet_list)	

	return tet_node_list


if __name__ == "__main__":
	parser = argparse.ArgumentParser(description=
		"Converts a cubic vtk mesh to a tetrahedral one")

	# Parse input arguments
	parser.add_argument("mesh_path", type=str, metavar="mesh-path",
		help="path to the mesh to convert")

	args = parser.parse_args()

	[mesh_path, mesh_name] = os.path.split(args.mesh_path)
	[mesh_name, mesh_ext] = mesh_name.split('.')

	# Check extension
	try:
		assert(mesh_ext == 'vtk')

	except AssertionError:
		sys.stderr.write("Error: incorrect extension\n")
		sys.stderr.write("Got {} instead of vtk".format(mesh_ext))
		exit()

	new_mesh = os.path.join(mesh_path, mesh_name + '_tet.' + mesh_ext)

	# Read the whole vtk file
	with open(args.mesh_path, 'r') as f:
		mesh = f.read()

	# Open the new mesh
	with open(new_mesh, 'w') as f:
		mesh_lines = mesh.split('\n')
	
		# Write header and POINTS
		for i, line in enumerate(mesh_lines):
			if line[0:5] == "CELLS":
				break

			else:
				f.write(line + '\n')

		# Get the number of cells
		cell_line = mesh_lines[i]
		[CELLS, n, size] = cell_line.split(' ') # Using VTK notation

		# Convert n and size to int
		n = int(n)
		size = int(size)

		# Calculate new values
		new_n = 6 * n
		new_size = new_n * 5

		# Write the CELLS
		f.write("CELLS {} {}\n".format(new_n, new_size))

		for j, line in enumerate(mesh_lines[i+1:]):
			if line[0] == 'C':
				# Exit after reading all the cells
				break
			
			str_node_list = line.split(' ')
			int_node_list = [int(x) for x in str_node_list[1:]] 

			tet_node_list = convertConnections(int_node_list)

			for node_list in tet_node_list:
				f.write("4 ") # Write the number of points

				for idx in node_list:
					f.write("{} ".format(idx))

				f.write('\n') # Write EOL

		# Write the CELL_TYPES
		f.write("CELL_TYPES {}\n".format(new_n))

		for k in range(new_n):
			f.write("10\n") # VTK_TETRA id	
