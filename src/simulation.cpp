#include "../include/simulation.hpp"

void run_simulation(const int dim) {
  // Get parameters from config file
  std::string param_file = USMC_SYSTEM_CONSTANTS::CONFIG_DIR;

  if (dim == 2) {
    param_file += USMC_SYSTEM_CONSTANTS::GENERAL_2D_PARAM_FILE;
  } else {  // Dimension is 3
    param_file += USMC_SYSTEM_CONSTANTS::GENERAL_3D_PARAM_FILE;
  }

  const auto sys_params = toml::parse(param_file);

  // Time constants
  const double sim_duration = toml::find<double>(sys_params, "sim_duration");
  const double ode_timestep = toml::find<double>(sys_params, "ode_timestep");
  const double pde_timestep = toml::find<double>(sys_params, "pde_timestep");
  const double print_timestep = toml::find<double>(sys_params,
    "print_timestep");
  const bool orthotropic = toml::find<bool>(sys_params, "orthotropic");

  const std::string mesh_dir = getenv("CHASTE_SOURCE_DIR") +
    toml::find<std::string>(sys_params, "mesh_dir");
  const std::string mesh_name = toml::find<std::string>(sys_params,
    "mesh_name");
  const std::string cell_type = toml::find<std::string>(sys_params,
    "cell_type");
  const std::string save_dir = toml::find<std::string>(sys_params,
    "save_dir");  // Top folder to save results
  const std::string stimulus_type = toml::find<std::string>(sys_params,
    "stimulus_type");  // Regular, simple or zero stimulus

  std::string cell_param_file = "";

  if (cell_type == std::string("Roesler")) {
    // Get the estrus phase as well
    const std::string estrus_phase = toml::find<std::string>(sys_params,
      "estrus");
    cell_param_file = USMC_SYSTEM_CONSTANTS::CONFIG_DIR +
      "estrus/" + cell_type + "_" + estrus_phase + ".toml";
  } else {
    cell_param_file = USMC_SYSTEM_CONSTANTS::CONFIG_DIR +
      "cell/" + cell_type + ".toml";
  }

  const auto cell_params = toml::parse(cell_param_file);
  std::vector<double> conductivities;

  // Cell parameters
  if (orthotropic) {
    // If orthotropic extract the correct conductivities
    conductivities = toml::find<std::vector<double>>(
      cell_params, "ortho_conductivities");
    HeartConfig::Instance()->SetMeshFileName(mesh_dir + mesh_name,
                                             cp::media_type::Orthotropic);
  } else if (dim == 2) {  // If 2D extract x, y conductivities
    conductivities = toml::find<std::vector<double>>(
      cell_params, "conductivities_2d");
    HeartConfig::Instance()->SetMeshFileName(mesh_dir + mesh_name);
  } else if (dim == 3) {  // If 3D extract x, y, z conductivities
    conductivities = toml::find<std::vector<double>>(
      cell_params, "conductivities_3d");
    HeartConfig::Instance()->SetMeshFileName(mesh_dir + mesh_name);
  }
  const double capacitance = toml::find<double>(cell_params, "capacitance");

  std::string save_path = cell_type + "/" + save_dir + "/" + stimulus_type;

  // Log file location
  std::string log_dir = cell_type + "/" + save_dir + "/log";
  OutputFileHandler output_file_handler(log_dir, false);
  std::string log_path =
    output_file_handler.GetOutputDirectoryFullPath() + "log.log";

  HeartConfig::Instance()->SetSimulationDuration(sim_duration);  // ms
  HeartConfig::Instance()->SetOutputDirectory(save_path);
  HeartConfig::Instance()->SetOutputFilenamePrefix("results");

  HeartConfig::Instance()->SetVisualizeWithVtk(true);

  if (dim == 2) {
    HeartConfig::Instance()->SetIntracellularConductivities(Create_c_vector(
      conductivities[0], conductivities[1]));
  } else {  // In 3D
    HeartConfig::Instance()->SetIntracellularConductivities(Create_c_vector(
      conductivities[0], conductivities[1], conductivities[2]));
  }

  HeartConfig::Instance()->SetSurfaceAreaToVolumeRatio(7420);  // 1/cm
  HeartConfig::Instance()->SetCapacitance(capacitance);  // uF/cm^2

  HeartConfig::Instance()->SetOdePdeAndPrintingTimeSteps(ode_timestep,
    pde_timestep, print_timestep);

  // Print information on the simulation to screen
  std::cout << "Running " << dim << "D simulation..." << std::endl;

  std::cout << "\nStimulus type: " << stimulus_type << std::endl;

  // Write to log file
  std::ofstream log_stream;
  log_stream.open(log_path, ios::app);

  log_stream << "System information" << std::endl;
  log_stream << "  cell type: " <<  cell_type << std::endl;
  log_stream << "  mesh: " << mesh_name << std::endl;
  log_stream << "  capacitance: " << capacitance << " uF/cm2" << std::endl;
  if (orthotropic) {
    log_stream << "  fibre conductivity = " << conductivities[0] << std::endl;
    log_stream << "  sheet conductivity = " << conductivities[1] << std::endl;
    log_stream << "  normal conductivity = " << conductivities[2] << std::endl;
  } else {  // If not orthotropic
    log_stream << "  x axis conductivity = " << conductivities[0] << std::endl;
    log_stream << "  y axis conductivity = " << conductivities[1] << std::endl;

    if (dim == 3) {
      log_stream << "  z axis conductivity = " << conductivities[2] << std::endl;
    }
  }

  log_stream << "Simulation parameters" << std::endl;
  log_stream << "  duration: " << sim_duration << " ms" << std::endl;
  log_stream << "  ode timestep: " << ode_timestep << " ms" << std::endl;
  log_stream << "  pde timestep: " << pde_timestep << " ms" << std::endl;
  log_stream << "  print timestep: " << print_timestep << " ms" << std::endl;

  log_stream.close();

  if (dim == 2) {
    simulation_2d(stimulus_type, log_path);
  } else if (dim == 3) {
    simulation_3d(stimulus_type, log_path);
  } else {
    const std::string err_msg = "Invalid dimension";
    const std::string err_filename = "main.cpp";
    unsigned line_number = 110;

  throw Exception(err_msg, err_filename, line_number);
  }

  HeartEventHandler::Headings();
  HeartEventHandler::Report();
}


void simulation_2d(std::string stimulus_type, std::string log_path) {
  constexpr int DIM = 2;

  AbstractUterineCellFactoryTemplate<DIM> *factory = NULL;

  if (stimulus_type == "simple") {
    factory = new UterineSimpleCellFactory<DIM>();
  } else if (stimulus_type == "regular") {
    factory = new UterineRegularCellFactory<DIM>();
  } else if (stimulus_type == "region") {
    factory = new UterineRegionCellFactory<DIM>();
  } else if (stimulus_type == "zero") {
    factory = new UterineZeroCellFactory<DIM>();
  } else {
    const std::string err_message = "Unrecognized stimulus type";
    const std::string err_filename = "simulation.cpp";
    unsigned line_number = 163;

    throw Exception(err_message, err_filename, line_number);
  }

  factory->WriteLogInfo(log_path);

  MonodomainProblem<DIM> monodomain_problem(factory);

  monodomain_problem.Initialise();
  monodomain_problem.Solve();
}


void simulation_3d(std::string stimulus_type, std::string log_path) {
  constexpr int DIM = 3;

  AbstractUterineCellFactoryTemplate<DIM> *factory = NULL;

  if (stimulus_type == "simple") {
    factory = new UterineSimpleCellFactory<DIM>();
  } else if (stimulus_type == "regular") {
    factory = new UterineRegularCellFactory<DIM>();
  } else if (stimulus_type == "region") {
    factory = new UterineRegionCellFactory<DIM>();
  } else if (stimulus_type == "zero") {
    factory = new UterineZeroCellFactory<DIM>();
  } else {
    const std::string err_message = "Unrecognized stimulus type";
    const std::string err_filename = "simulation.cpp";
    unsigned line_number = 195;

    throw Exception(err_message, err_filename, line_number);
  }
  factory->WriteLogInfo(log_path);

  MonodomainProblem<DIM> monodomain_problem(factory);

  monodomain_problem.Initialise();
  monodomain_problem.Solve();
}
