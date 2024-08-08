#ifndef TESTCHAYKEIZER1983CELLSIMULATION_HPP_
#define TESTCHAYKEIZER1983CELLSIMULATION_HPP_

#include <cxxtest/TestSuite.h>
#include "AbstractCvodeCell.hpp"
#include "CellProperties.hpp"
#include "EulerIvpOdeSolver.hpp"
#include "ZeroStimulus.hpp"
#include "ChayKeizer1983Cvode.hpp"
#include "SteadyStateRunner.hpp"
#include "FakePetscSetup.hpp"

class TestChayKeizerCellSimulation : public CxxTest::TestSuite
{
public:
    void TestChayKeizerCellSimulationClass()
    {
#ifdef CHASTE_CVODE
        boost::shared_ptr<ZeroStimulus> p_stimulus(new ZeroStimulus());
        boost::shared_ptr<AbstractIvpOdeSolver> p_solver;
        boost::shared_ptr<AbstractCvodeCell> p_model(new CellChayKeizer1983FromCellMLCvode(p_solver, p_stimulus));

        p_model->SetTolerances(1e-8, 1e-8);

        double max_timestep = 0.1;

        p_model->SetMaxTimestep(max_timestep);

        double sampling_timestep = max_timestep;
        double start_time = 0.0;
        double end_time = 7000.0;
        OdeSolution solution = p_model->Compute(start_time, end_time, sampling_timestep);

        solution.CalculateDerivedQuantitiesAndParameters(p_model.get());

        unsigned steps_per_row = 1u; // allows you to downsample output.
        bool clean_dir = false;
        unsigned precision = 6u;
        bool include_derived_quantities = true;
        solution.WriteToFile("SingleCellSimulationTest", "ChayKeizer1983Cvode", "ms", steps_per_row, clean_dir, precision, include_derived_quantities);

#else
        std::cout << "Cvode is not enabled.\n";
#endif
    }
};

#endif /*TESTCHAYKEIZER1983CELLSIMULATION_HPP_*/
