#ifndef INCLUDE_FACTORIES_UTERINEREGIONCELLFACTORY3D_HPP_
#define INCLUDE_FACTORIES_UTERINEREGIONCELLFACTORY3D_HPP_

#include <iostream>
#include <string>
#include <vector>

#include "../toml.hpp"
#include "AbstractUterineCellFactoryTemplate.hpp"
#include "MonodomainProblem.hpp"
#include "../stimulus/UterineRegionStimulus.hpp"

template <int DIM>
class UterineRegionCellFactory : public AbstractUterineCellFactoryTemplate<DIM> {
 private:
  std::vector<double> mpXStimLeft;  // Cols: start, end
  std::vector<double> mpYStimLeft;  // Cols: start, end
  std::vector<std::vector<double>> mpZStimLeft;  // Rows: ova, cen, cerv
  std::vector<double> mpXStimRight;  // Cols: start, end
  std::vector<double> mpYStimRight;  // Cols: start, end
  std::vector<std::vector<double>> mpZStimRight;  // Rows: ova, cen, cerv
  boost::shared_ptr<UterineRegionStimulus> mpOvariesStimulus;
  boost::shared_ptr<UterineRegionStimulus> mpCentreStimulus;
  boost::shared_ptr<UterineRegionStimulus> mpCervicalStimulus;

 public:
  UterineRegionCellFactory();
  AbstractCvodeCell* CreateCardiacCellForTissueNode(Node<DIM>* pNode);
  unsigned FindRegion(double x, double y, double z);
  unsigned IsInLeft(double x, double y, double z);
  unsigned IsInRight(double x, double y, double z);
  void ReadParams(std::string general_param_file);
  void ReadCellParams(std::string cell_param_file);
  void ReadMeshParams(std::string mesh_param_file, std::string horn);
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

#include "../../src/factories/UterineRegionCellFactory.tpp"
#endif  // INCLUDE_FACTORIES_UTERINEREGIONCELLFACTORY3D_HPP_



