#include "../../include/conductivity/distribution_fcts.hpp"

double linear_distribution(double z, double baseline, double slope,
                           double centre, double mean, double stddev) {

  //apply normal distribution to the baseline value   
  // Create a random number generator engine
  std::mt19937 generator(std::random_device{}());        
  // Define the normal distribution with mean 0 and standard deviation 1
  std::normal_distribution<double> distribution(mean, stddev);
  double mod_baseline = distribution(generator)*baseline;               

  double value = mod_baseline + (z - centre)*slope;

  //debug
  //std::cout << "(distribution_fcts.hpp) baseline: " << baseline << " modified: " << mod_baseline << std::endl;

  if (value < 0.0) {
    return 0.0;
  } else {
    return value;
  }
}

double gaussian_distribution(double z, double baseline, double slope,
                             double centre, double amplitude, double mean, 
                             double stddev) {

  //apply normal distribution to the baseline value   
  // Create a random number generator engine
  std::mt19937 generator(std::random_device{}());        
  // Define the normal distribution with mean 0 and standard deviation 1
  std::normal_distribution<double> distribution(mean, stddev);
  double mod_baseline = distribution(generator)*baseline;

  double value;
  value = mod_baseline*amplitude*std::exp(-slope*std::pow(z - centre, 2.0));

  //std::cout << "(distribution_fcts.hpp) baseline: " << baseline << " modified: " << mod_baseline << std::endl;
  

  if (value < 0.0) {
    //std::cout << "(distribution_fcts.hpp) z: " << z << " returned value: 0.0 " << std::endl;
    return 0.0;
  } else {
    //std::cout << "(distribution_fcts.hpp) z: " << z << " returned value: " << value << std::endl;
    return value;
  }

  

}
