#ifndef TEST_TESTTONG2014CELLSIMULATION_HPP_
#define TEST_TESTTONG2014CELLSIMULATION_HPP_

#include <cxxtest/TestSuite.h>
#include "AbstractCvodeCell.hpp"
#include "CellProperties.hpp"
#include "EulerIvpOdeSolver.hpp"
#include "SimpleStimulus.hpp"
#include "Tong2014Cvode.hpp"
#include "SteadyStateRunner.hpp"
#include "FakePetscSetup.hpp"

class TestTong2014CellSimulation : public CxxTest::TestSuite {
 public:
  void TestTong2014CellSimulationClass() {
    #ifdef CHASTE_CVODE
      boost::shared_ptr<SimpleStimulus> p_stimulus(
        new SimpleStimulus(-0.25, 2000.0, 1000.0));
      boost::shared_ptr<AbstractIvpOdeSolver> p_solver;
      boost::shared_ptr<AbstractCvodeCell> p_model(
        new CellTong2014FromCellMLCvode(p_solver, p_stimulus));
      double max_timestep = 0.1;
      double sampling_timestep = max_timestep;
      double start_time = 0.0;
      double end_time = 5000.0;
      unsigned steps_per_row = 1u;  // allows you to downsample output.
      bool clean_dir = false;
      unsigned precision = 6u;
      bool include_derived_quantities = true;

      // Set up model and run simulation
      p_model->SetMaxTimestep(max_timestep);
      p_model->SetTolerances(1e-7, 1e-7);
      OdeSolution solution = p_model->Compute(
        start_time, end_time, sampling_timestep);

      solution.CalculateDerivedQuantitiesAndParameters(p_model.get());

      solution.WriteToFile(
        "SingleCellSimulationTest", "Tong2014Cvode", "ms",
        steps_per_row, clean_dir, precision, include_derived_quantities);

    #else
      std::cout << "Cvode is not enabled.\n";
    #endif
  }
};

#endif  // TEST_TESTTONG2014CELLSIMULATION_HPP_
