## Module features

 - This example shows how to create and test a module

## Compile & Run

### Linux

### Using VSCode

 - `Ctrl+Shift+P` enter the command `CMake: Debug`
 - Choose `salamesh` as executable

### Using a Terminal

#### Compile

In directory

`cmake -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build --parallel 8`

#### Run

`./build/_deps/salamesh-build/salamesh`



### Windows

### Using Visual Studio



## Localhost dev mode (Optional)

If you want to modify module and use the local core API of app rather than the one hosted on git: 

 - create `CMakeLists.local.txt` at the root of the module
 - put `set(SALAMESH_URI "/your/path/to/salamesh_dir")` in this file

 ## How is the project organized ?

__SHOULD BE MODIFIED:__

  - `script.lua`: should contains script you want to execute
  - `script.h`: should contains the C++ functions / bindings you want to execute from your lua script

__MUST NOT BE MODIFIED:__

  - `interface.cpp`: contains the functions that enables salamesh to load the module
