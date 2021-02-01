# Castle Game 3D 2
An OpenGL based 3D game, that uses custom model and level formats.
WIP.

### Model and level format 
Models and levels are created using [Blender](https://www.blender.org/)
and exported via custom Python scripts (located in `/BlenderScripts`).
TODO...

### Game data
The data is located at `/cmake-build-release/data`.
The folder must reside in the same directory as the executable.

### Submodules
* [glm](https://github.com/g-truc/glm)
* [reactphysics3d](https://github.com/DanielChappuis/reactphysics3d)
* [stb](https://github.com/nothings/stb)

### Linked libraries
* glfw3
* OpenGL
* OpenAL

### How to build
Requires C++ 20 and CMake 3.17 (might work on lower versions)

##### 1. Clone repository
Clone and cd into the directory

`$ git clone https://github.com/thomaslienbacher/CastleGame3D2.git`

`$ cd CastleGame3D2`

##### 2. Init and download submodules
`$ git submodule update --init --recursive --depth 1`

#### 3. Generate Make files
CMake is used to generate the make files.
The build type must be given to CMake which can be either Release
`-DCMAKE_BUILD_TYPE=Release` or Debug `-DCMAKE_BUILD_TYPE=Debug`.

`$ cmake . -DCMAKE_BUILD_TYPE=Release`

#### 4. Build
`$ make all`

Add `-j 4` flag to compile using 4 jobs. 

Building tested on Windows 10 and Fedora 33

### Controls
* W A S D - movement
* Mouse - look around
* F - fullscreen

Basically standard FPS controls
