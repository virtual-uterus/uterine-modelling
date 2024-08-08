#include "AbstractUterineCellFactory3d.hpp"
#include "Exception.hpp"

AbstractUterineCellFactory3d::AbstractUterineCellFactory3d() : 
	AbstractCardiacCellFactory<3>()
{
	ReadParams(USMC_3D_SYSTEM_CONSTANTS::GENERAL_PARAM_FILE);
}


AbstractCvodeCell* AbstractUterineCellFactory3d::CreateCardiacCellForTissueNode(
	Node<3>* pNode)
{
	AbstractCvodeCell* cell;

	switch (mpCell_id)
	{
		case 0:
			cell = new CellHodgkinHuxley1952FromCellMLCvode(mpSolver,
				mpZeroStimulus);
			break;

		case 1:
			cell = new CellChayKeizer1983FromCellMLCvode(mpSolver, mpZeroStimulus);
			break;

		case 2:
			cell = new CellMeans2023FromCellMLCvode(mpSolver, mpZeroStimulus);

			for (auto it=mpCell_parameters.begin(); it != mpCell_parameters.end(); ++it)
			{
				cell->SetParameter(it->first, it->second);
			}
			break;
		
		case 3:
			cell = new CellTong2014FromCellMLCvode(mpSolver, mpZeroStimulus);

			for (auto it=mpCell_parameters.begin(); it != mpCell_parameters.end(); ++it)
			{
				cell->SetParameter(it->first, it->second);
			}
			break;

		case 4:
			cell = new CellRoesler2024FromCellMLCvode(mpSolver, mpZeroStimulus);

			for (auto it=mpCell_parameters.begin(); it != mpCell_parameters.end(); ++it)
			{
				cell->SetParameter(it->first, it->second);
			}
			break;

		default:
			cell = new CellHodgkinHuxley1952FromCellMLCvode(mpSolver,
				mpZeroStimulus);
	}	

	return cell;
}


std::string AbstractUterineCellFactory3d::GetCellType()
{
	return mpCell_type;
}


void AbstractUterineCellFactory3d::ReadParams(std::string general_param_file)
{
	std::string general_param_path = USMC_3D_SYSTEM_CONSTANTS::CONFIG_DIR +
		general_param_file;
	const auto params = toml::parse(general_param_path);
	
	mpCell_type = toml::find<std::string>(params, "cell_type");

	// Get the cell id
	std::string cell_param_file = mpCell_type + ".toml";
	ReadCellParams(cell_param_file);
}


void AbstractUterineCellFactory3d::ReadCellParams(std::string cell_param_file)
{
	std::string cell_param_path = USMC_3D_SYSTEM_CONSTANTS::CONFIG_DIR +
		cell_param_file;
	const auto cell_params = toml::parse(cell_param_path);

	mpCell_id = toml::find<unsigned short int>(cell_params, "cell_id");

	if (cell_params.contains("parameters"))
	{
		mpCell_parameters = toml::find<std::unordered_map<std::string, float>>(cell_params, 
			"parameters");
	}
}


void AbstractUterineCellFactory3d::PrintParams()
{
	std::cout << "mpCell_type = " << mpCell_type << std::endl;
	std::cout << "mpCell_id = " << mpCell_id << std::endl;
	std::cout << "mpCell_parameters\n";
	
	for (auto it=mpCell_parameters.begin(); it != mpCell_parameters.end(); ++it)
	{
		std::cout << "  " << it->first << " = " << it->second << std::endl;
	}
}


void AbstractUterineCellFactory3d::WriteLogInfo(std::string log_file)
{
	std::ofstream log_stream;
	log_stream.open(log_file, ios::app); // Open log file in append mode

	log_stream << "Cell parameters \n";

	for (auto it=mpCell_parameters.begin(); it != mpCell_parameters.end(); ++it)
	{
		log_stream << "  " << it->first << ": " << it->second << std::endl;
	}

	log_stream.close();
}

