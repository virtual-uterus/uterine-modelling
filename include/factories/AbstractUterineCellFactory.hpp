#ifndef INCLUDE_FACTORIES_ABSTRACTUTERINECELLFACTORY_HPP_
#define INCLUDE_FACTORIES_ABSTRACTUTERINECELLFACTORY_HPP_

#include "../toml.hpp"
#include "MonodomainProblem.hpp"
#include "ZeroStimulus.hpp"
#include "HodgkinHuxley1952Cvode.hpp"
#include "ChayKeizer1983Cvode.hpp"
#include "Means2023Cvode.hpp"
#include "Tong2014Cvode.hpp"
#include "Roesler2024Cvode.hpp"

namespace USMC_2D_SYSTEM_CONSTANTS {
const std::string CONFIG_DIR = getenv("CHASTE_MODELLING_CONFIG_DIR");
constexpr char GENERAL_PARAM_FILE[] = "general/2d_params.toml";
}

class AbstractUterineCellFactory : public AbstractCardiacCellFactory<2> {
 protected:
  std::string mpCell_type;
  std::string mpEstrus;  // Estrus stage if specified
  std::unordered_map<std::string, float> mpCell_parameters;
  std::int16_t mpCell_id;  // 0 = HH, 1 = CK, 2 = Means, 3 = Tong, 4 = Roesler


 public:
  AbstractUterineCellFactory();
  AbstractCvodeCell* CreateCardiacCellForTissueNode(Node<2>* pNode);
  std::string GetCellType();
  std::string GetCellParamFile();
  virtual void ReadParams(std::string general_param_file);
  virtual void ReadCellParams(std::string cell_param_file);
  virtual void PrintParams();
  virtual void WriteLogInfo(std::string log_file);
};

#endif  // INCLUDE_FACTORIES_ABSTRACTUTERINECELLFACTORY_HPP_
