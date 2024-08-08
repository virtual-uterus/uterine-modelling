#ifndef UTERINEZEROCELLFACTORY3D_HPP
#define UTERINEZEROCELLFACTORY3D_HPP

#include <toml.hpp>
#include "AbstractUterineCellFactory3d.hpp"
#include "MonodomainProblem.hpp"
#include "ZeroStimulus.hpp"

class UterineZeroCellFactory3d : public AbstractUterineCellFactory3d
{
private:
	boost::shared_ptr<ZeroStimulus> mpStimulus;

public:
	UterineZeroCellFactory3d();
	AbstractCvodeCell* CreateCardiacCellForTissueNode(Node<3>* pNode);
	void ReadParams(std::string general_param_file);
	void ReadCellParams(std::string cell_param_file);
	void PrintParams();
	void WriteLogInfo(std::string log_file);
};

#endif // UTERINEZEROCELLFACTORY3D_HPP



