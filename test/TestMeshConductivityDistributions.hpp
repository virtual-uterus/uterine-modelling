#ifndef TEST_TESTMESHCONDUCTIVITYDISTRIBUTIONS_HPP_
#define TEST_TESTMESHCONDUCTIVITYDISTRIBUTIONS_HPP_

#include <cxxtest/TestSuite.h>

#include "PetscSetupAndFinalize.hpp"
#include "AbstractTetrahedralMesh.hpp"
#include "VtkMeshWriter.hpp"
#include "../include/factories/AbstractUterineCellFactoryTemplate.hpp"
#include "../include/factories/UterineZeroCellFactory.hpp"
#include "../include/conductivity/UterineConductivityModifier.hpp"
#include "../include/toml.hpp"


class TestUterineCellFactories : public CxxTest::TestSuite {
 public:
  void TestUterineCellFactoriesClass() {
    AbstractUterineCellFactoryTemplate<3>* factory(nullptr);
    std::string cell_type = "MeansP";
    std::string save_dir = "MonodomainTest/" + cell_type +
      "/conductivity/zero_3d";
    std::vector<std::string> distributions{"linear", "gaussian"};

    factory = new UterineZeroCellFactory<3>;
    TS_ASSERT(factory != nullptr)

    std::cout << "Zero Cell Factory" << std::endl;
    factory->SetCellType(cell_type);
    factory->ReadCellParams(factory->GetCellParamFile());

    for (unsigned int i = 0; i < distributions.size(); ++i) {
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

      // Get the parameters for the passive cell
      const auto cell_params = toml::parse(USMC_SYSTEM_CONSTANTS::CONFIG_DIR +
                                           factory->GetCellParamFile());
      const auto& passive_params = toml::find(cell_params, "passive");

      UterineConductivityModifier modifier(  // Populate with passive cell params
        toml::find<double>(passive_params, "centre"),
        toml::find<double>(passive_params, "slope"),
        toml::find<double>(passive_params, "g_p"),
        toml::find<double>(passive_params, "amplitude"),
        distributions[i],
        &monodomain_problem.rGetMesh());

      MonodomainTissue<3>* tissue = monodomain_problem.GetMonodomainTissue();

      tissue->SetConductivityModifier(&modifier);

      VtkMeshWriter<3, 3> modified_mesh(save_dir,
                                        distributions[i] + "_conductivity_mesh",
                                        false);
      c_vector<double, 3> cur_conduct;
      std::vector< c_vector<double,3> > mod_conductivities;
      AbstractTetrahedralMesh<3, 3>* mesh = modifier.GetMesh();

      for (AbstractTetrahedralMesh<3, 3>::ElementIterator elem_iter=mesh->GetElementIteratorBegin();
            elem_iter != mesh->GetElementIteratorEnd();
            ++elem_iter) {
          unsigned index = elem_iter->GetIndex();

          cur_conduct[0] = tissue->rGetIntracellularConductivityTensor(
            index)(0, 0);
          cur_conduct[1] = tissue->rGetIntracellularConductivityTensor(
            index)(1, 1);
          cur_conduct[2] = tissue->rGetIntracellularConductivityTensor(
            index)(2, 2);
          mod_conductivities.push_back(cur_conduct);

          if (index != 0) {
            cur_conduct[0] /= index;
            cur_conduct[1] /= index;
            cur_conduct[2] /= index;
          }
      }

      modified_mesh.AddCellData("Conductivity Modifier", mod_conductivities);
      modified_mesh.WriteFilesUsingMesh(monodomain_problem.rGetMesh());
    }
  }
};

#endif  // TEST_TESTMESHCONDUCTIVITYDISTRIBUTIONS_HPP_
