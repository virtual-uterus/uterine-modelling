#ifndef UTERINEZEROCELLFACTORY_HPP
#define UTERINEZEROCELLFACTORY_HPP

#include <toml.hpp>
#include "AbstractUterineCellFactory.hpp"
#include "MonodomainProblem.hpp"
#include "ZeroStimulus.hpp"

class UterineZeroCellFactory : public AbstractUterineCellFactory
{
private:
    boost::shared_ptr<ZeroStimulus> mpStimulus;

public:
	UterineZeroCellFactory();
    AbstractCvodeCell* CreateCardiacCellForTissueNode(Node<2>* pNode);
	void ReadParams(std::string general_param_file);
	void ReadCellParams(std::string cell_param_file);
	void PrintParams();
	void WriteLogInfo(std::string log_file);
};

#endif // UTERINEZEROCELLFACTORY_HPP


