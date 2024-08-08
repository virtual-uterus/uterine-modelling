#include "simulations.hpp"

void simulation_0d()
{
	FHNOdeSystem ode;
	EulerIvpOdeSolver euler_solver;

	const auto sysParams = toml::parse(
		FHN_ODE_SYSTEM_CONSTANTS::CONFIG_PATH);

	auto initial_conditions = toml::find<std::vector<double>>(
		sysParams, "initial_conditions");

	const double start_time = toml::find<double>(sysParams, "start_time");
	const double end_time = toml::find<double>(sysParams, "end_time");
	const double timestep = toml::find<double>(sysParams, "timestep");
	const double sampling_step = toml::find<double>(sysParams, 
		"sampling_step");

	std::cout << "Running 0D simulation..." << std::endl;

	OdeSolution solution = euler_solver.Solve(&ode, initial_conditions, 
		start_time, end_time, timestep, sampling_step);

	solution.WriteToFile("0d_simulation", "ode_solution", "sec");
}


void simulation_2d()
{
	// Get parameters from config file
	std::string param_file = USMC_2D_SYSTEM_CONSTANTS::CONFIG_DIR + 
		USMC_2D_SYSTEM_CONSTANTS::GENERAL_PARAM_FILE;
	const auto sys_params = toml::parse(param_file);

	// Time constants
	const double sim_duration = toml::find<double>(sys_params, "sim_duration");
	const double ode_timestep = toml::find<double>(sys_params, "ode_timestep");
	const double pde_timestep = toml::find<double>(sys_params, "pde_timestep");
	const double print_timestep = toml::find<double>(sys_params, 
		"print_timestep");

	const std::string mesh_dir = getenv("CHASTE_SOURCE_DIR") + 
		toml::find<std::string>(sys_params, "mesh_dir");
	const std::string mesh_name = toml::find<std::string>(sys_params,
		"mesh_name");
	const std::string cell_type = toml::find<std::string>(sys_params, 
		"cell_type");
	const std::string save_dir = toml::find<std::string>(sys_params,
		"save_dir"); // Top folder to save results
	const std::string stimulus_type = toml::find<std::string>(sys_params,
		"stimulus_type"); // Regular, simple or zero stimulus

	std::string cell_param_file = USMC_3D_SYSTEM_CONSTANTS::CONFIG_DIR + 
		cell_type + ".toml";
	const auto cell_params = toml::parse(cell_param_file);

	// Cell parameters
	auto conductivities = toml::find<std::vector<double>>(
		cell_params, "conductivities_2d");	
	const double capacitance = toml::find<double>(cell_params, "capacitance");
	
	std::string default_ionic_model = cell_type + "I";
	std::string save_path = save_dir + "/" + cell_type + "/" + stimulus_type;

	// Log file location
	std::string log_dir = save_dir + "/" + cell_type + "/log";
	OutputFileHandler output_file_handler(log_dir, false);
	std::string log_path = output_file_handler.GetOutputDirectoryFullPath() + "log.log";

	HeartConfig::Instance()->SetSimulationDuration(sim_duration); //ms
	HeartConfig::Instance()->SetMeshFileName(mesh_dir + mesh_name);
	HeartConfig::Instance()->SetOutputDirectory(save_path);
	HeartConfig::Instance()->SetOutputFilenamePrefix("results");

	HeartConfig::Instance()->SetVisualizeWithVtk(true);

	AbstractUterineCellFactory *factory = NULL;

	if (stimulus_type == "simple")
	{
		factory = new UterineSimpleCellFactory();
	}

	else if (stimulus_type == "regular")
	{
		factory = new UterineRegularCellFactory();
	}

	else if (stimulus_type == "zero")
	{
		factory = new UterineZeroCellFactory();
	}

	else
	{
		const std::string err_message = "Unrecognized stimulus type";
		const std::string err_filename = "simulations.cpp";
		unsigned line_number = 96;

		throw Exception(err_message, err_filename, line_number);
	}

	MonodomainProblem<2> monodomain_problem(factory);

	HeartConfig::Instance()->SetIntracellularConductivities(Create_c_vector(
		conductivities[0], conductivities[1]));

	// HeartConfig::Instance()->SetSurfaceAreaToVolumeRatio(1400); // 1/cm
	HeartConfig::Instance()->SetCapacitance(capacitance); // uF/cm^2

	HeartConfig::Instance()->SetOdePdeAndPrintingTimeSteps(ode_timestep, 
		pde_timestep, print_timestep);
	HeartConfig::Instance()->SetDefaultIonicModel(default_ionic_model);

	// Print information on the simulation to screen
	std::cout << "Running 2D simulation..." << std::endl;

	std::cout << "\nStimulus type: " << stimulus_type << std::endl;
	std::cout << "\nuSMC cell factory parameters:\n" << std::endl;
	factory->PrintParams();
	std::cout << "x axis conductivity = " << conductivities[0] << std::endl;
	std::cout << "y axis conductivity = " << conductivities[1] << std::endl;

	std::cout << std::endl;

	monodomain_problem.Initialise();
	monodomain_problem.Solve();

	HeartEventHandler::Headings();
	HeartEventHandler::Report();

	// Write to log file
	std::ofstream log_stream;
	log_stream.open(log_path, ios::app);

	log_stream << "System information" << std::endl;
	log_stream << "  cell type: " <<  cell_type << std::endl;
	log_stream << "  mesh: " << mesh_name << std::endl;
	log_stream << "  capacitance: " << capacitance << " uF/cm2" << std::endl;
	log_stream << "  conductivity x axis: " << conductivities[0] << std::endl;
	log_stream << "  conductivity y axis: " << conductivities[1] << std::endl;

	log_stream << "Simulation parameters" << std::endl;
	log_stream << "  duration: " << sim_duration << " ms" << std::endl;
	log_stream << "  ode timestep: " << ode_timestep << " ms" << std::endl;
	log_stream << "  pde timestep: " << pde_timestep << " ms" << std::endl;
	log_stream << "  print timestep: " << print_timestep << " ms" << std::endl;

	log_stream.close();

	factory->WriteLogInfo(log_path);
}


void simulation_3d()
{
	// Get parameters from config file
	std::string param_file = USMC_3D_SYSTEM_CONSTANTS::CONFIG_DIR + 
		USMC_3D_SYSTEM_CONSTANTS::GENERAL_PARAM_FILE;
		
	const auto sys_params = toml::parse(param_file);

	// Time constants
	const double sim_duration = toml::find<double>(sys_params, "sim_duration");
	const double ode_timestep = toml::find<double>(sys_params, "ode_timestep");
	const double pde_timestep = toml::find<double>(sys_params, "pde_timestep");
	const double print_timestep = toml::find<double>(sys_params, 
		"print_timestep");

	const std::string mesh_dir = getenv("CHASTE_SOURCE_DIR") + 
		toml::find<std::string>(sys_params, "mesh_dir");
	const std::string mesh_name = toml::find<std::string>(sys_params,
		"mesh_name");
	const std::string cell_type = toml::find<std::string>(sys_params, 
		"cell_type");
	const std::string save_dir = toml::find<std::string>(sys_params,
		"save_dir"); // Top folder to save results
	const std::string stimulus_type = toml::find<std::string>(sys_params,
		"stimulus_type"); // Regular, simple or zero stimulus

	std::string cell_param_file = USMC_3D_SYSTEM_CONSTANTS::CONFIG_DIR + 
		cell_type + ".toml";
	const auto cell_params = toml::parse(cell_param_file);

	// Cell parameters
	auto conductivities = toml::find<std::vector<double>>(
		cell_params, "conductivities_3d");	
	const double capacitance = toml::find<double>(cell_params, "capacitance");
	
	std::string default_ionic_model = cell_type + "I";
	std::string save_path = save_dir + "/" + cell_type + "/" + stimulus_type;

	// Log file location
	std::string log_dir = save_dir + "/" + cell_type + "/log";
	OutputFileHandler output_file_handler(log_dir, false);
	std::string log_path = output_file_handler.GetOutputDirectoryFullPath() + "log.log";

	HeartConfig::Instance()->SetSimulationDuration(sim_duration); //ms
	HeartConfig::Instance()->SetMeshFileName(mesh_dir + mesh_name);
	HeartConfig::Instance()->SetOutputDirectory(save_path);
	HeartConfig::Instance()->SetOutputFilenamePrefix("results");

	HeartConfig::Instance()->SetVisualizeWithVtk(true);

	AbstractUterineCellFactory3d *factory = NULL;

	if (stimulus_type == "simple")
	{
		factory = new UterineSimpleCellFactory3d();
	}

	else if (stimulus_type == "regular")
	{
		factory = new UterineRegularCellFactory3d();
	}

	else if (stimulus_type == "zero")
	{
		factory = new UterineZeroCellFactory3d();
	}

	else
	{
		const std::string err_message = "Unrecognized stimulus type";
		const std::string err_filename = "simulations.cpp";
		unsigned line_number = 220;

		throw Exception(err_message, err_filename, line_number);
	}

	MonodomainProblem<3> monodomain_problem(factory);

	HeartConfig::Instance()->SetIntracellularConductivities(Create_c_vector(
		conductivities[0], conductivities[1], conductivities[2]));

	// HeartConfig::Instance()->SetSurfaceAreaToVolumeRatio(1400); // 1/cm
	HeartConfig::Instance()->SetCapacitance(capacitance); // uF/cm^2

	HeartConfig::Instance()->SetOdePdeAndPrintingTimeSteps(ode_timestep, 
		pde_timestep, print_timestep);
	HeartConfig::Instance()->SetDefaultIonicModel(default_ionic_model);

	// Print information on the simulation to screen
	std::cout << "Running 3D simulation..." << std::endl;

	std::cout << "\nStimulus type: " << stimulus_type << std::endl;
	std::cout << "\nuSMC cell factory parameters:\n" << std::endl;
	factory->PrintParams();
	std::cout << "x axis conductivity = " << conductivities[0] << std::endl;
	std::cout << "y axis conductivity = " << conductivities[1] << std::endl;
	std::cout << "z axis conductivity = " << conductivities[2] << std::endl;

	std::cout << std::endl;

	monodomain_problem.Initialise();
	monodomain_problem.Solve();

	HeartEventHandler::Headings();
	HeartEventHandler::Report();

	// Write to log file
	std::ofstream log_stream;
	log_stream.open(log_path, ios::app);

	log_stream << "System information" << std::endl;
	log_stream << "  cell type: " <<  cell_type << std::endl;
	log_stream << "  mesh: " << mesh_name << std::endl;
	log_stream << "  capacitance: " << capacitance << " uF/cm2" << std::endl;
	log_stream << "  conductivity x axis: " << conductivities[0] << std::endl;
	log_stream << "  conductivity y axis: " << conductivities[1] << std::endl;
	log_stream << "  conductivity z axis: " << conductivities[2] << std::endl;

	log_stream << "Simulation parameters" << std::endl;
	log_stream << "  duration: " << sim_duration << " ms" << std::endl;
	log_stream << "  ode timestep: " << ode_timestep << " ms" << std::endl;
	log_stream << "  pde timestep: " << pde_timestep << " ms" << std::endl;
	log_stream << "  print timestep: " << print_timestep << " ms" << std::endl;

	log_stream.close();

	factory->WriteLogInfo(log_path);
}
