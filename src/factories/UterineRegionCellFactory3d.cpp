#include "../../include/factories/UterineRegionCellFactory3d.hpp"
#include "Exception.hpp"

UterineRegionCellFactory3d::UterineRegionCellFactory3d() :
  AbstractUterineCellFactory3d() {
  boost::shared_ptr<UterineRegionSelector> selector(new UterineRegionSelector());
  mpOvariesStimulus = boost::make_shared<UterineRegionStimulus>(0.0, 0.0, 1.0, 0.0, selector);
  mpCentreStimulus = boost::make_shared<UterineRegionStimulus>(0.0, 0.0, 1.0, 0.0, selector);
  mpCervicalStimulus = boost::make_shared<UterineRegionStimulus>(0.0, 0.0, 1.0, 0.0, selector);
  ReadParams(USMC_3D_SYSTEM_CONSTANTS::GENERAL_PARAM_FILE);
  ReadCellParams(AbstractUterineCellFactory3d::GetCellParamFile());
}


AbstractCvodeCell* UterineRegionCellFactory3d::CreateCardiacCellForTissueNode(
  Node<3>* pNode) {
  // double x = pNode->rGetLocation()[0];
  // double y = pNode->rGetLocation()[1];
  double z = pNode->rGetLocation()[2];

  unsigned region = FindRegion(z);
  boost::shared_ptr<UterineRegionStimulus> stimulus;

  switch (region) {
    case 1:
      stimulus = mpOvariesStimulus;
      break;
    case 2:
      stimulus = mpCentreStimulus;
      break;
    case 3:
      stimulus = mpCervicalStimulus;
      break;
    default:
      return AbstractUterineCellFactory3d::CreateCardiacCellForTissueNode(
        pNode);
  }

  stimulus->SetRegion(region);

  AbstractCvodeCell* cell;

  switch (mpCell_id) {
    case 0:
      cell = new CellHodgkinHuxley1952FromCellMLCvode(mpSolver,
        stimulus);
      break;

    case 1:
      cell = new CellChayKeizer1983FromCellMLCvode(mpSolver, stimulus);
      break;

    case 2:
      cell = new CellMeans2023FromCellMLCvode(mpSolver, stimulus);

      for (auto it=mpCell_parameters.begin();
        it != mpCell_parameters.end();
        ++it) {
        cell->SetParameter(it->first, it->second);
      }
      break;

    case 3:
      cell = new CellTong2014FromCellMLCvode(mpSolver, stimulus);

      for (auto it=mpCell_parameters.begin();
        it != mpCell_parameters.end();
        ++it) {
        cell->SetParameter(it->first, it->second);
      }
      break;

    case 4:
      cell = new CellRoesler2024FromCellMLCvode(mpSolver, stimulus);

      for (auto it=mpCell_parameters.begin();
        it != mpCell_parameters.end();
        ++it) {
        cell->SetParameter(it->first, it->second);
      }
      break;

    default:
      cell = new CellHodgkinHuxley1952FromCellMLCvode(mpSolver,
        stimulus);
    }

    return cell;
}


unsigned UterineRegionCellFactory3d::FindRegion(double z) {
  unsigned region = 0;  // Default to no region
  if (z <= 45.0 && z >= 35.0) {
    region = 1;
  } else if (z <= 30.0 && z >= 25.0) {
    region = 2;
  } else if (z <= 25.0 && z >= 20.0) {
    region = 3;
  }
  return region;
}


void UterineRegionCellFactory3d::ReadParams(std::string general_param_file) {
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


void UterineRegionCellFactory3d::ReadCellParams(std::string cell_param_file) {
  std::string cell_param_path = USMC_3D_SYSTEM_CONSTANTS::CONFIG_DIR +
    cell_param_file;
  const auto cell_params = toml::parse(cell_param_path);

  // Stimulus parameters
  auto magnitude = toml::find<double>(cell_params, "magnitude");
  auto period = toml::find<double>(cell_params, "period");
  auto duration = toml::find<double>(cell_params, "duration");
  auto start = toml::find<double>(cell_params, "start_time");
  auto region_probs = toml::find<std::vector<double>>(cell_params, "region_p");

  SetStimulusParams(
    mpOvariesStimulus, magnitude, period, duration, start, region_probs);
  SetStimulusParams(
    mpCentreStimulus, magnitude, period, duration, start, region_probs);
  SetStimulusParams(
    mpCervicalStimulus, magnitude, period, duration, start, region_probs);
}


void UterineRegionCellFactory3d::SetStimulusParams(
    boost::shared_ptr<UterineRegionStimulus> stimulus,
    double magnitude,
    double period,
    double duration,
    double start,
    std::vector<double> region_probs) {
  stimulus->SetMagnitude(magnitude);
  stimulus->SetPeriod(period);
  stimulus->SetDuration(duration);
  stimulus->SetStartTime(start);
  stimulus->SetRegionProbs(region_probs);
}


void UterineRegionCellFactory3d::PrintParams() {
  AbstractUterineCellFactory3d::PrintParams();
  std::cout << "mpX_stim_start = " << mpX_stim_start << "\n";
  std::cout << "mpX_stim_end = " << mpX_stim_end << "\n";
  std::cout << "mpY_stim_start = " << mpY_stim_start << "\n";
  std::cout << "mpY_stim_end = " << mpY_stim_end << "\n";
  std::cout << "mpZ_stim_start = " << mpZ_stim_start << "\n";
  std::cout << "mpZ_stim_end = " << mpZ_stim_end << "\n";
  std::cout << "stimulus magnitude = "
    << mpOvariesStimulus->GetMagnitude()
    << std::endl;
  std::cout << "stimulus period = " << mpOvariesStimulus->GetPeriod() << std::endl;
  std::cout << "stimulus duration = "
    << mpOvariesStimulus->GetDuration()
    << std::endl;
  std::cout << "stimulus start time = " << mpOvariesStimulus->GetStartTime() << std::endl;
}


void UterineRegionCellFactory3d::WriteLogInfo(std::string log_file) {
  AbstractUterineCellFactory3d::WriteLogInfo(log_file);

  std::ofstream log_stream;
  log_stream.open(log_file, ios::app);  // Open log file in append mode

  log_stream << "Stimulus parameters" << std::endl;
  log_stream << "  type: regular" << std::endl;
  log_stream << "  start time: "
    << mpOvariesStimulus->GetStartTime()
    << " ms"
    << std::endl;
  log_stream << "  duration: "
    << mpOvariesStimulus->GetDuration()
    << " ms"
    << std::endl;
  log_stream << "  magnitude: "
    << mpOvariesStimulus->GetMagnitude()
    << " uA/cm2"
    << std::endl;
  log_stream << "  period: " << mpOvariesStimulus->GetPeriod() << " ms" << std::endl;
  log_stream << "  stimulated region: " << mpX_stim_start << " <= x <= ";
  log_stream << mpX_stim_end << "   " << mpY_stim_start << " <= y <= ";
  log_stream << mpY_stim_end << "   " << mpZ_stim_start << " <= z <= ";
  log_stream << mpZ_stim_end << std::endl;

  log_stream.close();
}


