#ifndef INCLUDE_FACTORIES_UTERINEREGIONCELLFACTORY3D_HPP_
#define INCLUDE_FACTORIES_UTERINEREGIONCELLFACTORY3D_HPP_

#include <iostream>
#include <string>
#include <vector>

#include "../toml.hpp"
#include "AbstractUterineCellFactory3d.hpp"
#include "MonodomainProblem.hpp"
#include "../stimulus/UterineRegionStimulus.hpp"

class UterineRegionCellFactory3d : public AbstractUterineCellFactory3d {
 private:
  double mpX_stim_start;
  double mpX_stim_end;
  double mpY_stim_start;
  double mpY_stim_end;
  double mpZ_stim_start;
  double mpZ_stim_end;
  boost::shared_ptr<UterineRegionStimulus> mpOvariesStimulus;
  boost::shared_ptr<UterineRegionStimulus> mpCentreStimulus;
  boost::shared_ptr<UterineRegionStimulus> mpCervicalStimulus;

 public:
  UterineRegionCellFactory3d();
  AbstractCvodeCell* CreateCardiacCellForTissueNode(Node<3>* pNode);
  unsigned FindRegion(double z);
  void ReadParams(std::string general_param_file);
  void ReadCellParams(std::string cell_param_file);
  void SetStimulusParams(
    boost::shared_ptr<UterineRegionStimulus> stimulus,
    double magnitude,
    double period,
    double duration,
    double start,
    std::vector<double> region_probs);
  void PrintParams() override;
  void WriteLogInfo(std::string log_file);
};

#endif  // INCLUDE_FACTORIES_UTERINEREGIONCELLFACTORY3D_HPP_



