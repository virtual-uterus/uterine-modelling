#ifndef SIMULATIONS_HPP
#define SIMULATIONS_HPP

#include <iostream>
#include "Exception.hpp"
#include "PetscException.hpp"
#include "OutputFileHandler.hpp"
#include "FHNOdeSystem.hpp"
#include "UterineSimpleCellFactory.hpp"
#include "UterineRegularCellFactory.hpp"
#include "UterineZeroCellFactory.hpp"
#include "UterineSimpleCellFactory3d.hpp"
#include "UterineRegularCellFactory3d.hpp"
#include "UterineZeroCellFactory3d.hpp"

void simulation_0d();
void simulation_2d();
void simulation_3d();

#endif // SIMULATIONS_HPP

