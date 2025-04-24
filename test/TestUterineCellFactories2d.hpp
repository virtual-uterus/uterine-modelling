#ifndef TEST_TESTUTERINECELLFACTORIES2D_HPP_
#define TEST_TESTUTERINECELLFACTORIES2D_HPP_

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
    AbstractUterineCellFactoryTemplate<2>* factory(nullptr);
    std::string cell_type;
    std::string save_dir;

    for (int i = 0; i < 3; ++i) {
      switch (i) {  // Switch between different factories
        case 0:
          factory = new UterineZeroCellFactory<2>;
          TS_ASSERT(factory != nullptr)

          std::cout << "Zero Cell Factory" << std::endl;
          cell_type = factory->GetCellType();
          save_dir = "MonodomainTest/" + cell_type + "/zero_2d";
          break;

        case 1:
          factory = new UterineSimpleCellFactory<2>;
          TS_ASSERT(factory != nullptr)

          std::cout << "Simple Cell Factory" << std::endl;
          cell_type = factory->GetCellType();
          save_dir = "MonodomainTest/" + cell_type + "/simple_2d";
          break;

        case 2:
          factory = new UterineRegularCellFactory<2>;
          TS_ASSERT(factory != nullptr)

          std::cout << "Regular Cell Factory" << std::endl;
          cell_type = factory->GetCellType();
          save_dir = "MonodomainTest/" + cell_type + "/regular_2d";
          break;

        default:
          factory = new UterineZeroCellFactory<2>;
          TS_ASSERT(factory != nullptr)

          std::cout << "Zero Cell Factory (default case)" << std::endl;
          cell_type = factory->GetCellType();
          save_dir = "MonodomainTest/" + cell_type + "/zero_2d";
          break;
      }

      factory->PrintParams();

      MonodomainProblem<2> monodomain_problem(factory);

      HeartConfig::Instance()->SetOutputDirectory(save_dir);
      HeartConfig::Instance()->SetSimulationDuration(5.0);  // ms
      HeartConfig::Instance()->SetMeshFileName(
          "mesh/test/data/2D_0_to_1mm_800_elements");
      HeartConfig::Instance()->SetOutputFilenamePrefix("results");
      HeartConfig::Instance()->SetVisualizeWithVtk(true);
      HeartConfig::Instance()->SetIntracellularConductivities(
          Create_c_vector(1.75, 0.19));
      HeartConfig::Instance()->SetSurfaceAreaToVolumeRatio(1400);  // 1/cm
      HeartConfig::Instance()->SetCapacitance(1.0);  // uF/cm^2
      HeartConfig::Instance()->SetOdePdeAndPrintingTimeSteps(0.1, 0.1, 0.1);

      monodomain_problem.Initialise();

      monodomain_problem.Solve();

      HeartEventHandler::Headings();
      HeartEventHandler::Report();
      }

    delete factory;
  }
};

#endif  // TEST_TESTUTERINECELLFACTORIES2D_HPP_
