cmake_minimum_required(VERSION 3.5...4.1.2)
project(hello_module)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)
 
# Needed if you want to use "app.is_debug"
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
  else()
    add_compile_definitions($<$<CONFIG:Debug>:DEBUG>)
endif()
 
# Include local overrides if the file exists
if(EXISTS "${CMAKE_SOURCE_DIR}/CMakeLists.local.txt")
 
  # Local include
	message(STATUS "Including local configuration")
	include("${CMAKE_SOURCE_DIR}/CMakeLists.local.txt")
 
  # Add salamesh as subdir
	add_subdirectory(${SALAMESH_URI} salamesh)
  # Source dir is the directory we point on local
  set(salamesh_SOURCE_DIR ${SALAMESH_URI})
 
else()
 
  # Fetch for git
  set(SALAMESH_URI "https://github.com/lbinria/salamesh.git")
 
  include(FetchContent)
  FetchContent_Declare(
    salamesh
    GIT_REPOSITORY "${SALAMESH_URI}"
    GIT_TAG        0237d8c1d6e72b874723c6114ac9ada46d2fcf24
    GIT_SUBMODULES_RECURSE ON
  )
  FetchContent_MakeAvailable(salamesh)
 
endif()
 
message(STATUS "Salamesh src directory: ${salamesh_SOURCE_DIR}")
 
add_library(hello_module SHARED
  script.h
  interface.cpp
)
 
# Link to salamesh core
target_link_libraries(hello_module PRIVATE salamesh_core)
 
message("CMAKE_CURRENT_BINARY_DIR=" ${CMAKE_CURRENT_BINARY_DIR})
 
# Get the build directory of the fetched content
get_property(salamesh_build_dir DIRECTORY ${salamesh_SOURCE_DIR} PROPERTY BINARY_DIR)
message(STATUS "Salamesh build directory: ${salamesh_build_dir}")
 
 
 
# Ensure salamesh is built
add_dependencies(hello_module salamesh)
 

if(CMAKE_CONFIGURATION_TYPES)
  # Multi-config generator (Visual Studio, Xcode, Ninja Multi-Config, etc.)
  set(CONFIG_SUBDIR "/$<CONFIG>/")
else()
  # Single-config generator (Unix Makefiles, Ninja, etc.)
  set(CONFIG_SUBDIR "/")
endif()

message(STATUS "Configuration subdirectory: ${CONFIG_SUBDIR}")

# First, use configure_file to expand variables at configure time
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/settings.json.in 
    ${CMAKE_CURRENT_SOURCE_DIR}/settings.json.tmp
)

# Second, use generate to expand generators at compile time
file(GENERATE
  OUTPUT ${salamesh_build_dir}${CONFIG_SUBDIR}settings.json
  INPUT ${CMAKE_CURRENT_SOURCE_DIR}/settings.json.tmp 
)
 
# Copy systematically script.lua file to output build dir when build
if (MSVC)
 
 
  add_custom_command(TARGET hello_module POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${salamesh_build_dir}/$<CONFIG>"
    COMMAND ${CMAKE_COMMAND} -E echo "Creating directory ${salamesh_build_dir}/$<CONFIG>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PROJECT_SOURCE_DIR}/script.lua ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/script.lua
    COMMAND ${CMAKE_COMMAND} -E echo "Copied settings.json to $<CONFIG>"
    COMMENT "Overall: copying resources for configuration $<CONFIG>"
  )
 
else()

  add_custom_target(copy-resources ALL
      COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/script.lua ${CMAKE_CURRENT_BINARY_DIR}/
      COMMENT "Copying script to ${CMAKE_CURRENT_BINARY_DIR}/"
  )
endif()