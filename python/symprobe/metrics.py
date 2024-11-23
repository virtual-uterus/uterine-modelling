#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
metrics.py

Metrics used to compare simulation outputs
Author: Mathias Roesler
Date: 11/24
"""

import numpy as np


def check_broadcasting(y_true, y_pred):
    """Checks if the arrays are broadcastable

    Arguments:
    y_true -- np.array, ground truth values.
    y_pred -- np.array, predicted values.

    Return:

    Raises:
    ValueError -- if the arrays are not broadcastable

    """
    try:
        # Check broadcasting compatibility
        np.broadcast_arrays(y_true, y_pred)
    except ValueError as e:
        raise ValueError(f"arrays are not broadcastable: {e}\n")


def compute_rmse(y_true, y_pred):
    """Computes the root mean square error between two arrays

    Arguments:
    y_true -- np.array, ground truth values.
    y_pred -- np.array, predicted values.

    Return:

    Raises:
    ValueError -- if the arrays are not broadcastable

    """
    # Check broadcasting before calculation
    try:
        check_broadcasting(y_true, y_pred)
    except ValueError as e:
        raise ValueError(e)
    return np.sqrt(np.mean((y_true - y_pred) ** 2))


def compute_mae(y_true, y_pred):
    """Computes the mean average error between arrays.

    Arguments:
    y_true -- np.array, ground truth values.
    y_pred -- np.array, predicted values.

    Return:

    Raises:
    ValueError -- if the arrays are not broadcastable

    """
    # Check broadcasting before calculation
    try:
        check_broadcasting(y_true, y_pred)
    except ValueError as e:
        raise ValueError(e)
    return np.mean(np.abs(y_true - y_pred))


def compute_mse(y_true, y_pred):
    """Computes the mean squared error between arrays.

    Arguments:
    y_true -- np.array, ground truth values.
    y_pred -- np.array, predicted values.

    Return:

    Raises:
    ValueError -- if the arrays are not broadcastable

    """
    # Check broadcasting before calculation
    try:
        check_broadcasting(y_true, y_pred)
    except ValueError as e:
        raise ValueError(e)
    return np.mean((y_true - y_pred) ** 2)
