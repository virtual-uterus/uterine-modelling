# Chaste setup guide
This guide explains the steps needed to install Singularity on Linux and run Chaste cardiac.
# Table of contents
1. [Install Singularity](#install)
2. [Pulling Chaste](#chaste)
3. [Setting up Chaste](#setup)
	1. [Cloning](#cloning)
	2. [Building](#building)
	3. [Testing](#testing)
4. [Setting up a project](#project)

<a id="install"></a>
## Install Singularity

Singularity install instructions can be found [here](https://docs.sylabs.io/guides/3.5/user-guide/quick_start.html).

Extract go in **/usr/local** folder if not using the provided command. 

Packages are missing that need to be manually installed for the singularity build to work, command to run is the following:
```
$ sudo apt-get install libgtk2.0-dev libfuse-dev libfuse3-dev
```

<a id="chaste"></a>
## Pulling Chaste

The container can be pulled directly using singularity:
```
$ singularity pull docker://chaste/release
```
To use a different version of Chaste replace **chaste/release** with a different image (*i.e.* chaste/develop). The list of option can be found [here](https://hub.docker.com/search?q=chaste).

<a id="setup"></a>
## Setting up Chaste

If not using Singularity set up Chaste with the Ubuntu package and follow the install guide [here](https://chaste.cs.ox.ac.uk/trac/wiki/InstallGuides/UbuntuPackage)

<a id="cloning"></a>
### Cloning
Otherwise, create a folder called Chaste and clone the release version of Chaste from GitHub into a folder inside called **src** with the following command:

```
$ git clone --branch release git@github.com:Chaste/Chaste.git Chaste/src
```

To use the Chaste version for uterine modelling use the following command: 

```
$ git clone --branch uterine-chaste git@github.com:mathiasroesler/chaste.git Chaste/src
```

Run the singularity container with the following command from the folder containing the .sif file:
```
$ singularity run chaste-container.sif
```
where *chaste-container.sif* is the name of the container pull from docker. 

Create a directory for the build outside of the directory containing the source code, here is an example of the tree:

```
Chaste (top-level directory)
|
|-- src (source code directory)
|-- lib (build directory)
```

<a id="building"></a>
### Building
Run the following command in the build directory:
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
To test that the build was successful run the testing command:
```
$ ctest -j4
```

**Note:** the -j4 flag specifies the number of cores to use (in this case 4), it can be upped to 10 on Dell Precision 3571.

To run a specific test use the -R flag followed by a regular expression. For example, the following command runs all the test with the expression 3d in the test name:
 ```
 $ ctest -R 3d
 ``` 

<a id="project"></a>
## Setting up a project

To setup a project follow the instructions [here](https://chaste.cs.ox.ac.uk/trac/wiki/ChasteGuides/UserProjects)

