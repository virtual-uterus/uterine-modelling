#ifndef INCLUDE_FACTORIES_UTERINEREGULARCELLFACTORY_HPP_
#define INCLUDE_FACTORIES_UTERINEREGULARCELLFACTORY_HPP_

#include <iostream>
#include <string>
#include "../toml.hpp"
#include "AbstractUterineCellFactory.hpp"
#include "MonodomainProblem.hpp"
#include "RegularStimulus.hpp"

class UterineRegularCellFactory : public AbstractUterineCellFactory {
 private:
  double mpX_stim_start;
  double mpX_stim_end;
  double mpY_stim_start;
  double mpY_stim_end;
  boost::shared_ptr<RegularStimulus> mpStimulus;

 public:
  UterineRegularCellFactory();
  AbstractCvodeCell* CreateCardiacCellForTissueNode(Node<2>* pNode);
  void ReadParams(std::string general_param_file);
  void ReadCellParams(std::string cell_param_file);
  void PrintParams() override;
  void WriteLogInfo(std::string log_file);
};

#endif  // INCLUDE_FACTORIES_UTERINEREGULARCELLFACTORY_HPP_

