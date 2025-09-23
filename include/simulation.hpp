#ifndef INCLUDE_SIMULATION_HPP_
#define INCLUDE_SIMULATION_HPP_

#include <iostream>
#include "Exception.hpp"
#include "PetscException.hpp"
#include "OutputFileHandler.hpp"

#include "factories/UterineSimpleCellFactory.hpp"
#include "factories/UterineRegularCellFactory.hpp"
#include "factories/UterineZeroCellFactory.hpp"
#include "factories/UterineRegionCellFactory.hpp"
#include "conductivity/UterineConductivityModifier.hpp"
#include "conductivity/UterineTissueModifier.hpp"
#include "VtkMeshWriter.hpp"

void run_simulation(const int dim);
void simulation_2d(std::string stimulus_type, std::string log_path);
void simulation_3d(std::string stimulus_type, std::string log_path, std::string save_path, UterineTissueModifier modifier);

#endif  // INCLUDE_SIMULATION_HPP_
