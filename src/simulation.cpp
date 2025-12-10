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

  if (cell_type == std::string("Roesler") || cell_type == std::string("RoeslerP")) {
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

  //tissue conductivity modifier
  //create tissue modifier
	UterineTissueModifier modifier;
   if (cell_params.contains("tissuemod")) {
    for (const auto& [key, value] : toml::find<toml::value>(
      cell_params, "tissuemod").as_table()) {
        
        if (value.is_floating()) {
            std::cout << "(simulation.cpp) reading key value: " << key << std::endl;
            if (key == "conduct_mod_center") {
              modifier.mCentre = toml::get<double>(value);
              std::cout << "     value: " << toml::get<double>(value) << std::endl;
            }
            else if (key == "conduct_mod_steep") {
              modifier.mSteep = toml::get<double>(value);;
              std::cout << "     value: " << toml::get<double>(value) << std::endl;
            }
            else if (key == "conduct_mod_min") {
              modifier.mBaseline = toml::get<double>(value);;
              std::cout << "     value: " << toml::get<double>(value) << std::endl;
            }
            else if (key == "conduct_mod_amplitude") {
              modifier.mAmplitude = toml::get<double>(value);;
              std::cout << "     value: " << toml::get<double>(value) << std::endl;
            }
            //modifier.mpTissue_parameters[key] = toml::get<double>(value);
        } else if (key == "conductivity_mod_type") {
            modifier.mpTissue_dist = toml::get<std::string>(value);
        }
    }
  } 

  //std::cout << "(simulation.cpp) tissue conductivity mod type: " << modifier.mpTissue_dist << std::endl;

  //load these read parameters into the tissuemodifier 
  //modifier.TissueModLoadParms();

	/* const std::string cond_modifier_type = toml::find<std::string>(cell_params,
		"conductivity_mod_type");

  //load tissue conductivity modifier parms
	//attempt parameter array 
	std::vector<double> cond_mod_parms;

	//load tissue modifier parms
  if (cond_modifier_type == "none"){
    std::cout << "(simulation.cpp) no tissue modifier" << std::endl;
  }
	else if (cond_modifier_type == "linear"){
		//compiler wants these declared and init before type check...?! */
	/* 	double conduct_mod_slope = -1;
		double conduct_mod_max = -1; */		
/*    std::cout << "(simulation.cpp) linear tissue modifier" << std::endl;
		cond_mod_parms.push_back(toml::find<double>(cell_params, "conduct_mod_slope"));
		cond_mod_parms.push_back(toml::find<double>(cell_params, "conduct_mod_max"));
	}

	else if (cond_modifier_type == "gaussian"){
    std::cout << "(simulation.cpp) gaussian tissue modifier" << std::endl;
		cond_mod_parms.push_back(toml::find<double>(cell_params, "conduct_mod_center"));
		cond_mod_parms.push_back(toml::find<double>(cell_params, "conduct_mod_steep"));
		cond_mod_parms.push_back(toml::find<double>(cell_params, "conduct_mod_min"));
		
	}

  else {
    std::cout << "(simulation.cpp) tissue modifer type unrecognised" << std::endl;

    const std::string err_message = "Unrecognized tissue modifer type";
    const std::string err_filename = "simulation.cpp";
    unsigned line_number = 97;

    throw Exception(err_message, err_filename, line_number);    

  } */

  const double capacitance = toml::find<double>(cell_params, "capacitance");
  const std::string output_dir = getenv("CHASTE_TEST_OUTPUT");
  //below all from effort tidy up output directories
  /* //search for 'testoutput' and append following directory string
  //std::string teststr = "testoutput";
  std::string teststr = cell_type;
  //std::string output_dir_post = output_dir.substr((output_dir.find(teststr,0)+teststr.length),output_dir.length);
  std::string output_dir_post = output_dir.substr(output_dir.find(teststr,0)+teststr.length()+1,output_dir.length());
  //std::cout << "(simulation.cpp) found: " << output_dir_post << std::endl;
  //std::string save_path = output_dir_post + "/" + cell_type + "/" + save_dir + "/" + stimulus_type; */
  std::string save_path = cell_type + "/" + save_dir + "/" + stimulus_type;

  std::cout << "(simulation.cpp) save_path: " << save_path << std::endl;

  //below issue chaste overwriting results directory now handled by NOT setting 'save_path' for output directory below
  // and in uterine-simulation script for vtk conversion script
  //test protecting the output directory from deletion, the default behaviour noted here
  //./global/src/OutputFileHandler.cpp:149
  //OutputFileHandler save_dir_file_handler(save_path, false);

  // Log file location
  std::string log_dir = cell_type + "/" + save_dir + "/log";
  //std::string log_dir = output_dir_post + "/" + cell_type + "/" + save_dir + "/log";
  //std::string log_dir = cell_type + "/" + save_dir + "/" + stimulus_type + "/log";

  std::cout << "(simulation.cpp) log_dir: " << log_dir << std::endl;
  std::cout << std::endl;

  OutputFileHandler output_file_handler(log_dir, false);
  std::string log_path =
    output_file_handler.GetOutputDirectoryFullPath() + "log.log";

  HeartConfig::Instance()->SetSimulationDuration(sim_duration);  // ms
  //below commented out: chaste deletes this results folder! instead now use default and handle
  //moving vtk output in control script uterine-simulation
  //HeartConfig::Instance()->SetOutputDirectory(save_path);
  //HeartConfig::Instance()->SetOutputDirectory("./uterine-modeling");
  //test the set directory -- these must be relative?
  //std::cout << "(simulation.cpp) GetOutputDirectory set: " << HeartConfig::Instance()->GetOutputDirectory() << std::endl;
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
  std::cout << "System information" << std::endl;
  std::cout << "  cell type: " <<  cell_type << std::endl;
  std::cout << "  mesh: " << mesh_name << std::endl;
  std::cout << "  stimulus type: " << stimulus_type << std::endl;
  std::cout << "  duration: " << sim_duration << " ms" << std::endl;

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
  if (cell_params.contains("tissuemod")) {
    log_stream << "  conductivity modifier type = " << modifier.mpTissue_dist << std::endl;
    log_stream << "  conduct_mod_center = " << modifier.mCentre << std::endl;
    log_stream << "  conduct_mod_steep = " << modifier.mSteep << std::endl;
    log_stream << "  conduct_mod_min = " << modifier.mBaseline << std::endl;
    log_stream << "  conduct_mod_amplitude = " << modifier.mAmplitude << std::endl;

  }
  
  //log_stream << "  conductivity modfier type = " << cond_modifier_type << std::endl;


  log_stream << "Simulation parameters" << std::endl;
  log_stream << "  duration: " << sim_duration << " ms" << std::endl;
  log_stream << "  ode timestep: " << ode_timestep << " ms" << std::endl;
  log_stream << "  pde timestep: " << pde_timestep << " ms" << std::endl;
  log_stream << "  print timestep: " << print_timestep << " ms" << std::endl;

  log_stream.close();

  if (dim == 2) {
    simulation_2d(stimulus_type, log_path);
  } else if (dim == 3) {
    simulation_3d(stimulus_type, log_path, save_path, modifier);
  } else {
    const std::string err_msg = "Invalid dimension";
    const std::string err_filename = "simulation.cpp";
    unsigned line_number = 140;

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
    unsigned line_number = 166;

    throw Exception(err_message, err_filename, line_number);
  }

  factory->WriteLogInfo(log_path);

  MonodomainProblem<DIM> monodomain_problem(factory);

  monodomain_problem.Initialise();
  monodomain_problem.Solve();
}


void simulation_3d(std::string stimulus_type, std::string log_path, std::string save_path, UterineTissueModifier modifier) {
  // Include passive cell params to input arguments
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
    unsigned line_number = 197;

    throw Exception(err_message, err_filename, line_number);
  }
  factory->WriteLogInfo(log_path);

  MonodomainProblem<DIM> monodomain_problem(factory);

  monodomain_problem.Initialise();

  //with the problem initialised, modify tissue conductivity if set
  //if gaussian or linear tissue mod call routine, else skip
  if (modifier.mpTissue_dist != "none") {
    std::cout << "(simulation.cpp) 3d problem with tissue conductivity modifier type " 
      << modifier.mpTissue_dist << " flagged" << std::endl;

    //get the tissue model
	  MonodomainTissue<3>* p_monodomain_tissue = monodomain_problem.GetMonodomainTissue();

	  //following https://chaste.github.io/docs/user-tutorials/bidomainwithconductivitymodifier/
	  //this does the conductivity modification in the model
	  p_monodomain_tissue->SetConductivityModifier( &modifier );

    //below is for visualisation -- for pulling _out_ the modified conductivities
  	modifier.p_mesh = &(monodomain_problem.rGetMesh());	

	  //for writing out modifier distribution conductivities
    std::cout << "(simulation.cpp) Saving tissue conductivity modifier visual to file " << 
      save_path << "_modifier tissue_conductivity_mod" << std::endl;
	  VtkMeshWriter<3,3> conductivities_mesh_writer(save_path+"_modifier","tissue_conductivity_mod",false);

    //output to a vtk file for visualisation
	  //for mapping over the conductivity tensor to output var
	  c_vector<double, 3> cur_conduct;
	  std::vector< c_vector<double,3> > mod_conductivities;

    for (AbstractTetrahedralMesh<3,3>::ElementIterator elt_iter=modifier.p_mesh->GetElementIteratorBegin();
      elt_iter!=modifier.p_mesh->GetElementIteratorEnd();
      ++elt_iter)
		  {
        unsigned index = elt_iter->GetIndex();

        if (index == 0)
			    {
            cur_conduct[0] = p_monodomain_tissue->rGetIntracellularConductivityTensor(index)(0,0);
				    cur_conduct[1] = p_monodomain_tissue->rGetIntracellularConductivityTensor(index)(1,1);
				    cur_conduct[2] = p_monodomain_tissue->rGetIntracellularConductivityTensor(index)(2,2);	
				    mod_conductivities.push_back( cur_conduct);
          }
        else
        {
				    /* cur_conduct[0] = p_monodomain_tissue->rGetIntracellularConductivityTensor(index)(0,0)/index;
				    cur_conduct[1] = p_monodomain_tissue->rGetIntracellularConductivityTensor(index)(1,1)/index;
				    cur_conduct[2] = p_monodomain_tissue->rGetIntracellularConductivityTensor(index)(2,2)/index; */
            //testing without modification by index
            cur_conduct[0] = p_monodomain_tissue->rGetIntracellularConductivityTensor(index)(0,0);
				    cur_conduct[1] = p_monodomain_tissue->rGetIntracellularConductivityTensor(index)(1,1);
				    cur_conduct[2] = p_monodomain_tissue->rGetIntracellularConductivityTensor(index)(2,2);
				    mod_conductivities.push_back( cur_conduct);
			  }

        //debug
        /* std::cout << "(simulation.cpp) mod_conductivities index: " << index << "conduct: " << 
          cur_conduct[0] << "," << cur_conduct[1] << "," << cur_conduct[2] << std::endl; */
        /* std::cout << "(simulation.cpp) mod_conductivities index: " << index << "conduct: " << 
          cur_conduct[0] << std::endl; */

      }

    //pass this vector<c_vector>) to AddCellData for output
	  conductivities_mesh_writer.AddCellData("Conductivity Modifier",mod_conductivities);
	  conductivities_mesh_writer.WriteFilesUsingMesh(monodomain_problem.rGetMesh());

  }


  std::string cell_type = factory->GetCellType();

  if (cell_type[cell_type.length() -1] == 'P') {
    std::cout << "(simulation.cpp) Solving passive cell type" << std::endl;

    // Export passive cell potential and conductivities if passive cell
    // Set up tissue conductivity modifier if passive cell
    std::vector<std::string> output_variables;
    output_variables.push_back("v_p");
    output_variables.push_back("g_p");
    output_variables.push_back("cai");
    HeartConfig::Instance()->SetOutputVariables(output_variables);

/*     // Get the parameters for the passive cell
    const auto cell_params = toml::parse(USMC_SYSTEM_CONSTANTS::CONFIG_DIR +
                                         factory->GetCellParamFile());
    const auto& passive_params = toml::find(cell_params, "passive");
    std::vector<double> conductivities = toml::find<std::vector<double>>(
      cell_params, "conductivities_3d");

    UterineConductivityModifier modifier(  // Populate with passive cell params
      toml::find<double>(passive_params, "centre"),
      toml::find<double>(passive_params, "slope"),
      conductivities[2],  // z value of conductivity
      toml::find<double>(passive_params, "amplitude"),
      toml::find<std::string>(passive_params, "type"),
      &monodomain_problem.rGetMesh());

    MonodomainTissue<3>* tissue = monodomain_problem.GetMonodomainTissue();
    tissue->SetConductivityModifier(&modifier); */

    //below accidentally commented out and nothing happens!
    monodomain_problem.Solve();  // Need this here otherwise code breaks */

  } else {  // Need this here otherwise code breaks
    monodomain_problem.Solve();
  }
}
