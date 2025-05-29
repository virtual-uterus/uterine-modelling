#include "../../include/factories/UterineRegularCellFactory.hpp"
#include "Exception.hpp"

template <int DIM>
UterineRegularCellFactory<DIM>::UterineRegularCellFactory() :
  AbstractUterineCellFactoryTemplate<DIM>(),
  mpStimulus(new RegularStimulus(0.0, 0.0, 0.1, 0.0)) {
    if (DIM == 2) {
      this->ReadParams(USMC_SYSTEM_CONSTANTS::GENERAL_2D_PARAM_FILE);
    } else if (DIM == 3) {
      this->ReadParams(USMC_SYSTEM_CONSTANTS::GENERAL_3D_PARAM_FILE);
    } else {
      const std::string err_msg = "Invalid dimension";
      const std::string err_filename = "AbstractUterineCellFactoryTemplate.tpp";
      unsigned line_number = 17;
      throw Exception(err_msg, err_filename, line_number);
    }
    this->ReadCellParams(AbstractUterineCellFactoryTemplate<DIM>::GetCellParamFile());
}


template <int DIM>
AbstractCvodeCell* UterineRegularCellFactory<DIM>::CreateCardiacCellForTissueNode(
  Node<DIM>* pNode) {
  double x = pNode->rGetLocation()[0];
  double y = pNode->rGetLocation()[1];
  double z;

  if (DIM == 3) {
    z = pNode->rGetLocation()[2];
  }

  AbstractCvodeCell* cell;

  if (x >= mpX_stim_start && x <= mpX_stim_end &&
      y >= mpY_stim_start && y <= mpY_stim_end) {
    if ((DIM == 3 && z >= mpZ_stim_start && z <= mpZ_stim_end) || (DIM == 2)) {
      AbstractUterineCellFactoryTemplate<DIM>::InitCell(cell, this->mpStimulus);
      AbstractUterineCellFactoryTemplate<DIM>::SetCellParams(cell);
    }
    return cell;

  } else {
    /* The other cells have zero stimuli. */
    return AbstractUterineCellFactoryTemplate<DIM>::CreateCardiacCellForTissueNode(pNode);
  }
}


template <int DIM>
void UterineRegularCellFactory<DIM>::ReadParams(std::string general_param_file) {
  AbstractUterineCellFactoryTemplate<DIM>::ReadParams(general_param_file);

  std::string general_param_path = USMC_SYSTEM_CONSTANTS::CONFIG_DIR +
    general_param_file;
  const auto params = toml::parse(general_param_path);

  // Stimulus location parameters
  mpX_stim_start = toml::find<double>(params, "x_stim_start");
  mpX_stim_end = toml::find<double>(params, "x_stim_end");
  mpY_stim_start = toml::find<double>(params, "y_stim_start");
  mpY_stim_end = toml::find<double>(params, "y_stim_end");

  if (DIM == 3) {
    mpZ_stim_start = toml::find<double>(params, "z_stim_start");
    mpZ_stim_end = toml::find<double>(params, "z_stim_end");
  } else {
    mpZ_stim_start = -1;
    mpZ_stim_end = 0;
  }
}


template <int DIM>
void UterineRegularCellFactory<DIM>::ReadCellParams(std::string cell_param_file) {
  std::string cell_param_path = USMC_SYSTEM_CONSTANTS::CONFIG_DIR +
    cell_param_file;
  const auto cell_params = toml::parse(cell_param_path);

  // Stimulus parameters
  mpStimulus->SetMagnitude(toml::find<double>(cell_params, "magnitude"));
  mpStimulus->SetPeriod(toml::find<double>(cell_params, "period"));
  mpStimulus->SetDuration(toml::find<double>(cell_params, "duration"));
  mpStimulus->SetStartTime(toml::find<double>(cell_params, "start_time"));
}


template <int DIM>
void UterineRegularCellFactory<DIM>::PrintParams() {
  AbstractUterineCellFactoryTemplate<DIM>::PrintParams();
  std::cout << "mpX_stim_start = " << mpX_stim_start << "\n";
  std::cout << "mpX_stim_end = " << mpX_stim_end << "\n";
  std::cout << "mpY_stim_start = " << mpY_stim_start << "\n";
  std::cout << "mpY_stim_end = " << mpY_stim_end << "\n";

  if (DIM == 3) {
    std::cout << "mpZ_stim_start = " << mpZ_stim_start << "\n";
    std::cout << "mpZ_stim_end = " << mpZ_stim_end << "\n";
  }

  std::cout << "mpStimulus magnitude = "
    << mpStimulus->GetMagnitude()
    << std::endl;
  std::cout << "mpStimulus period = " << mpStimulus->GetPeriod() << std::endl;
  std::cout << "mpStimulus duration = "
    << mpStimulus->GetDuration()
    << std::endl;
  std::cout << "mpStimulus start time = " << mpStimulus->GetStartTime() << std::endl;
}


template <int DIM>
void UterineRegularCellFactory<DIM>::WriteLogInfo(std::string log_file) {
  AbstractUterineCellFactoryTemplate<DIM>::WriteLogInfo(log_file);

  std::ofstream log_stream;
  log_stream.open(log_file, ios::app);  // Open log file in append mode

  log_stream << "Stimulus parameters" << std::endl;
  log_stream << "  type: regular" << std::endl;
  log_stream << "  start time: "
    << mpStimulus->GetStartTime()
    << " ms"
    << std::endl;
  log_stream << "  duration: "
    << mpStimulus->GetDuration()
    << " ms"
    << std::endl;
  log_stream << "  magnitude: "
    << mpStimulus->GetMagnitude()
    << " uA/cm2"
    << std::endl;
  log_stream << "  period: " << mpStimulus->GetPeriod() << " ms" << std::endl;
  log_stream << "  stimulated region: " << mpX_stim_start << " <= x <= ";
  log_stream << mpX_stim_end << "   " << mpY_stim_start << " <= y <= ";

  if (DIM == 2) {
    log_stream << mpY_stim_end << std::endl;
  } else {
    log_stream << mpY_stim_end << "   " << mpZ_stim_start << " <= z <= ";
    log_stream << mpZ_stim_end << std::endl;
  }

  log_stream.close();
}
