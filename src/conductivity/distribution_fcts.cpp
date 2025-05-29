#include "../../include/conductivity/distribution_fcts.hpp"

double linear_distribution(double z, double baseline, double slope,
                           double centre) {
  return baseline + (z - centre)*slope;
}

double gaussian_distribution(double z, double baseline, double slope,
                             double centre, double amplitude) {
  return baseline + amplitude*std::exp(-slope*pow(z - centre, 2.0));
}
