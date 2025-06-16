#include "../../include/factories/UterineRegionCellFactory.hpp"
#include "Exception.hpp"

template <int DIM>
UterineRegionCellFactory<DIM>::UterineRegionCellFactory() :
  AbstractUterineCellFactoryTemplate<DIM>() {
    boost::shared_ptr<UterineRegionSelector> selector(
      new UterineRegionSelector());
    mpOvariesStimulus = boost::make_shared<UterineRegionStimulus>(
      0.0, 0.0, 1.0, 0.0, selector);
    mpCentreStimulus = boost::make_shared<UterineRegionStimulus>(
      0.0, 0.0, 1.0, 0.0, selector);
    mpCervicalStimulus = boost::make_shared<UterineRegionStimulus>(
      0.0, 0.0, 1.0, 0.0, selector);

    if (DIM == 2) {
      this->ReadParams(USMC_SYSTEM_CONSTANTS::GENERAL_2D_PARAM_FILE);
    } else if (DIM == 3) {
      this->ReadParams(USMC_SYSTEM_CONSTANTS::GENERAL_3D_PARAM_FILE);
    } else {
      const std::string err_msg = "Invalid dimension";
      const std::string err_filename = "AbstractUterineCellFactoryTemplate.tpp";
      unsigned line_number = 23;
      throw Exception(err_msg, err_filename, line_number);
    }

    this->ReadCellParams(this->GetCellParamFile());
}

template <int DIM>
AbstractCvodeCell* UterineRegionCellFactory<DIM>::CreateCardiacCellForTissueNode(
  Node<DIM>* pNode) {
  double x = pNode->rGetLocation()[0];
  double y = pNode->rGetLocation()[1];
  double z = pNode->rGetLocation()[2];

  unsigned region = FindRegion(x, y, z);
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
      return this->CreateCardiacCellForTissueNode(
        pNode);
  }

  stimulus->SetRegion(region);

  AbstractCvodeCell* cell;

  this->InitCell(cell, stimulus);
  this->SetCellParams(cell);
  return cell;
}


template <int DIM>
unsigned UterineRegionCellFactory<DIM>::FindRegion(double x, double y, double z) {
  unsigned region(0);  // Default to no region

  region = IsInLeft(x, y, z);
  if (region == 0) {  // Check right region only if not in left
    region = IsInRight(x, y, z);
  } else {
    return region;
  }

  return region;
}


template <int DIM>
unsigned UterineRegionCellFactory<DIM>::IsInLeft(double x, double y, double z) {
  unsigned region = 0;

  if (x <= mpXStimLeft[1] && x >= mpXStimLeft[0] &&
      y <= mpYStimLeft[1] && y >= mpYStimLeft[0]) {
    // In the correct x-y plane for stimulus
    if (z <= mpZStimLeft[0][1] && z >= mpZStimLeft[0][0]) {
      region = 1;  // In the ovaries stimulus region
    } else if (z <= mpZStimLeft[1][1] && z >= mpZStimLeft[1][0]) {
      region = 2;  // In the centre stimulus region
    } else if (z <= mpZStimLeft[2][1] && z >= mpZStimLeft[2][0]) {
      region = 3;  // In the cervical stimulus region
    }
  }
  return region;
}


template <int DIM>
unsigned UterineRegionCellFactory<DIM>::IsInRight(double x, double y, double z) {
  unsigned region = 0;

  if (x <= mpXStimRight[1] && x >= mpXStimRight[0] &&
      y <= mpYStimRight[1] && y >= mpYStimRight[0]) {
    // In the correct x-y plane for stimulus
    if (z <= mpZStimRight[0][1] && z >= mpZStimRight[0][0]) {
      region = 1;  // In the ovaries stimulus region
    } else if (z <= mpZStimRight[1][1] && z >= mpZStimRight[1][0]) {
      region = 2;  // In the centre stimulus region
    } else if (z <= mpZStimRight[2][1] && z >= mpZStimRight[2][0]) {
      region = 3;  // In the cervical stimulus region
    }
  }
  return region;
}


template <int DIM>
void UterineRegionCellFactory<DIM>::ReadParams(std::string general_param_file) {
  this->ReadParams(general_param_file);

  // Read region stimulus specific parameters
  std::string general_param_path = USMC_SYSTEM_CONSTANTS::CONFIG_DIR +
    general_param_file;
  const auto params = toml::parse(general_param_path);
  std::string mesh_name = toml::find<std::string>(
    params, "mesh_name");  // Get mesh name

  // Get mesh stimulus locations
  const auto mesh_param_file = "mesh/" + mesh_name + ".toml";

  ReadMeshParams(mesh_param_file, "left");
  ReadMeshParams(mesh_param_file, "right");
}


template <int DIM>
void UterineRegionCellFactory<DIM>::ReadCellParams(std::string cell_param_file) {
  std::string cell_param_path = USMC_SYSTEM_CONSTANTS::CONFIG_DIR +
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


template <int DIM>
void UterineRegionCellFactory<DIM>::ReadMeshParams(
  std::string mesh_param_file, std::string horn) {
  std::string mesh_param_path = USMC_SYSTEM_CONSTANTS::CONFIG_DIR +
    mesh_param_file;
  auto mesh_params = toml::parse(mesh_param_path);


  // Read horn specific parameters
  if (mesh_params.contains(horn)) {
    double z_start_ova(0.0);
    double z_end_ova(0.0);
    double z_start_cen(0.0);
    double z_end_cen(0.0);
    double z_start_cev(0.0);
    double z_end_cev(0.0);

    std::vector<double> x_stim = {
      toml::find<double>(mesh_params[horn], "x_start"),
      toml::find<double>(mesh_params[horn], "x_end")
    };
    std::vector<double> y_stim = {
      toml::find<double>(mesh_params[horn], "y_start"),
      toml::find<double>(mesh_params[horn], "y_end")
    };

    auto horn_table = mesh_params[horn].as_table();

    // Get ovaries stimulus location
    z_start_ova = toml::find<double>(
      horn_table["ovaries"],
      "z_start");
    z_end_ova = toml::find<double>(
      horn_table["ovaries"],
      "z_end");

    // Get centre stimulus location
    z_start_cen = toml::find<double>(
      horn_table["centre"],
      "z_start");
    z_end_cen = toml::find<double>(
      horn_table["centre"],
      "z_end");

    // Get cervical stimulus location
    z_start_cev = toml::find<double>(
      horn_table["cervical"],
      "z_start");
    z_end_cev = toml::find<double>(
      horn_table["cervical"],
      "z_end");

    if (horn == "left") {
      mpXStimLeft = x_stim;
      mpYStimLeft = y_stim;
      mpZStimLeft = {
        {z_start_ova, z_end_ova},
        {z_start_cen, z_end_cen},
        {z_start_cev, z_end_cev}
      };
    } else if (horn == "right") {
      mpXStimRight = x_stim;
      mpYStimRight = y_stim;
      mpZStimRight = {
        {z_start_ova, z_end_ova},
        {z_start_cen, z_end_cen},
        {z_start_cev, z_end_cev}
      };
    } else {
      throw Exception("Incorrect horn", "UterineRegionCellFactory<DIM>.tpp", 196);
    }
  }
}


template <int DIM>
void UterineRegionCellFactory<DIM>::SetStimulusParams(
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


template <int DIM>
void UterineRegionCellFactory<DIM>::PrintParams() {
  this->PrintParams();
  std::cout << "stimulus magnitude = "
    << mpOvariesStimulus->GetMagnitude()
    << std::endl;
  std::cout << "stimulus period = "
    << mpOvariesStimulus->GetPeriod() << std::endl;
  std::cout << "stimulus duration = "
    << mpOvariesStimulus->GetDuration()
    << std::endl;
  std::cout << "stimulus start time = " << mpOvariesStimulus->GetStartTime() << std::endl;
  std::cout << "    left" << std::endl;
  std::cout << "      " << mpXStimLeft[0] << " <= x <= " <<
    mpXStimLeft[1] << std::endl;
  std::cout << "      " << mpYStimLeft[0] << " <= y <= " <<
    mpYStimLeft[1] << std::endl;
  std::cout << "      ovaries: " << mpZStimLeft[0][0] << " <= z <= " <<
    mpZStimLeft[0][1] << std::endl;
  std::cout << "      centre: " << mpZStimLeft[1][0] << " <= z <= " <<
    mpZStimLeft[1][1] << std::endl;
  std::cout << "      cervical: " << mpZStimLeft[2][0] << " <= z <= " <<
    mpZStimLeft[2][1] << std::endl;
  std::cout << "    right" << std::endl;
  std::cout << "      " << mpXStimRight[0] << " <= x <= " <<
    mpXStimRight[1] << std::endl;
  std::cout << "      " << mpYStimRight[0] << " <= y <= " <<
    mpYStimRight[1] << std::endl;
  std::cout << "      ovaries: " << mpZStimRight[0][0] << " <= z <= " <<
    mpZStimRight[0][1] << std::endl;
  std::cout << "      centre: " << mpZStimRight[1][0] << " <= z <= " <<
    mpZStimRight[1][1] << std::endl;
  std::cout << "      cervical: " << mpZStimRight[2][0] << " <= z <= " <<
    mpZStimRight[2][1] << std::endl;
}


template <int DIM>
void UterineRegionCellFactory<DIM>::WriteLogInfo(std::string log_file) {
  this->WriteLogInfo(log_file);

  std::ofstream log_stream;
  log_stream.open(log_file, ios::app);  // Open log file in append mode

  log_stream << "Stimulus parameters" << std::endl;
  log_stream << "  type: region" << std::endl;
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
  log_stream << "  period: " << mpOvariesStimulus->GetPeriod()
    << " ms" << std::endl;
  log_stream << "  stimulated regions: " << std::endl;
  log_stream << "    left" << std::endl;
  log_stream << "      " << mpXStimLeft[0] << " <= x <= " <<
    mpXStimLeft[1] << std::endl;
  log_stream << "      " << mpYStimLeft[0] << " <= y <= " <<
    mpYStimLeft[1] << std::endl;
  log_stream << "      ovaries: " << mpZStimLeft[0][0] << " <= z <= " <<
    mpZStimLeft[0][1] << std::endl;
  log_stream << "      centre: " << mpZStimLeft[1][0] << " <= z <= " <<
    mpZStimLeft[1][1] << std::endl;
  log_stream << "      cervical: " << mpZStimLeft[2][0] << " <= z <= " <<
    mpZStimLeft[2][1] << std::endl;
  log_stream << "    right" << std::endl;
  log_stream << "      " << mpXStimRight[0] << " <= x <= " <<
    mpXStimRight[1] << std::endl;
  log_stream << "      " << mpYStimRight[0] << " <= y <= " <<
    mpYStimRight[1] << std::endl;
  log_stream << "      ovaries: " << mpZStimRight[0][0] << " <= z <= " <<
    mpZStimRight[0][1] << std::endl;
  log_stream << "      centre: " << mpZStimRight[1][0] << " <= z <= " <<
    mpZStimRight[1][1] << std::endl;
  log_stream << "      cervical: " << mpZStimRight[2][0] << " <= z <= " <<
    mpZStimRight[2][1] << std::endl;

  log_stream.close();
}


