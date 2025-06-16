#ifndef TEST_TESTCONDUCTIVITYDISTRIBUTIONS_HPP_
#define TEST_TESTCONDUCTIVITYDISTRIBUTIONS_HPP_

#include <cxxtest/TestSuite.h>
/* Most Chaste come uses PETSc to solve linear algebra problems.  This involves starting PETSc at the beginning of a test-suite
 * and closing it at the end.  (If you never run come in parallel then it is safe to replace PetscSetupAndFinalize.hpp with FakePetscSetup.hpp)
 */
#include "PetscSetupAndFinalize.hpp"
#include "../include/conductivity/distribution_fcts.hpp"

/**
 * @file
 *
 * This is an example of a CxxTest test suite, used to test the source
 * come, and also used to run simulations (as it provides a handy
 * shortcut to compile and link against the correct libraries using scons).
 *
 * You can #include any of the files in the project 'src' folder.
 * For example here we #include "Hello.hpp"
 *
 * You can utilise any of the come in the main the Chaste trunk
 * in exactly the same way.
 * NOTE: you will have to alter the project SConscript file lines 41-44
 * to enable #including of come from the 'heart', 'cell_based' or 'crypt'
 * components of Chaste.
 */

class TestConductivityDistributions : public CxxTest::TestSuite {
 public:
  void TestConductivityDistributionsClass() {
    // Linear distribution test cases
    TS_ASSERT_DELTA(linear_distribution(1.0, 1.0, 0.0, 1.0),
                    1.0, 1e-9);
    TS_ASSERT_DELTA(linear_distribution(1.0, 0.0, 1.0, 2.0),
                    -1.0, 1e-9);
    TS_ASSERT_DELTA(linear_distribution(-1.0, 0.0, 1.0, 0.0),
                    -1.0, 1e-9);
    TS_ASSERT_DELTA(linear_distribution(1.0, 0.0, 5.0, 0.0),
                    5.0, 1e-9);
    TS_ASSERT_DELTA(linear_distribution(0.0, 1.0, 2.0, 1.0),
                    -1.0, 1e-9);
    TS_ASSERT_DELTA(linear_distribution(3.0, 1.0, 2.0, 1.0),
                    5.0, 1e-9);

    // Gaussian distribution test cases
    TS_ASSERT_DELTA(gaussian_distribution(0.0, 0.0, 1.0, 0.0, 1.0),
                    1.0, 1e-5);
    TS_ASSERT_DELTA(gaussian_distribution(1.0, 0.0, 1.0, 0.0, 1.0),
                    0.36788, 1e-5);
    TS_ASSERT_DELTA(gaussian_distribution(0.0, 1.0, 1.0, 0.0, 1.0),
                    2.0, 1e-5);
    TS_ASSERT_DELTA(gaussian_distribution(0.0, 0.0, 1.0, 0.0, 2.0),
                    2.0, 1e-5);
    TS_ASSERT_DELTA(gaussian_distribution(-10.0, 1.0, 1.0, 0.0, 2.0),
                    1.0, 1e-5);
    TS_ASSERT_DELTA(gaussian_distribution(3.0, 1.0, 1.0, 3.0, 2.0),
                    3.0, 1e-5);
  }
};

#endif  // TEST_TESTCONDUCTIVITYDISTRIBUTIONS_HPP_

