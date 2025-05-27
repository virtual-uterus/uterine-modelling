#ifndef INCLUDE_FACTORIES_ABSTRACTUTERINECELLFACTORYTEMPLATE_HPP_
#define INCLUDE_FACTORIES_ABSTRACTUTERINECELLFACTORYTEMPLATE_HPP_

#include <random>
#include <algorithm>
#include <vector>
#include <cmath>
#include <unordered_map>
#include "../toml.hpp"
#include "MonodomainProblem.hpp"
#include "ZeroStimulus.hpp"
#include "HodgkinHuxley1952Cvode.hpp"
#include "ChayKeizer1983Cvode.hpp"
#include "Means2023Cvode.hpp"
#include "Means2023PCvode.hpp"
#include "Tong2014Cvode.hpp"
#include "Roesler2024Cvode.hpp"
#include "Roesler2024PCvode.hpp"


namespace USMC_SYSTEM_CONSTANTS {
const std::string CONFIG_DIR = getenv("CHASTE_MODELLING_CONFIG_DIR");
constexpr char GENERAL_2D_PARAM_FILE[] = "general/2d_params.toml";
constexpr char GENERAL_3D_PARAM_FILE[] = "general/3d_params.toml";
}

template <int DIM>
class AbstractUterineCellFactoryTemplate : public AbstractCardiacCellFactory<DIM> {
 protected:
  std::string mpCell_type;
  std::string mpEstrus;  // Estrus stage if specified
  std::unordered_map<std::string, float> mpCell_parameters;
  std::unordered_map<std::string, float> mpPassive_parameters;
  std::int16_t mpCell_id;  // 0 = HH, 1 = CK, 2 = Means, 3 = Tong, 4 = Roesler


 public:
  AbstractUterineCellFactoryTemplate();
  virtual ~AbstractUterineCellFactoryTemplate();
  AbstractCvodeCell* CreateCardiacCellForTissueNode(Node<DIM>* pNode);
  std::string GetCellType();
  std::string GetCellParamFile();
  void SetCellParams(AbstractCvodeCell* cell);
  void SetPassiveParams(AbstractCvodeCell* cell, double z);
  void InitCell(AbstractCvodeCell*& cell,
                boost::shared_ptr<AbstractStimulusFunction> stimulus);
  virtual void ReadParams(std::string general_param_file);
  virtual void ReadCellParams(std::string cell_param_file);
  virtual void PrintParams();
  virtual void WriteLogInfo(std::string log_file);
};

#include "../../src/factories/AbstractUterineCellFactoryTemplate.tpp"
#endif  // INCLUDE_FACTORIES_ABSTRACTUTERINECELLFACTORYTEMPLATE_HPP_
