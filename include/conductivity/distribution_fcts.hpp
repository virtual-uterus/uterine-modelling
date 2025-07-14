#ifndef INCLUDE_CONDUCTIVITY_DISTRIBUTION_FCTS_HPP_
#define INCLUDE_CONDUCTIVITY_DISTRIBUTION_FCTS_HPP_

#include <iostream>
#include <cmath>
#include <random>

#include "Exception.hpp"


double linear_distribution(double z, double baseline, double slope,
                           double centre, double mean, double stddev);
double gaussian_distribution(double z, double baseline, double slope,
                             double centre, double amplitude, double mean, 
                             double stddev);

#endif  // INCLUDE_CONDUCTIVITY_DISTRIBUTION_FCTS_HPP_

