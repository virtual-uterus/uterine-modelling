#ifndef INCLUDE_FACTORIES_UTERINEZEROCELLFACTORY_HPP_
#define INCLUDE_FACTORIES_UTERINEZEROCELLFACTORY_HPP_

#include <iostream>
#include <string>
#include "../toml.hpp"
#include "AbstractUterineCellFactoryTemplate.hpp"
#include "MonodomainProblem.hpp"
#include "ZeroStimulus.hpp"

template <int DIM>
class UterineZeroCellFactory : public AbstractUterineCellFactoryTemplate<DIM> {
 private:
  boost::shared_ptr<ZeroStimulus> mpStimulus;

 public:
  UterineZeroCellFactory();
  AbstractCvodeCell* CreateCardiacCellForTissueNode(Node<DIM>* pNode);
  void ReadParams(std::string general_param_file);
  void ReadCellParams(std::string cell_param_file);
  void PrintParams() override;
  void WriteLogInfo(std::string log_file);
};

#include "../../src/factories/UterineZeroCellFactory.tpp"
#endif  // INCLUDE_FACTORIES_UTERINEZEROCELLFACTORY_HPP_


