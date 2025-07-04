# Chaste uterine modeling
# Table of contents
1. [General setup](#general-setup)
   1. [Installing uterine-chaste](#install)
   2. [Installing uterine-modelling](#uterine-modelling)
   3. [Edits](#edits)
   4. [Build Chaste](#build)
   5. [Testing](#testing)
2. [Configuration files](#config)
	1. [General configuration files](#sims)
	2. [Cell configuration files](#cells)
	3. [Mesh configuration files](#mesh)
3. [Passive cells](#passive)
	1.  [Configuration files](#passive-config)
	2.  [Distributions](#distros)
4. [Running simulations](#simulations)
5. [Editing code](#editing-code)
	1. [Adding a cell](#add-cell)
	2. [Adding a test](#add-test)

<a id="general-setup"></a>
## General setup
 Here is an example of the directory tree:
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
|              |-- general/
|              |-- cell/
|              |-- estrus/
|              |-- mesh/
|          |-- scripts/
|              |-- chaste-env
|              |-- uterine-simulation
|              |-- multi-simulation
|              |-- estrus-simulation
|              |-- simulation-sweep
|-- lib/ (build directory)
|-- testoutput/ (results directory)
|-- config/ (cp from uterine-modelling)
|-- scripts/ (cp from uterine-modelling)
```

<a id="install"></a>
### Installing uterine-chaste
#### Singularity
Singularity install instructions can be found [here](https://docs.sylabs.io/guides/3.5/user-guide/quick_start.html).

Extract go in **/usr/local** folder if not using the provided command. 

Packages are missing that need to be manually installed for the singularity build to work, command to run is the following:
```
$ sudo apt-get install libgtk2.0-dev libfuse-dev libfuse3-dev
```

<a id="chaste"></a>
#### Pulling the Chaste environment

The container can be pulled directly using singularity:
```
$ singularity pull docker://chaste/release:2024.2
```
To use a different version of Chaste replace **chaste/release** with a different image (*i.e.* chaste/develop). The list of option can be found [here](https://hub.docker.com/search?q=chaste). This will create a .sif file that can be loaded with singularity.

<a id="cloning"></a>
#### Cloning uterine-chaste
Create a folder called Chaste then clone uterine-chaste from GitHub into a folder called **src** with the following commands:

```
$ mkdir Chaste
$ git clone git@github.com:virtual-uterus/uterine-chaste.git Chaste/src
```

<a id="uterine-modelling"></a>
### Installing uterine-modelling 
Clone the uterine-modelling repository to the projects folder in the uterine-chaste **src** folder:
```bash
$ git clone git@github.com:virtual-uterus/uterine-modelling.git Chaste/src/projects
```

Next run the singularity container with the following command from the folder containing the .sif file:
```
$ singularity run chaste-container.sif
```
where *chaste-container.sif* is the name of the container pull from docker. 

<a id="edits"></a>
### Edits
Copy the **config** and **scripts** folder from the _uterine-modelling_ project to the Chaste top-level directory and create the **testoutput** folder (refer to the [directory tree](#tree)).

The project uses environment variables that need to be updated in order to run the project. Open the **chaste-env** script in a text editor and change the **CHASTE_DIR** variable to be the path from the root of the system (**/**) to the Chaste top-level directory. Make sure that the **CHASTE_SOURCE_DIR** and **CHASTE_BUILD_DIR** variables are using the correct names of the source code and build directory, respectively.

**Note:** the **APPTAINERENV_** variables are only used on the HPCs and don't have to be updated if running on a local system.

It is recommended to update the user's **.bashrc** file to source the **chaste-env** script automatically when opening a new terminal window. Open the **.bashrc** file in a text editor:

```
$ gedit ~/.bashrc
```

Add the following line at the end of the file after replacing __path_to_Chaste__ with the path to the Chaste top-level directory:

```
source ${HOME}/path_to_Chaste/Chaste/scripts/chaste-env
```


<a id="build"></a>
### Build Chaste
Create a directory for the build outside of the directory containing the source code, here is an example of the tree:

```
Chaste (top-level directory)
|
|-- src (source code directory)
|-- lib (build directory)
```

Run the **setup_project.py** script in the _uterine-modelling_ folder in the source code directory (refer to the [directory tree](#tree)) and answer the prompted questions. This step only needs to be performed for the first build and can be skipped if rebuilding the project.
 
**Note:** this project only uses cardiac Chaste.

In the Chaste build folder run the following command to update the CMakeLists for the first build or if a new file has been added to the project:

```
$ cmake ../src
```
**Note:** the argument give to cmake must be the path to the Chaste source code, change it as needed.

Once cmake has been run the project can be build with the following command:
```
$ make -j4 Continuous
```

**Note:** the -j4 flag specifies the number of cores to use (in this case 4), it can be upped to 10 on Dell Precision 3571.

<a id="testing"></a>
### Testing

Source the **chaste-env** script in the scripts folder in the Chaste top-level directory to update the environment variables:

```
$ source chaste-env
```

To test that the build was successful run the testing command:
```
$ ctest -j4
```

**Note:** the -j4 flag specifies the number of cores to use (in this case 4), it can be upped to 10 on Dell Precision 3571.

To run a specific test use the -R flag followed by a regular expression. For example, the following command runs all the test with the expression 3d in the test name:
 ```
 $ ctest -R 3d
 ``` 

<a id="config"></a>
## Configuration files
The project uses [TOML](https://toml.io/en/) configuration files to edit simulation and cell parameters. An example of configuration files for the different simulations and each available cell type are found in the **config** folder of the _uterine-modelling_ folder in the source code directory (refer to the [directory tree](#tree)). 

**Note:** the configuration files used during the simulations are located in the **config** folder of the Chaste top-level directory. 

<a id="sims"></a>
### General configuration files
The **2d_params.toml** and **3d_params.toml** configuration files pilot the 2D and 3D simulations, respectively,and are located in the **config/general** folder. They are structures similarly:
1. _General parameters_ consists of top-level parameters, the name of the directory in which to save the results, the name of the mesh to use, the directory containing the mesh from Chaste/src/, and the stimulus type, and the orthotropic flag to include fibre orientations; 
2. _Stimulus parameters_ defines the area which will be stimulated in the x, y, and z (if in 3D) directions and only used for simple and regular stimuli; 
3. _Time parameters_ defines the time properties of the simulation, the duration, the ODE time step, the PDE time step, and the printing time step, _i.e._ the number of time points in the results. The ODE and PDE time steps should be equal and the printing time step equal or greater than the ODE and PDE time steps;
4. _Cell parameters_ defines the name of the cell model to use, and the estrus cycle (only needed if the cell type is Roesler or any other non-pregnant cell). 

There are four different stimuli that are implemented:
* _zero_ which does not provide any stimulus;
* _simple_ which is a single pulse;
* _regular_ which is an impulse train with a certain frequency;
* _region_ which is the same as regular but with selects one of the regions (see cell configuration files). 

The parameters for the stimuli are set for each individual cell type in their respective configuration files. 

**Note:** the default meshes provided by Chaste are located in the **src/mesh/test/data/** folder and the uterine meshes are located in the **src/mesh/uterus** folder.

<a id="cells"></a>
### Cell configuration files
The cell configuration files are located in the **config/cell** and **config/estrus** folder. The files in the **config/cell** folder are for pregnant cells or cells that do not include non-pregnant variations. The files in the **config/estrus** must have the form Cell-name_estrus-phase, where Cell-name is the cell name and estrus-phase one of cycle stages (proestrus, estrus, metestrus, and diestrus). All cell configuration files are structured in the same way:
1. _Cell properties_ which consists of the capacitance and conductivities for 2D, 3D, and orthotropic simulations;
2. _Stimulus_ which consists of the settings of the stimulus, the magnitude, period (only used if a regular stimulus is applied), the duration, the start time, and the region probability;
3. _cell_id_ is the unique identifier of the cell. There are currently 4 different cells;
4.  _Cell parameters_ which lists the [modifiable parameters](https://chaste.github.io/docs/user-guides/code-generation-from-cellml/) in the CellML model. 

The three first categories are required for the cell model to work. The _Cell parameters_ category should only contain the name and value of parameters that have been identified as modifiable in the CellML model. However, if the parameter is modifiable it does not need to be listed for the cell model to work, the default value will be used instead. 

The orthotropic conductivity vector (ortho_conductivities) is only used if the orthotropic flag is true. In that case, an ortho file is required to be in the same folder as the mesh that is used.

The region probabilities is only required when using the region stimulus. The location is selected with a random number generator and the probabilities should always sum to one. The region is either the ovarian end, centre, or cervical end of the uterine horns. The locations are specified in the mesh configuration files. 

**Note:** The units are specified as comments after _Cell properties_ and _Stimulus_ parameters. The initial value and range are specified after _Cell parameters_ parameters. 


<a id="mesh"></a>
### Mesh configuration files
The mesh configuration files located in the **config/mesh** folder provide the locations of the stimulated regions. There are three regions per horn defined by the z coordinates:
1. _cervical_ the lowest and near the cervix;
2. _centre_ the central section of the horn; and
3. _ovarian_ the highest region located near the ovaries.

There is one section per horn which contains the x and y limits. They are used to stimulate the given horn rather than both. There is one file per mesh and must have the same name as the mesh.

<a id="passive"></a>
## Passive cells
For cells that include passive cells, the cell name should finish with the letter P (capital is required). For example, the Means2023 cell model which includes passive cells is Means2023P and the configuration file is named MeansP.toml.

<a id="passive-config"></a>
### Configuration file
The cell configuration file contains an additional category of parameters for the passive cell with the following parameters:
1. **type**: Gaussian or linear, the type of distribution to use.
2. **g_p**: the value of the passive cell conductance.
3. **slope**: slope for the distribution.
4. **centre**: centre of the distribution.
5. **amplitude**: scaling factor for the Gaussian distribution.

<a id="distros"></a>
### Distributions
Currently, there are two distributions: linear and Gaussian. The distributions modify the value of g_p and the tissue conductivity based on the z axis of the mesh. 
The formula for the linear ad the Gaussian distributions is:
``` linear
new_value = baseline + (z - centre) * slope
```
``` Gaussian
new_value = baseline + amplitude * exp(-slope * (z - centre) ^ 2)
```
where baseline is either g_p or the z-axis conductivity value.

<a id="simulations"></a>
## Running simulations
The simulations are piloted through the config files found in the **config** folder of the Chaste top-level directory and the script found in the **scripts** folder of the Chaste top-level directory. There are several scripts that can be used:
1. _uterine-simulation_ is the most basic one, it runs a single simulation with the parameters in the configuration files.
2. _multi-simulation_ is an modified version of the _uterine-simualtion_ script which runs N identical simulations. 
3. _estrus-simulation_ is specifically designed to run 4 simulation, one at each stage of the estrus cycle. The meshes are changed to the estrus-specific ones. They can be edited directly in the script.
4.  _simulation-sweep_ calls the Python script from the [symprobe](https://github.com/virtual-uterus/symprobe)  package which provides different types of sweeps.

The scripts all have at least one positional argument: the dimension of the simulation. Valid options are 2 or 3. Depending on the dimension, the 2d_ or 3d_params.toml file will be used. The parameters for the different cell types are found in the config files with their respective name (see [Configuration files](#config) for more details).

The results of each simulation are stored in the **testoutput** folder with the following path CELL/monodomain_DIM/results, where DIM is the script input dimension and CELL is the cell name found in the general configuration file. A log file is created for each simulation which saves the parameters used for the simulation. 

<a id="editing-code"></a>
## Editing code

This section details the steps for editing the code. 
<a id="add-cell"></a>
### Adding a cell

To add a new cell to the project follow these steps:
1. Ensure that the new cell model has the correct annotation by following the code generation from CellML [guide](https://chaste.cs.ox.ac.uk/trac/wiki/ChasteGuides/CodeGenerationFromCellML). 
2. Place the annotated CellML file for the desired cell in the **src** folder located in the _uterine-modelling_ folder in the source code directory (refer to the [directory tree](#tree)). 
3. Add a case for the new _cell_id_ in the switch statement of the *InitCell* functions in the AbstractUterineCellFactoryTemplate.tpp file in the **src/projects/uterine-modelling/src/factories** folder (see [code snippet](#code) for details).
4. Add the `#include` statement for the new cell before the `namespace` declaration (see [include statement](#include) for details) in the following files in the **include** folder located in the _uterine-modelling_ folder in the source code directory (refer to the [directory tree](#tree)):
	- **AbstractUterineCellFactoryTemplate.hpp**
5. Rebuild the project by following these commands while in the singularity container:
```bash
$ cd Chaste/lib/projects/uterine-modelling/
$ cmake ../../
$ cmake --build .
```

<a id="code"></a>
Example of the code snippet to add from step 3:
```
case cell_id:
	cell = new CellNAMEFromCellMLCvode(this->mpSolver, stim);
	break;
```
where cell_id is replaced with the new cell number and NAME is replaced with the new cell name.


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
