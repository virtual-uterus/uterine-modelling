import pytest
import numpy as np

from symprobe.metrics import (
    compute_rmse,
    compute_mae,
    compute_mse,
    check_broadcasting,
)


# Test check_broadcasting function
def test_check_broadcasting_valid():
    y_true = np.array([1, 2, 3])
    y_pred = np.array([1, 2, 3])
    # Should not raise an error
    check_broadcasting(y_true, y_pred)


def test_check_broadcasting_invalid():
    y_true = np.array([1, 2])
    y_pred = np.array([1, 2, 3])
    # Should raise ValueError
    with pytest.raises(ValueError):
        check_broadcasting(y_true, y_pred)


# Test compute_rmse function
def test_compute_rmse_valid():
    y_true = np.array([1, 2, 3])
    y_pred = np.array([1, 2, 3])
    result = compute_rmse(y_true, y_pred)
    assert np.isclose(result, 0.0), f"Expected 0.0, but got {result}"


def test_compute_rmse_non_broadcastable():
    y_true = np.array([1, 2])
    y_pred = np.array([1, 2, 3])
    with pytest.raises(ValueError):
        compute_rmse(y_true, y_pred)


def test_compute_rmse_edge_case():
    y_true = np.array([0])
    y_pred = np.array([0])
    result = compute_rmse(y_true, y_pred)
    assert np.isclose(result, 0.0), f"Expected 0.0, but got {result}"


# Test compute_mae function
def test_compute_mae_valid():
    y_true = np.array([1, 2, 3])
    y_pred = np.array([1, 2, 3])
    result = compute_mae(y_true, y_pred)
    assert np.isclose(result, 0.0), f"Expected 0.0, but got {result}"


def test_compute_mae_non_broadcastable():
    y_true = np.array([1, 2])
    y_pred = np.array([1, 2, 3])
    with pytest.raises(ValueError):
        compute_mae(y_true, y_pred)


def test_compute_mae_edge_case():
    y_true = np.array([0])
    y_pred = np.array([0])
    result = compute_mae(y_true, y_pred)
    assert np.isclose(result, 0.0), f"Expected 0.0, but got {result}"


# Test compute_mse function
def test_compute_mse_valid():
    y_true = np.array([1, 2, 3])
    y_pred = np.array([1, 2, 3])
    result = compute_mse(y_true, y_pred)
    assert np.isclose(result, 0.0), f"Expected 0.0, but got {result}"


def test_compute_mse_non_broadcastable():
    y_true = np.array([1, 2])
    y_pred = np.array([1, 2, 3])
    with pytest.raises(ValueError):
        compute_mse(y_true, y_pred)


def test_compute_mse_edge_case():
    y_true = np.array([0])
    y_pred = np.array([0])
    result = compute_mse(y_true, y_pred)
    assert np.isclose(result, 0.0), f"Expected 0.0, but got {result}"
