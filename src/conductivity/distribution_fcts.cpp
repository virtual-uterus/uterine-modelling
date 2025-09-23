#include "../../include/conductivity/distribution_fcts.hpp"
//below for pareto distribution
#include <random>

double passive_linear_distribution(double z, double baseline, double slope,
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

double passive_gaussian_distribution(double z, double baseline, double slope,
                             double centre, double amplitude, double min, 
                             double mean, double stddev) {

  //apply normal distribution to the baseline value   
  // Create a random number generator engine
  std::mt19937 generator(std::random_device{}());        
  // Define the normal distribution with mean 0 and standard deviation 1
  //std::normal_distribution<double> distribution(mean, stddev);
  //alternatively use exponential distribution with 1/mean as lambda
  std::exponential_distribution<double> distribution(1/mean);
  
  //we compute the pareto via its CDF  = sigma/(1 - U)^kappa 
  //and use mean for sigma scaling, stddev for kappa shaping
  //note upper range of uniform determines outlier extremity for pareto...adjust at your peril
  //std::uniform_real_distribution<> unifRand(0, 0.999);
  //compute pareto given this uniform
  //double myUnifRand = unifRand(generator);

  double mod_baseline = distribution(generator)*baseline;
  //for some reason mean*baseline must be squared to get right scaling...
  //double mod_baseline = mean*baseline*(mean*baseline/pow((1 - myUnifRand),stddev));

  double value;
  value = mod_baseline*(min+amplitude*std::exp(-slope*std::pow(z - centre, 2.0)));
  //value = baseline*(amplitude+std::exp(-slope*std::pow(z - centre, 2.0)));

  //std::cout << "(distribution_fcts.hpp) baseline: " << baseline << " modified: " << mod_baseline << " value: " << value << std::endl;
  

  if (value < 0.0) {
    //std::cout << "(distribution_fcts.hpp) z: " << z << " returned value: 0.0 " << std::endl;
    return 0.0;
  } else {
    //std::cout << "(distribution_fcts.hpp) z: " << z << " returned value: " << value << std::endl;
    return value;
  }

  

}

//function for spatial modifications to the tissue conductivity
/* std::vector<std::vector<double>> tissue_gaussian_distribution(double z, double baseline, double slope,
                             double centre, double amplitude) {

    std::cout << "(distribution_fcts.hpp) tissue_gaussian_distribution..." << std::endl;
    return zero_matrix<double>(3,3);
} */
                             
