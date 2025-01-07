#include "../../include/factories/UterineRegionCellFactory3d.hpp"
#include "Exception.hpp"

UterineRegionCellFactory3d::UterineRegionCellFactory3d() :
  AbstractUterineCellFactory3d() {
  boost::shared_ptr<UterineRegionSelector> selector(
    new UterineRegionSelector());
  mpOvariesStimulus = boost::make_shared<UterineRegionStimulus>(
    0.0, 0.0, 1.0, 0.0, selector);
  mpCentreStimulus = boost::make_shared<UterineRegionStimulus>(
    0.0, 0.0, 1.0, 0.0, selector);
  mpCervicalStimulus = boost::make_shared<UterineRegionStimulus>(
    0.0, 0.0, 1.0, 0.0, selector);
  ReadParams(USMC_3D_SYSTEM_CONSTANTS::GENERAL_PARAM_FILE);
  ReadCellParams(AbstractUterineCellFactory3d::GetCellParamFile());
}


AbstractCvodeCell* UterineRegionCellFactory3d::CreateCardiacCellForTissueNode(
  Node<3>* pNode) {
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


unsigned UterineRegionCellFactory3d::FindRegion(double x, double y, double z) {
  unsigned region(0);  // Default to no region

  region = IsInLeft(x, y, z);
  if (region == 0) {  // Check right region only if not in left
    region = IsInRight(x, y, z);
  } else {
    return region;
  }

  return region;
}


unsigned UterineRegionCellFactory3d::IsInLeft(double x, double y, double z) {
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


unsigned UterineRegionCellFactory3d::IsInRight(double x, double y, double z) {
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


void UterineRegionCellFactory3d::ReadParams(std::string general_param_file) {
  AbstractUterineCellFactory3d::ReadParams(general_param_file);

  // Read region stimulus specific parameters
  std::string general_param_path = USMC_3D_SYSTEM_CONSTANTS::CONFIG_DIR +
    general_param_file;
  const auto params = toml::parse(general_param_path);
  std::string mesh_name = toml::find<std::string>(
    params, "mesh_name");  // Get mesh name

  // Get mesh stimulus locations
  const auto mesh_param_file = "mesh/" + mesh_name + ".toml";

  ReadMeshParams(mesh_param_file, "left");
  ReadMeshParams(mesh_param_file, "right");
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


void UterineRegionCellFactory3d::ReadMeshParams(
  std::string mesh_param_file, std::string horn) {
  std::string mesh_param_path = USMC_3D_SYSTEM_CONSTANTS::CONFIG_DIR +
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
      throw Exception("Incorrect horn", "UterineRegionCellFactory3d.cpp", 196);
    }
  }
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


void UterineRegionCellFactory3d::WriteLogInfo(std::string log_file) {
  AbstractUterineCellFactory3d::WriteLogInfo(log_file);

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


