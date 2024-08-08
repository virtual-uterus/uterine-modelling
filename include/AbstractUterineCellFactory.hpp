#ifndef ABSTRACTUTERINECELLFACTORY_HPP
#define ABSTRACTUTERINECELLFACTORY_HPP

#include <toml.hpp>
#include "MonodomainProblem.hpp"
#include "ZeroStimulus.hpp"
#include "HodgkinHuxley1952Cvode.hpp"
#include "ChayKeizer1983Cvode.hpp"
#include "Means2023Cvode.hpp"
#include "Tong2014Cvode.hpp"
#include "Roesler2024Cvode.hpp"

namespace USMC_2D_SYSTEM_CONSTANTS
{
	const std::string CONFIG_DIR = 
		getenv("CHASTE_MODELLING_CONFIG_DIR");
	const std::string GENERAL_PARAM_FILE = 
		"2d_params.toml";
}

class AbstractUterineCellFactory : public AbstractCardiacCellFactory<2>
{
protected:
	std::string mpCell_type;
	std::unordered_map<std::string, float> mpCell_parameters;
	unsigned short int mpCell_id; // 0 = HH, 1 = CK, 2 = Means, 3 = Tong, 4 = Roesler


public:
	AbstractUterineCellFactory();
    AbstractCvodeCell* CreateCardiacCellForTissueNode(Node<2>* pNode);
	std::string GetCellType();
	virtual void ReadParams(std::string general_param_file);
	virtual void ReadCellParams(std::string cell_param_file);
	virtual void PrintParams();
	virtual void WriteLogInfo(std::string log_file);

};

#endif // ABSTRACTUTERINECELLFACTORY_HPP
