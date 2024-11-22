import pytest
import os
import numpy as np

from unittest.mock import patch, MagicMock
from symprobe.paraview_fct import (
    paraview_extract,
    fetch_quality_data,
    paraview_quality,
)


@patch("paraview.simple.XMLUnstructuredGridReader")
@patch("paraview.simple.SaveData")
@patch("paraview.simple.QuerySelect")
@patch("paraview.simple.ExtractSelection")
@patch("paraview.simple.GetActiveViewOrCreate")
def test_paraview_extract_success(
    mock_get_view, mock_extract, mock_query, mock_save, mock_reader
):
    mock_view = MagicMock()
    mock_get_view.return_value = mock_view

    mesh_path = "mesh.vtu"
    save_path = "output.csv"
    pts_list = [1, 2, 3]

    # Mock mesh reader
    mock_mesh = MagicMock()
    mock_reader.return_value = mock_mesh

    # Run the function
    paraview_extract(mesh_path, save_path, pts_list)

    # Validate sequence of calls
    mock_reader.assert_called_once_with(
        registrationName="mesh.vtu", FileName=[mesh_path]
    )
    mock_query.assert_called_once_with(
        QueryString="(in1d(id, [1, 2, 3]))", FieldType="POINT", InsideOut=0
    )
    mock_extract.assert_called_once_with(
        registrationName="Selected_pts", Input=mock_mesh
    )
    mock_save.assert_called_once_with(
        save_path,
        proxy=mock_extract.return_value,
        WriteTimeSteps=1,
        PointDataArrays=["V"],
        AddMetaData=0,
        AddTime=1,
    )


@patch("paraview.servermanager.Fetch")
def test_fetch_quality_data_success(mock_fetch):
    mock_fetched_data = MagicMock()
    mock_fetch.return_value = mock_fetched_data

    # Mock cell data and quality array
    mock_cell_data = MagicMock()
    mock_fetched_data.GetCellData.return_value = mock_cell_data
    mock_quality_array = MagicMock()
    mock_quality_array.GetNumberOfTuples.return_value = 3
    mock_quality_array.GetValue.side_effect = [1.0, 2.0, 3.0]
    mock_cell_data.GetArray.return_value = mock_quality_array

    quality = "Aspect Ratio"
    mock_mesh_quality = MagicMock()
    mock_view = MagicMock()

    result = fetch_quality_data(quality, mock_mesh_quality, mock_view)

    # Assertions
    assert np.array_equal(result, [1.0, 2.0, 3.0])
    mock_mesh_quality.TetQualityMeasure = quality
    mock_fetch.assert_called_once_with(mock_mesh_quality)


@patch("paraview.servermanager.Fetch")
def test_fetch_quality_data_missing_quality_array(mock_fetch):
    mock_fetched_data = MagicMock()
    mock_fetch.return_value = mock_fetched_data

    # Mock cell data without a quality array
    mock_cell_data = MagicMock()
    mock_fetched_data.GetCellData.return_value = mock_cell_data
    mock_cell_data.GetArray.return_value = None

    quality = "Jacobian"
    mock_mesh_quality = MagicMock()
    mock_view = MagicMock()

    with pytest.raises(ValueError, match="quality array not found"):
        fetch_quality_data(quality, mock_mesh_quality, mock_view)


def test_paraview_quality_success():
    # Define the path to the test mesh
    mesh_path = os.path.join("tests", "test_tet.vtk")

    # Check if the file exists to avoid test failures
    assert os.path.isfile(mesh_path), f"Test mesh not found at {mesh_path}"

    # Run the function
    try:
        paraview_quality(mesh_path)
    except Exception as e:
        pytest.fail(f"paraview_quality raised an exception: {e}\n")
