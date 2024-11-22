import pytest
import numpy as np

from unittest.mock import patch
from symprobe.plots import plot_cell_data, plot_correlation_heatmap


def test_plot_cell_data_shape_mismatch():
    # Define mismatched shapes for V and t
    V = np.random.rand(100, 3)  # 100 time steps, 3 cells
    t = np.linspace(0, 10, 99)  # 99 time steps

    # Expect a ValueError
    with pytest.raises(ValueError, match="dimensions must agree."):
        plot_cell_data(V, t)


@patch("matplotlib.pyplot.show")  # Prevent plots from displaying
def test_plot_cell_data_success(mock_show):
    # Define matching shapes for V and t
    V = np.random.rand(100, 3)  # 100 time steps, 3 cells
    t = np.linspace(0, 10, 100)  # 100 time steps

    # Run the function
    plot_cell_data(V, t)

    # Verify that `show` is called to display the plot
    mock_show.assert_called_once()


@patch("matplotlib.pyplot.show")  # Prevent plots from displaying
def test_plot_correlation_heatmap_success(mock_show):
    # Define correlation values and simulation names
    correl_values = {("res1", "res2"): 0.9, ("res2", "res3"): 0.8}
    sim_names = ["res1", "res2", "res3"]

    # Run the function
    plot_correlation_heatmap(correl_values, sim_names)

    # Verify that `show` is called to display the heatmap
    mock_show.assert_called_once()
