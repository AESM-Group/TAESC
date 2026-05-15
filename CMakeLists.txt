cmake_minimum_required(VERSION 3.13)

project(taesc LANGUAGES CXX)

# Extract version number from version.h file
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/src/version.h version_file)
string(REGEX MATCH "TAESC_MAJOR_VERSION ([0-9]+)" taesc_major_version ${version_file})
set(taesc_major_version ${CMAKE_MATCH_1})
string(REGEX MATCH "TAESC_MINOR_VERSION ([0-9]+)" taesc_minor_version ${version_file})
set(taesc_minor_version ${CMAKE_MATCH_1})
string(REGEX MATCH "TAESC_PATCH_VERSION ([0-9]+)" taesc_patch_version ${version_file})
set(taesc_patch_version ${CMAKE_MATCH_1})
set(version "${taesc_major_version}.${taesc_minor_version}.${taesc_patch_version}")

set(CMAKE_PROJECT_VERSION "${version}")

# Public options setup
option(TAESC_ENABLE_LIBRPA          ""    ON)
option(TAESC_ENABLE_TEST            ""    OFF)
option(TAESC_ENABLE_FORTRAN_BIND    ""    ON)
option(TAESC_FORTRAN_DP             ""    "c_double")

# C++ standard requirement
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(GNUInstallDirs)

target_include_directories(taesc
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

add_subdirectory(src)

install(TARGETS taesc
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES
  include/taesc/taesc.h
  include/taesc/taesc.hpp
  include/taesc/taesc_compute.h
  include/taesc/taesc_enums.h
  include/taesc/taesc_global.h
  include/taesc/taesc_handler.h
  include/taesc/taesc_input.h
  include/taesc/taesc_options.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/taesc)

add_subdirectory(driver)
