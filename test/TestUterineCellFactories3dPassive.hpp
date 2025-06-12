#ifndef TEST_TESTUTERINECELLFACTORIES3DPASSIVE_HPP_
#define TEST_TESTUTERINECELLFACTORIES3DPASSIVE_HPP_

#include <cxxtest/TestSuite.h>
#include "PetscSetupAndFinalize.hpp"
#include "../include/factories/AbstractUterineCellFactoryTemplate.hpp"
#include "../include/factories/UterineZeroCellFactory.hpp"
#include "../include/factories/UterineSimpleCellFactory.hpp"
#include "../include/factories/UterineRegularCellFactory.hpp"
#include "../include/factories/UterineRegionCellFactory.hpp"


class TestUterineCellFactories : public CxxTest::TestSuite {
 public:
  void TestUterineCellFactoriesClass() {
    AbstractUterineCellFactoryTemplate<3>* factory(nullptr);
    std::string cell_type = "MeansP";
    std::string save_dir;

    for (int i = 0; i < 4; ++i) {
      switch (i) {  // Switch between different factories
        case 0:
          factory = new UterineZeroCellFactory<3>;
          TS_ASSERT(factory != nullptr)

          std::cout << "Zero Cell Factory" << std::endl;
          factory->SetCellType(cell_type);
          factory->ReadCellParams(factory->GetCellParamFile());
          save_dir = "MonodomainTest/" + cell_type + "/zero_3d";
          break;

        case 1:
          factory = new UterineSimpleCellFactory<3>;
          TS_ASSERT(factory != nullptr)

          std::cout << "Simple Cell Factory" << std::endl;
          factory->SetCellType(cell_type);
          factory->ReadCellParams(factory->GetCellParamFile());
          save_dir = "MonodomainTest/" + cell_type + "/simple_3d";
          break;

        case 3:
          factory = new UterineRegularCellFactory<3>;
          TS_ASSERT(factory != nullptr)

          std::cout << "Regular Cell Factory" << std::endl;
          factory->SetCellType(cell_type);
          factory->ReadCellParams(factory->GetCellParamFile());
          save_dir = "MonodomainTest/" + cell_type + "/regular_3d";
          break;

        case 4:
          factory = new UterineRegionCellFactory<3>;
          TS_ASSERT(factory != nullptr)

          std::cout << "Region Cell Factory" << std::endl;
          factory->SetCellType(cell_type);
          factory->ReadCellParams(factory->GetCellParamFile());
          save_dir = "MonodomainTest/" + cell_type + "/region_3d";
          break;

        default:
          factory = new UterineZeroCellFactory<3>;
          TS_ASSERT(factory != nullptr)

          std::cout << "Zero Cell Factory (default case)" << std::endl;
          factory->SetCellType(cell_type);
          factory->ReadCellParams(factory->GetCellParamFile());
          save_dir = "MonodomainTest/" + cell_type + "/zero_3d";
          break;
      }

      factory->PrintParams();

      MonodomainProblem<3> monodomain_problem(factory);

      HeartConfig::Instance()->SetOutputDirectory(save_dir);
      HeartConfig::Instance()->SetSimulationDuration(5.0);  // ms
      HeartConfig::Instance()->SetMeshFileName(
          "mesh/uterus/test/tube_10mm");
      HeartConfig::Instance()->SetOutputFilenamePrefix("results");
      HeartConfig::Instance()->SetVisualizeWithVtk(true);
      HeartConfig::Instance()->SetIntracellularConductivities(
          Create_c_vector(1.75, 0.19));
      HeartConfig::Instance()->SetSurfaceAreaToVolumeRatio(1400);  // 1/cm
      HeartConfig::Instance()->SetCapacitance(1.0);  // uF/cm^3
      HeartConfig::Instance()->SetOdePdeAndPrintingTimeSteps(0.1, 0.1, 0.1);

      monodomain_problem.Initialise();

      monodomain_problem.Solve();

      HeartEventHandler::Headings();
      HeartEventHandler::Report();
      }

    delete factory;
  }
};

#endif  // TEST_TESTUTERINECELLFACTORIES3DPASSIVE_HPP_



