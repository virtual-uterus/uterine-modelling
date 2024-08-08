#include "UterineRegularCellFactory3d.hpp"
#include "Exception.hpp"

UterineRegularCellFactory3d::UterineRegularCellFactory3d() : 
	AbstractUterineCellFactory3d(), 
	mpStimulus(new RegularStimulus(0.0, 0.0, 0.1, 0.0))
{
	ReadParams(USMC_3D_SYSTEM_CONSTANTS::GENERAL_PARAM_FILE);
	ReadCellParams(mpCell_type + ".toml");
}


AbstractCvodeCell* UterineRegularCellFactory3d::CreateCardiacCellForTissueNode(
	Node<3>* pNode)
{
	double x = pNode->rGetLocation()[0];
	double y = pNode->rGetLocation()[1];
	double z = pNode->rGetLocation()[2];

	AbstractCvodeCell* cell;

	if (x >= mpX_stim_start && x <= mpX_stim_end && 
			y >= mpY_stim_start && y <= mpY_stim_end &&
			z >= mpZ_stim_start && z <= mpZ_stim_end)
	{
		switch (mpCell_id)
		{
			case 0:
				cell = new CellHodgkinHuxley1952FromCellMLCvode(mpSolver,
					mpStimulus);
				break;

			case 1:
				cell = new CellChayKeizer1983FromCellMLCvode(mpSolver, mpStimulus);
				break;

			case 2:
				cell = new CellMeans2023FromCellMLCvode(mpSolver, mpStimulus);

				for (auto it=mpCell_parameters.begin(); it != mpCell_parameters.end(); ++it)
				{
					cell->SetParameter(it->first, it->second);
				}
				break;
			
			case 3:
				cell = new CellTong2014FromCellMLCvode(mpSolver, mpStimulus);

				for (auto it=mpCell_parameters.begin(); it != mpCell_parameters.end(); ++it)
				{
					cell->SetParameter(it->first, it->second);
				}
				break;

			case 4:
				cell = new CellRoesler2024FromCellMLCvode(mpSolver, mpStimulus);

				for (auto it=mpCell_parameters.begin(); it != mpCell_parameters.end(); ++it)
				{
					cell->SetParameter(it->first, it->second);
				}
				break;

			default:
				cell = new CellHodgkinHuxley1952FromCellMLCvode(mpSolver,
					mpStimulus);
		}	

		return cell;
	}
	else
	{
		/* The other cells have zero stimuli. */
		return AbstractUterineCellFactory3d::CreateCardiacCellForTissueNode(pNode);
	}
}


void UterineRegularCellFactory3d::ReadParams(std::string general_param_file)
{
	AbstractUterineCellFactory3d::ReadParams(general_param_file);

	std::string general_param_path = USMC_3D_SYSTEM_CONSTANTS::CONFIG_DIR +
		general_param_file;
	const auto params = toml::parse(general_param_path);
	
	// Stimulus location parameters
	mpX_stim_start = toml::find<double>(params, "x_stim_start");
	mpX_stim_end = toml::find<double>(params, "x_stim_end");
	mpY_stim_start = toml::find<double>(params, "y_stim_start");
	mpY_stim_end = toml::find<double>(params, "y_stim_end");
	mpZ_stim_start = toml::find<double>(params, "z_stim_start");
	mpZ_stim_end = toml::find<double>(params, "z_stim_end");
}

void UterineRegularCellFactory3d::ReadCellParams(std::string cell_param_file)
{
	std::string cell_param_path = USMC_3D_SYSTEM_CONSTANTS::CONFIG_DIR +
		cell_param_file;
	const auto cell_params = toml::parse(cell_param_path);
	
	// Stimulus parameters
	mpStimulus->SetMagnitude(toml::find<double>(cell_params, "magnitude"));
	mpStimulus->SetPeriod(toml::find<double>(cell_params, "period"));
	mpStimulus->SetDuration(toml::find<double>(cell_params, "duration"));
	mpStimulus->SetStartTime(toml::find<double>(cell_params, "start_time"));
}


void UterineRegularCellFactory3d::PrintParams()
{
	AbstractUterineCellFactory3d::PrintParams();
	std::cout << "mpX_stim_start = " << mpX_stim_start << "\n";
	std::cout << "mpX_stim_end = " << mpX_stim_end << "\n";
	std::cout << "mpY_stim_start = " << mpY_stim_start << "\n";
	std::cout << "mpY_stim_end = " << mpY_stim_end << "\n";
	std::cout << "mpZ_stim_start = " << mpZ_stim_start << "\n";
	std::cout << "mpZ_stim_end = " << mpZ_stim_end << "\n";
	std::cout << "mpStimulus magnitude = " << mpStimulus->GetMagnitude() << std::endl;
	std::cout << "mpStimulus period = " << mpStimulus->GetPeriod() << std::endl;
	std::cout << "mpStimulus duration = " << mpStimulus->GetDuration() << std::endl;
	std::cout << "mpStimulus start time = " << mpStimulus->GetStartTime() << std::endl;
}


void UterineRegularCellFactory3d::WriteLogInfo(std::string log_file)
{
	AbstractUterineCellFactory3d::WriteLogInfo(log_file);

	std::ofstream log_stream;
	log_stream.open(log_file, ios::app); // Open log file in append mode

	log_stream << "Stimulus parameters" << std::endl;
	log_stream << "  type: regular" << std::endl;
	log_stream << "  start time: " << mpStimulus->GetStartTime() << " ms" << std::endl; 
	log_stream << "  duration: " << mpStimulus->GetDuration() << " ms" << std::endl;
	log_stream << "  magnitude: " << mpStimulus->GetMagnitude() << " uA/cm2" << std::endl;
	log_stream << "  period: " << mpStimulus->GetPeriod() << " ms" << std::endl;
	log_stream << "  stimulated region: " << mpX_stim_start << " <= x <= ";
	log_stream << mpX_stim_end << "   " << mpY_stim_start << " <= y <= ";
	log_stream << mpY_stim_end << "   " << mpZ_stim_start << " <= z <= ";
	log_stream << mpZ_stim_end << std::endl;

	log_stream.close();
}

