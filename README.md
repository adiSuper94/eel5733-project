# eel5733-project

Project is built using cmake(3.22)
To install cmake from source refer [this](https://cmake.org/install/).
To install using apt repositories on ubuntu refer [this](https://askubuntu.com/questions/355565/how-do-i-install-the-latest-version-of-cmake-from-the-command-line).

## Setps to compile Project using `make`
- `cd` into project root(same dir this README file is sitting in)
- run `make`. This should complile the required binaries in the current dir.

## Steps to compile Projectusing `cmake` (prefered)
- `cd` into the directory where the `CMakeLists.txt` file exists.
- run `cmake -S ./ -B ./build`
- run `cd build` to cd into the build directory which should be generated by the cmake command. This dir should contain `Makefile`
- run `make`. This should compile all source code and generated required binaries in the `build` dir.


### Assignment 1
- There are three binaries associated with this assignment: `email_filter`, `calendar_filter` and `location-updater`.
- Before running `location-updater` ensure that you are in the directory where all three binaries are.
- run `./location-updater < input.txt` (input.txt is a placeholder)

### Assignment 2
- There is only one binary associated with the assignment: `threaded_location_updater`
- run `./threaded_location_updater < input.txt` (input.txt is a placeholder)

### Assignment 3
- There is only one binary associated with the assignment: `eft`
- run `./eft ./input.txt 5` (input.txt is a placeholder) , 5 is an example thread count

### Assignment 4
- There is only one binary associated with the assignment: `mmap_location_updater`
- run `./mmap_location_updater 10 < ./input.txt ` (input.txt is a placeholder) , 10 is buffer size
