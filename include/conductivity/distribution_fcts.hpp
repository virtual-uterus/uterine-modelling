#ifndef INCLUDE_CONDUCTIVITY_DISTRIBUTION_FCTS_HPP_
#define INCLUDE_CONDUCTIVITY_DISTRIBUTION_FCTS_HPP_

#include <iostream>
#include "Exception.hpp"


double linear_distribution(double z, double baseline, double slope,
                           double centre);
double gaussian_distribution(double z, double baseline, double slope,
                             double centre, double amplitude);

#endif  // INCLUDE_CONDUCTIVITY_DISTRIBUTION_FCTS_HPP_

