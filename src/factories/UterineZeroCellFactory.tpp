#include "../../include/factories/UterineZeroCellFactory.hpp"
#include "Exception.hpp"

template <int DIM>
UterineZeroCellFactory<DIM>::UterineZeroCellFactory() : 
  AbstractUterineCellFactoryTemplate<DIM>(),
  mpStimulus(new ZeroStimulus()) {
  ReadCellParams(this->GetCellParamFile());
}

template <int DIM>
AbstractCvodeCell* UterineZeroCellFactory<DIM>::CreateCardiacCellForTissueNode(
  Node<DIM>* pNode) {
  return this->CreateCardiacCellForTissueNode(pNode);
}


template <int DIM>
void UterineZeroCellFactory<DIM>::ReadParams(std::string general_param_file) {
  this->ReadParams(general_param_file);
}


template <int DIM>
void UterineZeroCellFactory<DIM>::ReadCellParams(std::string cell_param_file) {
  this->ReadCellParams(cell_param_file);
}


template <int DIM>
void UterineZeroCellFactory<DIM>::PrintParams() {
  this->PrintParams();
}


template <int DIM>
void UterineZeroCellFactory<DIM>::WriteLogInfo(std::string log_file) {
  this->WriteLogInfo(log_file);

  std::ofstream log_stream;
  log_stream.open(log_file, ios::app);  // Open log file in append mode

  log_stream << "Stimulus parameters" << std::endl;
  log_stream << "  type: zero" << std::endl;

  log_stream.close();
}
