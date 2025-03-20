#ifndef INCLUDE_FACTORIES_UTERINEZEROCELLFACTORY3D_HPP_
#define INCLUDE_FACTORIES_UTERINEZEROCELLFACTORY3D_HPP_

#include <iostream>
#include <string>
#include "../toml.hpp"
#include "AbstractUterineCellFactory3d.hpp"
#include "MonodomainProblem.hpp"
#include "ZeroStimulus.hpp"

class UterineZeroCellFactory3d : public AbstractUterineCellFactory3d {
 private:
  boost::shared_ptr<ZeroStimulus> mpStimulus;

 public:
  UterineZeroCellFactory3d();
  AbstractCvodeCell* CreateCardiacCellForTissueNode(Node<3>* pNode);
  void ReadParams(std::string general_param_file);
  void ReadCellParams(std::string cell_param_file);
  void PrintParams() override;
  void WriteLogInfo(std::string log_file);
};

#endif  // INCLUDE_FACTORIES_UTERINEZEROCELLFACTORY3D_HPP_



