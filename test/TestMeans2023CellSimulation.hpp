#ifndef TESTMEANS2022CELLSIMULATION_HPP_
#define TESTMEANS2022CELLSIMULATION_HPP_

#include <cxxtest/TestSuite.h>
#include "AbstractCvodeCell.hpp"
#include "CellProperties.hpp"
#include "EulerIvpOdeSolver.hpp"
#include "SimpleStimulus.hpp"
#include "Means2023Cvode.hpp"
#include "SteadyStateRunner.hpp"
#include "FakePetscSetup.hpp"

class TestMeansCellSimulation : public CxxTest::TestSuite
{
public:
    void TestMeansCellSimulationClass()
    {
#ifdef CHASTE_CVODE
        boost::shared_ptr<SimpleStimulus> p_stimulus(new SimpleStimulus(-0.5, 2000.0, 1000.0));
        boost::shared_ptr<AbstractIvpOdeSolver> p_solver;
        boost::shared_ptr<AbstractCvodeCell> p_model(new CellMeans2023FromCellMLCvode(p_solver, p_stimulus));

        p_model->SetTolerances(1e-7, 1e-7);

        double max_timestep = 0.1;

        p_model->SetMaxTimestep(max_timestep);

        double sampling_timestep = max_timestep;
        double start_time = 0.0;
        double end_time = 5000.0;
        OdeSolution solution = p_model->Compute(start_time, end_time, sampling_timestep);

        solution.CalculateDerivedQuantitiesAndParameters(p_model.get());

        unsigned steps_per_row = 1u; // allows you to downsample output.
        bool clean_dir = false;
        unsigned precision = 6u;
        bool include_derived_quantities = true;
        solution.WriteToFile("SingleCellSimulationTest", "Means2023Cvode", "ms", steps_per_row, clean_dir, precision, include_derived_quantities);

#else
        std::cout << "Cvode is not enabled.\n";
#endif
    }
};

#endif /*TESTMEANS2022CELLSIMULATION_HPP_*/

