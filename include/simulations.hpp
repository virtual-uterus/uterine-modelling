#ifndef INCLUDE_SIMULATIONS_HPP_
#define INCLUDE_SIMULATIONS_HPP_

#include <iostream>
#include "Exception.hpp"
#include "PetscException.hpp"
#include "OutputFileHandler.hpp"

#include "factories/UterineSimpleCellFactory.hpp"
#include "factories/UterineRegularCellFactory.hpp"
#include "factories/UterineZeroCellFactory.hpp"
#include "factories/UterineSimpleCellFactory3d.hpp"
#include "factories/UterineRegularCellFactory3d.hpp"
#include "factories/UterineRegionCellFactory3d.hpp"
#include "factories/UterineZeroCellFactory3d.hpp"

void simulation_2d();
void simulation_3d();

#endif  // INCLUDE_SIMULATIONS_HPP_
