#ifndef TESTUTERINEREGULARCELLFACTORY3D_HPP_
#define TESTUTERINEREGULARCELLFACTORY3D_HPP_

#include <cxxtest/TestSuite.h>
#include "PetscSetupAndFinalize.hpp"
#include "UterineRegularCellFactory3d.hpp"

class TestUterineRegularCellFactory3d : public CxxTest::TestSuite
{
public:
    void TestUterineRegularCellFactory3dClass()
    {
		HeartConfig::Instance()->SetSimulationDuration(5.0); //ms
        HeartConfig::Instance()->SetMeshFileName("mesh/test/data/3D_0_to_1mm_6000_elements");
        HeartConfig::Instance()->SetOutputFilenamePrefix("results");

        HeartConfig::Instance()->SetVisualizeWithVtk(true);

		UterineRegularCellFactory3d factory;
		std::string cell_type = factory.GetCellType();

		std::string save_dir = "MonodomainTest/" + cell_type + "/regular_3d";
        HeartConfig::Instance()->SetOutputDirectory(save_dir);

		factory.PrintParams();

        MonodomainProblem<3> monodomain_problem( &factory );

        HeartConfig::Instance()->SetIntracellularConductivities(Create_c_vector(
			1.75, 0.19, 0.19));

        HeartConfig::Instance()->SetSurfaceAreaToVolumeRatio(1400); // 1/cm
        HeartConfig::Instance()->SetCapacitance(1.0); // uF/cm^2

        HeartConfig::Instance()->SetOdePdeAndPrintingTimeSteps(0.01, 0.01, 0.1);

        monodomain_problem.Initialise();

        monodomain_problem.Solve();

        HeartEventHandler::Headings();
        HeartEventHandler::Report();
    }
};

#endif /*TESTUTERINEREGULARCELLFACTORY3D_HPP_*/



