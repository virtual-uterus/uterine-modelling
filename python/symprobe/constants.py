#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
constants.py

Constants for the symprobe package
Author: Mathias Roesler
Date: 11/24
"""

import os

# Define global constants
HOME = os.path.expanduser("~")
BASE = "Documents/phd"
CONFIG_ENV_VAR = "CHASTE_MODELLING_CONFIG_DIR"
PTS_LIST = {  # List of points to extract for the scaffolds
    "uterus_scaffold_0": [195, 265, 329],
    "uterus_scaffold_1": [365, 499, 627],
    "uterus_scaffold_2": [725, 995, 1251],
    "uterus_scaffold_3": [1088, 1493, 1877],
    "uterus_scaffold_4": [1595, 2192, 2768],
    "uterus_scaffold_5": [2387, 3284, 4148],
    "uterus_scaffold_6": [3183, 4379, 5531],
    "uterus_scaffold_7": [4195, 5775, 7311],
    "uterus_scaffold_8": [5587, 7695, 9743],
    "uterus_scaffold_9": [6984, 9619, 12179],
}
CONVERSION_IDX = [  # List of node indices for each tetrahedra
    [0, 1, 2, 4],  # Tetrahedron 1
    [0, 2, 3, 4],  # Tetrahedron 2
    [1, 2, 4, 5],  # Tetrahedron 3
    [2, 3, 4, 7],  # Tetrahedron 4
    [2, 4, 5, 6],  # Tetrahedron 5
    [2, 4, 6, 7],  # Tetrahedron 6
]
