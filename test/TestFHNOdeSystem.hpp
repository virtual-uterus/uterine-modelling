#ifndef TESTFHNODESYSTEM_HPP_
#define TESTFHNODESYSTEM_HPP_

#include <cxxtest/TestSuite.h>
#include "PetscSetupAndFinalize.hpp"
#include "FHNOdeSystem.hpp"

class TestFHNOdeSystem : public CxxTest::TestSuite
{
public:
    void TestFHNOdeSystemClass()
    {
		FHNOdeSystem my_ode;
        EulerIvpOdeSolver euler_solver;
        std::vector<double> initial_condition;
        initial_condition.push_back(1.0);
		initial_condition.push_back(0.0);

        OdeSolution solutions = euler_solver.Solve(&my_ode, initial_condition, 0, 1, 0.01, 0.1);

        solutions.WriteToFile("ODESystemTest", "FHN_ode_solution", "sec");
    }
};

#endif /*TESTFHNODESYSTEM_HPP_*/

