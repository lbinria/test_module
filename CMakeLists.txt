cmake_minimum_required(VERSION 3.5...4.1.2)
project(hello_module)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)

# Needed if you want to use "app.is_debug"
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
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
    GIT_TAG        eaad9251e1044fded2d0511f204b725b0443c0e8
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

# Generate JSON settings file after build
file(WRITE ${salamesh_build_dir}/settings.json "
{
	\"modules\": [
		\"${CMAKE_CURRENT_BINARY_DIR}\",
		\"modules/module_menubar\"
	]
}
")

# Copy systematically script.lua file to output build dir when build
if (MSVC)
  add_custom_target(copy-resources ALL
      COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/script.lua ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/ 
      COMMENT "Copying script to ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/"
  )
else()
  add_custom_target(copy-resources ALL
      COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/script.lua ${CMAKE_CURRENT_BINARY_DIR}/
      COMMENT "Copying script to ${CMAKE_CURRENT_BINARY_DIR}/"
  )
endif()