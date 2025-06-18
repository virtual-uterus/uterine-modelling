#ifndef INCLUDE_FACTORIES_UTERINESIMPLECELLFACTORY_HPP_
#define INCLUDE_FACTORIES_UTERINESIMPLECELLFACTORY_HPP_

#include <iostream>
#include <string>
#include "../toml.hpp"
#include "AbstractUterineCellFactoryTemplate.hpp"
#include "MonodomainProblem.hpp"
#include "SimpleStimulus.hpp"

template <int DIM>
class UterineSimpleCellFactory : public AbstractUterineCellFactoryTemplate<DIM> {
 private:
  double mpX_stim_start;
  double mpX_stim_end;
  double mpY_stim_start;
  double mpY_stim_end;
  double mpZ_stim_start;
  double mpZ_stim_end;
  boost::shared_ptr<SimpleStimulus> mpStimulus;

 public:
  UterineSimpleCellFactory();
  AbstractCvodeCell* CreateCardiacCellForTissueNode(Node<DIM>* pNode);
  void ReadParams(std::string general_param_file);
  void ReadCellParams(std::string cell_param_file);
  void PrintParams() override;
  void WriteLogInfo(std::string log_file);
};

#include "../../src/factories/UterineSimpleCellFactory.tpp"
#endif  // INCLUDE_FACTORIES_UTERINESIMPLECELLFACTORY_HPP_
