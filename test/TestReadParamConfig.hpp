#ifndef TEST_TESTREADPARAMCONFIG_HPP_
#define TEST_TESTREADPARAMCONFIG_HPP_

#include <cxxtest/TestSuite.h>
/* Most Chaste come uses PETSc to solve linear algebra problems.  This involves starting PETSc at the beginning of a test-suite
 * and closing it at the end.  (If you never run come in parallel then it is safe to replace PetscSetupAndFinalize.hpp with FakePetscSetup.hpp)
 */
#include "PetscSetupAndFinalize.hpp"
#include "../include/factories/AbstractUterineCellFactoryTemplate.hpp"

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

class TestReadParamConfig : public CxxTest::TestSuite {
 public:
  void TestReadParamConfigClass() {
    AbstractUterineCellFactoryTemplate<2> uSMC_factory_2d;  // 2D uSMC factory 
    AbstractUterineCellFactoryTemplate<3> uSMC_factory_3d;  // 3D uSMC factory

    uSMC_factory_2d.ReadParams(USMC_SYSTEM_CONSTANTS::GENERAL_2D_PARAM_FILE);
    uSMC_factory_2d.PrintParams();

    uSMC_factory_3d.ReadParams(USMC_SYSTEM_CONSTANTS::GENERAL_3D_PARAM_FILE);
    uSMC_factory_3d.PrintParams();
    }
};

#endif  // TEST_TESTREADPARAMCONFIG_HPP_
