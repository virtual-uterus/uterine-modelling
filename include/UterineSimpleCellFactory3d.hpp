#ifndef UTERINESIMPLECELLFACTORY3D_HPP
#define UTERINESIMPLECELLFACTORY3D_HPP

#include <toml.hpp>
#include "AbstractUterineCellFactory3d.hpp"
#include "MonodomainProblem.hpp"
#include "SimpleStimulus.hpp"

class UterineSimpleCellFactory3d : public AbstractUterineCellFactory3d
{
private:
	double mpX_stim_start;
	double mpX_stim_end;
	double mpY_stim_start;
	double mpY_stim_end;
	double mpZ_stim_start;
	double mpZ_stim_end;
	boost::shared_ptr<SimpleStimulus> mpStimulus;

public:
	UterineSimpleCellFactory3d();
	AbstractCvodeCell* CreateCardiacCellForTissueNode(Node<3>* pNode);
	void ReadParams(std::string general_param_file);
	void ReadCellParams(std::string cell_param_file);
	void PrintParams();
	void WriteLogInfo(std::string log_file);
};

#endif // UTERINESIMPLECELLFACTORY3D_HPP

