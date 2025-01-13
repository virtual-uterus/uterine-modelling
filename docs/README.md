# Chaste uterine modeling
# Table of contents
1. [General setup](#general-setup)
   1. [Edits](#edits)
   2. [Ready to build](#build)
   3. [Testing](#testing)
2. [Configuration files](#config)
	1. [Cells](#cells)
	2. [0D simulations](#0d)
	3. [2D and 3D simulations](#2d)
2. [Running simulations](#simulations)
3. [Editing code](#editing-code)
	1. [Adding a cell](#add-cell)
	2. [Adding a test](#add-test)

<a id="general-setup"></a>
## General setup
This setup guide assumes that the uterine version of [Chaste](https://github.com/mathiasroesler/chaste/tree/uterine-chaste) has been built successfully. Here is an example of the directory tree:
<a id="tree"></a>
```
Chaste/ (top-level directory)
|
|-- src/ (source code directory)
|   |-- projects/
|       |--uterine-modelling/ (this project)
|          |-- src/
|          |-- include/
|          |-- config/
|          |-- scripts/
|              |-- chaste-env
|              |-- chaste-simulation
|-- lib/ (build directory)
|-- testoutput/ (results directory)
|-- config/ (cp from uterine-modelling)
|-- scripts/ (cp from uterine-modelling)
```

Clone the project in the projects folder of the Chaste source code directory.

<a id="edits"></a>
### Edits

Copy the **config** and **scripts** folder from the _uterine-modelling_ project to the Chaste top-level directory and create the **testoutput** folder (refer to the [directory tree](#tree)).

The project uses environment variables that need to be updated in order to run the project. Open the **chaste-env** script in a text editor and change the **CHASTE_DIR** variable to be the path from **/** to the Chaste top-level directory. Make sure that the **CHASTE_SOURCE_DIR** and **CHASTE_BUILD_DIR** variables are using the correct names of the source code and build directory, respectively.

**Note:** the **APPTAINERENV_** variables are only used on the HPCs and don't have to be updated.

It is recommended to update the user's **.bashrc** file to source the **chaste-env** script automatically when opening a new terminal window. Open the **.bashrc** file in a text editor:

```
$ gedit ~/.bashrc
```

Add the following line at the end of the file after replacing __path_to_Chaste__ with the path to the Chaste top-level directory:

```
source ${HOME}/path_to_Chaste/Chaste/scripts/chaste-env
```


<a id="build"></a>
### Ready to build
Run the **setup_project.py** script in the _uterine-modelling_ folder in the source code directory (refer to the [directory tree](#tree)) and answer the prompted questions. This step only needs to be performed for the first build and can be skipped if rebuilding the project.
 
**Note:** this project only uses cardiac Chaste.

In the Chaste build folder run the following command to update the CMakeLists for the first build or if a new file has been added to the project:

```
$ cmake .
```

Navigate to the _uterine-modelling_ folder in the build directory (refer to the [directory tree](#tree)) and run the following command to build the project:

```
$ cmake --build .
```

<a id="testing"></a>
### Testing
Source the **chaste-env** script in the scripts folder in the Chaste top-level directory to update the environment variables:

```
$ source chaste-env
```

In the _uterine-modelling_ folder in the build directory (refer to the [directory tree](#tree)) run the tests and make sure that they all pass:

```
$ ctest 
```

<a id="config"></a>
## Configuration files

The project uses [TOML](https://toml.io/en/) configuration files to edit simulation and cell parameters. An example of configuration files for the different simulations and each available cell type are found in the **config** folder of the _uterine-modelling_ folder in the source code directory (refer to the [directory tree](#tree)). 

**Note:** the configuration files used during the simulations are located in the **config** folder of the Chaste top-level directory. 

<a id="cells"></a>
### Cells

The cell configuration files are all structured in the same way:
1. _Cell properties_ which consists of the capacitance and conductivities for 2D and 3D simulations;
2. _Stimulus_ which consists of the settings of the stimulus, the magnitude, period (only used if a regular stimulus is applied), the duration, and the start time;
3. _cell_id_ is the unique identifier of the cell. There are currently 4 different cells;
4.  _Cell parameters_ which lists the [modifiable parameters](https://chaste.cs.ox.ac.uk/trac/wiki/ChasteGuides/CodeGenerationFromCellML) in the CellML model. 

The three first categories are required for the cell model to work. The _Cell parameters_ category should only contain the name and value of parameters that have been identified as modifiable in the CellML model. However, if the parameter is modifiable it does not need to be listed for the cell model to work, the default value will be used instead. 
	
**Note:** The units are specified as comments after _Cell properties_ and _Stimulus_ parameters. The initial value and range are specified after _Cell parameters_ parameters. 

<a id="0d"></a>
### 0D simulations

The **0d_params.toml** configuration file is used for single cell simulations only. The only cell type implemented is the FitzHugh-Nagumo cell. The file defines the initial conditions for the equations, the simulation duration, and the ODE solver time and sampling steps. There are two different modes, the slow-wave mode and the original mode. The latter is select by setting the _slow_wave_  parameter to **false**, rather than **true**. 

<a id="2d"></a>
### 2D and 3D simulations

The **2d_params.toml** and **3d_params.toml** configuration files pilot the 2D and 3D simulations, respectively. They are structures similarly:
1. _General parameters_ consists of top-level parameters, the name of the directory in which to save the results, the name of the mesh to use, the directory containing the mesh from Chaste/src/, and the stimulus type; 
2. _Stimulus parameters_ defines the area which will be stimulated in the x, y, and z (if in 3D) directions; 
3. _Time parameters_ defines the time properties of the simulation, the duration, the ODE time step, the PDE time step, and the printing time step, _i.e._ the number of time points in the results. The ODE and PDE time steps should be equal and the printing time step equal or greater than the ODE and PDE time steps;
4. _cell_type_ is the name of the cell model to use. 

There are three different stimuli that are implemented:
* _zero_ which does not provide any stimulus;
* _simple_ which is a single pulse;
* _regular_ which is a impulse train; 

The parameters for the stimuli are set for each individual cell type in their respective configuration files. 

**Note:** the default meshes provided by Chaste are located in the **/mesh/test/data/** folder and the uterine meshes are located in the **/mesh/uterus** folder.

<a id="simulations"></a>
## Running simulations

The simulations are piloted through the config files found in the **config** folder of the Chaste top-level directory and the **chaste-simulation** script found in the **scripts** folder of the Chaste top-level directory.

The **chaste-simulation** script has one argument: the dimension of the simulation. Valid options are 0, 2, and 3. Depending on the dimension, the 0d_, 2d_ or 3d_params.toml file will be used. The parameters for the different cell types are found in the config files with their respective name (see [Configuration files](#config) for more details).  

The results of each simulation are stored in the **testoutput** folder with the following path monodomain_DIM/CELL/results, where DIM is the **chaste-simulation** input dimension and CELL is the cell name found in the config file. A log file is created for each simulation which saves the parameters used for the simulation. 

**Note:** for 0D simulations the results are stored directly in the 0d_simulation folder. 

<a id="editing-code"></a>
## Editing code

This section details the steps for editing the code. 
<a id="add-cell"></a>
### Adding a cell

To add a new cell to the project follow these steps:
1. Ensure that the new cell model has the correct annotation by following the code generation from CellML [guide](https://chaste.cs.ox.ac.uk/trac/wiki/ChasteGuides/CodeGenerationFromCellML). 
2. Place the annotated CellML file for the desired cell in the **src** folder located in the _uterine-modelling_ folder in the source code directory (refer to the [directory tree](#tree)). 
3. Add a case for the new _cell_id_ in the switch statement of the *CreateCardiacCellForTissueNode* functions (see [code snippet](#code) for details) in the following files in the **src** folder located in the _uterine-modelling_ folder in the source code directory (refer to the [directory tree](#tree)):
	- **AbstractUterineCellFactory.cpp**
	- **AbstractUterineCellFactory3d.cpp**
	- **UterineRegularCellFactory.cpp**
	- **UterineRegularCellFactory3d.cpp**
	- **UterineSimpleCellFactory.cpp**
	- **UterineSimpleCellFactory3d.cpp**
4. Add the `#include` statement for the new cell before the `namespace` declaration (see [include statement](#include) for details) in the following files in the **include** folder located in the _uterine-modelling_ folder in the source code directory (refer to the [directory tree](#tree)):
	- **AbstractUterineCellFactory.hpp**
	- **AbstractUterineCellFactory3d.hpp**
5. Rebuild the project (see [Ready to build](#build) section for more details). 

<a id="code"></a>
Example of the code snippet to add from step 3:
```
case cell_id:
	cell = new CellNAMEFromCellMLCvode(mpSolver, mpStimulus);
		
	for (auto it=mpCell_parameters.begin(); it != mpCell_parameters.end(); ++it)
	{
		cell->SetParameter(it->first, it->second);
	}
	break;
```
where cell_id is replaced with the new cell number and NAME is replaced with the new cell name. The `for` loop is only needed if the cell model has [modifiable parameters](https://chaste.cs.ox.ac.uk/trac/wiki/ChasteGuides/CodeGenerationFromCellML).


<a id="include"></a>
Example of the `#include` statement to add from step 4:
```
#include "NAMECvode.hpp"
```
where NAME is replaced with the new cell name. 

**Note:** the convention used for naming the cells is AuthorYear.cellml, _i.e_ **Tong2014.cellml**.


<a id="add-test"></a>
### Adding a test

All of the tests for the project reside in the **test** folder located in the _uterine-modelling_ folder in the source code directory. 

**Note:** if adding a new cell model it is recommended to write a new test for that particular cell, based on the previous cell tests. 

The tests are always classes that inherit from the CxxTest::TestSuite class, with a single public function named NAMEClass(), where NAME is replaced with the name of the test class. When writing a new cell test, ensure that:
* the `#ifndef`, `#define`, and `#endif` statements are updated with the new test name;
* the `#include` statement for the new cell is added (see [include statement](#include) for details) before the `class` declaration;
* the `class` name and public function are updated with the new test name;
* the new cell model is called inside the test function. 

For the test to be include, the **ContinuousTestPack.txt** file must be updated with the name of the new test file. Once the test has been added to the list, rebuild the project (see [Ready to build](#build) section for more details). 
