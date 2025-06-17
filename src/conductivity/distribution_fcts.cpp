#include "../../include/conductivity/distribution_fcts.hpp"

double linear_distribution(double z, double baseline, double slope,
                           double centre) {
  double value = baseline + (z - centre)*slope;

  if (value < 0.0) {
    return 0.0;
  } else {
    return value;
  }
}

double gaussian_distribution(double z, double baseline, double slope,
                             double centre, double amplitude) {
  double value;
  value = baseline + amplitude*std::exp(-slope*std::pow(z - centre, 2.0));

  if (value < 0.0) {
    return 0.0;
  } else {
    return value;
  }
}
