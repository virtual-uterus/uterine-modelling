#ifndef INCLUDE_FHNODESYSTEM_HPP_
#define INCLUDE_FHNODESYSTEM_HPP_

#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "../include/toml.hpp"

#include "CheckpointArchiveTypes.hpp"
#include "EulerIvpOdeSolver.hpp"
#include "AbstractOdeSystem.hpp"
#include "OdeSystemInformation.hpp"


namespace FHN_ODE_SYSTEM_CONSTANTS {
const std::string CONFIG_DIR = getenv("CHASTE_MODELLING_CONFIG_DIR");
constexpr char GENERAL_PARAM_FILE[] = "0d_params.toml";
const std::string CONFIG_PATH = CONFIG_DIR + "general/" + GENERAL_PARAM_FILE;
}

class FHNOdeSystem : public AbstractOdeSystem {
 private:
  double mA;
  double mB;
  double mC;
  double mFreq;
  double mBeta;
  double mDelta;
  double mGamma;
  double mEpsilon;
  double mStim;
  bool mSlow_wave;  // True if slow-wave version of INCLUDE_FHN

 public:
  FHNOdeSystem();
  explicit FHNOdeSystem(std::string param_config_file);

  void EvaluateYDerivatives(double time,
          const std::vector<double>& rY,
          std::vector<double>& rDY);
  void ReadConfigParams(std::string config_file);
  void PrintParams();
};

#endif  // INCLUDE_FHNODESYSTEM_HPP_
