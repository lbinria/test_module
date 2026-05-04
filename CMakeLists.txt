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
    GIT_TAG        2de61bccdeec1e25a227077e57d38e8b9ce383ee
    GIT_SUBMODULES_RECURSE ON
  )
  FetchContent_MakeAvailable(salamesh)
 
endif()
 
message(STATUS "Salamesh src directory: ${salamesh_SOURCE_DIR}")
 
add_library(hello_module SHARED
  script.h
  interface.cpp
  renderers/my_line_renderer.h renderers/my_line_renderer.cpp
)
 
# Link to salamesh core
target_link_libraries(hello_module PRIVATE salamesh_core)
 
message("CMAKE_CURRENT_BINARY_DIR=" ${CMAKE_CURRENT_BINARY_DIR})
 
# Get the build directory of the fetched content
get_property(salamesh_build_dir DIRECTORY ${salamesh_SOURCE_DIR} PROPERTY BINARY_DIR)
message(STATUS "Salamesh build directory: ${salamesh_build_dir}")
 
 
 
# Ensure salamesh is built
add_dependencies(hello_module salamesh)
 
 
 
# Copy systematically script.lua file to output build dir when build
if (MSVC)
 
  # Generate JSON settings file at configure time
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/settings.json "
  {
    \"modules\": [
      \"${CMAKE_CURRENT_BINARY_DIR}/Debug\",
      \"modules/module_menubar\"
    ]
  }
  ")
 
  add_custom_command(TARGET hello_module POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${salamesh_build_dir}/$<CONFIG>"
    COMMAND ${CMAKE_COMMAND} -E echo "Creating directory ${salamesh_build_dir}/$<CONFIG>"
 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PROJECT_SOURCE_DIR}/script.lua ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/script.lua
 
    # Generate JSON at build time (use generator expressions if needed)
    # COMMAND ${CMAKE_COMMAND} -E echo_append "{\n\"modules\": [\n\"${CMAKE_CURRENT_BINARY_DIR}\",\n\"modules/module_menubar\"\n]\n}\n" > "${CMAKE_CURRENT_BINARY_DIR}/settings.json"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_CURRENT_BINARY_DIR}/settings.json"
      "${salamesh_build_dir}/$<CONFIG>/settings.json"
    COMMAND ${CMAKE_COMMAND} -E echo "Copied settings.json to $<CONFIG>"
    COMMENT "Overall: copying resources for configuration $<CONFIG>"
  )
 
else()
 
  # Generate JSON settings file after configuration time
  file(WRITE ${salamesh_build_dir}/settings.json "
  {
    \"modules\": [
      \"${CMAKE_CURRENT_BINARY_DIR}\",
      \"modules/module_menubar\"
    ]
  }
  ")
 
  add_custom_target(copy-resources ALL
      COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/script.lua ${CMAKE_CURRENT_BINARY_DIR}/
      COMMENT "Copying script to ${CMAKE_CURRENT_BINARY_DIR}/"
  )
endif()