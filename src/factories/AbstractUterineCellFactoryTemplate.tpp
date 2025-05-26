#include "../../include/factories/AbstractUterineCellFactoryTemplate.hpp"
#include "Exception.hpp"

template <int DIM>
AbstractUterineCellFactoryTemplate<DIM>::AbstractUterineCellFactoryTemplate() : 
  AbstractCardiacCellFactory<DIM>() {
    if (DIM == 2) {
      ReadParams(USMC_SYSTEM_CONSTANTS::GENERAL_2D_PARAM_FILE);
    } else if (DIM == 3) {
      ReadParams(USMC_SYSTEM_CONSTANTS::GENERAL_3D_PARAM_FILE);
    } else {
      const std::string err_msg = "Invalid dimension";
      const std::string err_filename = "AbstractUterineCellFactoryTemplate.cpp";
      unsigned line_number = 15;
      throw Exception(err_msg, err_filename, line_number);
    }
}


template<int DIM>
AbstractUterineCellFactoryTemplate<DIM>::~AbstractUterineCellFactoryTemplate() {
}


template <int DIM>
AbstractCvodeCell* AbstractUterineCellFactoryTemplate<DIM>::CreateCardiacCellForTissueNode(
  Node<DIM>* pNode) {
  AbstractCvodeCell* cell(nullptr);
  
  // Initialise cell with ZeroStimulus
  this->InitCell(cell, this->mpZeroStimulus);

  // Set parameters for the cell
  this->SetCellParams(cell);

  return cell;
}


template <int DIM>
std::string AbstractUterineCellFactoryTemplate<DIM>::GetCellType() {
  return mpCell_type;
}


template <int DIM>
std::string AbstractUterineCellFactoryTemplate<DIM>::GetCellParamFile() {
  std::string cell_param_file = "";

  if (mpEstrus == "") {
    // No specified estrus phase
    cell_param_file = "cell/" + mpCell_type + ".toml";
  } else {
    cell_param_file = "estrus/" + mpCell_type + "_" + mpEstrus + ".toml";
  }

  return cell_param_file;
}

template <int DIM>
void AbstractUterineCellFactoryTemplate<DIM>::ReadParams(std::string general_param_file) {
  std::string general_param_path = USMC_SYSTEM_CONSTANTS::CONFIG_DIR +
    general_param_file;
  const auto params = toml::parse(general_param_path);

  mpCell_type = toml::find<std::string>(params, "cell_type");

  if ((mpCell_type == std::string("Roesler")) || (mpCell_type == std::string("RoeslerP"))) {
    // Get the estrus phase as well
    mpEstrus = toml::find<std::string>(params, "estrus");
  } else {
    mpEstrus = "";
  }

  // Get the cell configuration
  std::string cell_param_file = GetCellParamFile();

  ReadCellParams(cell_param_file);
}


template <int DIM>
void AbstractUterineCellFactoryTemplate<DIM>::ReadCellParams(std::string cell_param_file) {
  std::string cell_param_path = USMC_SYSTEM_CONSTANTS::CONFIG_DIR +
    cell_param_file;
  const auto cell_params = toml::parse(cell_param_path);

  mpCell_id = toml::find<std::int16_t>(cell_params, "cell_id");

  if (cell_params.contains("parameters")) {
    mpCell_parameters = toml::find<std::unordered_map<std::string, float>>(
      cell_params,
      "parameters");
  }
}


template <int DIM>
void AbstractUterineCellFactoryTemplate<DIM>::SetCellParams(AbstractCvodeCell* cell) {
  if (mpCell_id > 1) {
    for (auto it=mpCell_parameters.begin();
        it != mpCell_parameters.end();
        ++it) {
          if (it->first == "g_p") {  // If the parameters has a passive cell
            // Generate a random number between 0 and 1
            std::random_device rd;
            std::mt19937 mt(rd());  // Random number generator
            std::uniform_real_distribution<double> dist(0.0, 1.0);

            double rand_val = dist(mt);
            if (rand_val < 0.9) {
              cell->SetParameter(it->first, 0.0);
            } else {
              cell->SetParameter(it->first, it->second);
            }
          } else {
            cell->SetParameter(it->first, it->second);
          }
    }
  }
}


template <int DIM>
void AbstractUterineCellFactoryTemplate<DIM>::InitCell(AbstractCvodeCell*& cell,
                                                       boost::shared_ptr<AbstractStimulusFunction> stim) {
  switch (mpCell_id) {
    case 0:
      cell = new CellHodgkinHuxley1952FromCellMLCvode(this->mpSolver,
        stim);
      break;

    case 1:
      cell = new CellChayKeizer1983FromCellMLCvode(this->mpSolver, stim);
      break;

    case 2:
      cell = new CellMeans2023FromCellMLCvode(this->mpSolver, stim);
      break;

    case 3:
      cell = new CellTong2014FromCellMLCvode(this->mpSolver, stim);
      break;

    case 4:
      cell = new CellRoesler2024FromCellMLCvode(this->mpSolver, stim);
      break;

    case 5:
      cell = new CellRoesler2024PFromCellMLCvode(this->mpSolver, stim);
      break;

    case 6:
      cell = new CellMeans2023PFromCellMLCvode(this->mpSolver, stim);
      break;

    default:
      const std::string err_msg = "Invalid cell type";
      const std::string err_filename = "AbstractUterineCellFactoryTemplate.cpp";
      unsigned line_number = 160;
      throw Exception(err_msg, err_filename, line_number);
  }
}


template <int DIM>
void AbstractUterineCellFactoryTemplate<DIM>::PrintParams() {
  std::cout << "mpCell_type = " << mpCell_type << std::endl;
  std::cout << "mpCell_id = " << mpCell_id << std::endl;
  std::cout << "mpCell_parameters\n";

  for (auto it=mpCell_parameters.begin(); it != mpCell_parameters.end(); ++it) {
    std::cout << "  " << it->first << " = " << it->second << std::endl;
  }
}


template <int DIM>
void AbstractUterineCellFactoryTemplate<DIM>::WriteLogInfo(std::string log_file) {
  std::ofstream log_stream;
  log_stream.open(log_file, ios::app);  // Open log file in append mode

  log_stream << "Cell parameters \n";

  for (auto it=mpCell_parameters.begin(); it != mpCell_parameters.end(); ++it) {
    log_stream << "  " << it->first << ": " << it->second << std::endl;
  }

  log_stream.close();
}
