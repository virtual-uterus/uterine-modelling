/*

Copyright (c) 2005-2021, University of Oxford.
All rights reserved.

University of Oxford means the Chancellor, Masters and Scholars of the
University of Oxford, having an administrative office at Wellington
Square, Oxford OX1 2JD, UK.

This file is part of Chaste.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
 * Neither the name of the University of Oxford nor the names of its
   contributors may be used to endorse or promote products derived from this
   software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/**
 * @file
 *
 * This file gives an example of how you can create your own executable
 * in a user project.
 */

#include <iostream>
#include <string>
#include "../../include/toml.hpp"

#include "ExecutableSupport.hpp"
#include "Exception.hpp"
#include "PetscTools.hpp"
#include "PetscException.hpp"

#include "../../include/simulation.hpp"

int main(int argc, char *argv[]) {
    // This sets up PETSc and prints out copyright information, etc.
    ExecutableSupport::StandardStartup(&argc, &argv);

    int exit_code = ExecutableSupport::EXIT_OK;

    // You should put all the main code within a try-catch, to ensure that
    // you clean up PETSc before quitting.
    try {
      if (argc < 2) {
        // Default to 2d simulation
        std::cout << "Default simulation" << std::endl;
        run_simulation(2);
      } else {
        std::istringstream string_stream(argv[1]);
        int dim;

        if (!(string_stream >> dim)) {
          // Check if input is an integer
          const std::string err_msg = "Input is not an integer";
          const std::string err_filename = "main.cpp";
          unsigned line_number = 77;

          throw Exception(err_msg, err_filename, line_number);
        } else if (!string_stream.eof()) {
          // Check that the input is just an integer
          const std::string err_msg = "Trailing characters after input";
          const std::string err_filename = "main.cpp";
          unsigned line_number = 84;

          throw Exception(err_msg, err_filename, line_number);
        }

        if (dim != 2 && dim != 3) {
          const std::string err_msg = "Invalid dimension";
          const std::string err_filename = "main.cpp";
          unsigned line_number = 101;

          throw Exception(err_msg, err_filename, line_number);
        } else {
          run_simulation(dim);
        }
      }
    }
    catch (const Exception& e) {
        ExecutableSupport::PrintError(e.GetMessage());
        exit_code = ExecutableSupport::EXIT_ERROR;
    }

    // Optional - write the machine info to file.
    // ExecutableSupport::WriteMachineInfoFile("machine_info");

    // End by finalizing PETSc, and returning a suitable exit code.
    // 0 means 'no error'
    ExecutableSupport::FinalizePetsc();
    return exit_code;
}
