#ifndef INCLUDE_CONDUCTIVITY_DISTRIBUTION_FCTS_HPP_
#define INCLUDE_CONDUCTIVITY_DISTRIBUTION_FCTS_HPP_

#include <iostream>
#include <cmath>
#include <random>

#include "Exception.hpp"
#include "AbstractTetrahedralMesh.hpp"
#include "AbstractConductivityModifier.hpp"


double passive_linear_distribution(double z, double baseline, double slope,
                           double centre, double mean, double stddev);
double passive_gaussian_distribution(double z, double baseline, double slope,
                             double centre, double amplitude, double min, 
                             double mean, double stddev);

//moved to Uterine Conductivity Modifier
//spatial modifier for tissue conductivity
//std::vector<std::vector<double>> tissue_gaussian_distribution(double z, double baseline, double slope,
//                             double centre, double amplitude);                            

#endif  // INCLUDE_CONDUCTIVITY_DISTRIBUTION_FCTS_HPP_

