cmake_minimum_required(VERSION 3.0.2)
project(PMH-S VERSION 1.0)

set(pmh-s-cp_src
  src/cpmain.cpp
  src/basetree.cpp
  src/clonetree.cpp
  src/migrationgraph.cpp
  src/cpsolver.cpp
  src/solver.cpp
  src/utils.cpp
)

set(pmh-s-cp_hdr
  src/basetree.h
  src/clonetree.h
  src/migrationgraph.h
  src/cpsolver.h
  src/solver.h
  src/utils.h
)

set(pmh-s-dp_src
  src/dpmain.cpp
  src/basetree.cpp
  src/clonetree.cpp
  src/migrationgraph.cpp
  src/migrationtree.cpp
  src/dpsolver.cpp
  src/solver.cpp
  src/utils.cpp
)

set(pmh-s-dp_hdr
  src/basetree.h
  src/clonetree.h
  src/migrationgraph.h
  src/migrationtree.h
  src/dpsolver.h
  src/solver.h
  src/utils.h
)

# first we can indicate the documentation build as an option and set it to ON by default
option(BUILD_DOC "Build documentation" ON)

if (CMAKE_BUILD_TYPE MATCHES "^[Rr]elease")
  # check if Doxygen is installed
  find_package(Doxygen)
  if (DOXYGEN_FOUND)
      # set input and output files
      set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
      set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

      # request to configure the file
      configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
      message("Doxygen build started")

      # note the option ALL which allows to build the docs together with the application
      add_custom_target( doc_doxygen ALL
          COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
          WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
          COMMENT "Generating API documentation with Doxygen"
          VERBATIM )
  else (DOXYGEN_FOUND) 
    message("Doxygen need to be installed to generate the doxygen documentation")
  endif (DOXYGEN_FOUND)
endif()

if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: Debug Release Profile" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set(LIBLEMON_ROOT "$ENV{HOME}/lemon" CACHE PATH "Additional search directory for lemon library" )

set(Boost_USE_STATIC_LIBS ON )
find_package(Boost REQUIRED COMPONENTS thread system filesystem)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")

include_directories( ${INCLUDE_DIRECTORIES} "${LIBLEMON_ROOT}/include" "src" ${Boost_INCLUDE_DIRS} )
link_directories( ${LINK_DIRECTORIES} "${LIBLEMON_ROOT}/lib" )

#find_package(ortools CONFIG REQUIRED)
#add_executable(pmh-s-cp ${pmh-s-cp_src} ${pmh-s-cp_hdr})
#target_link_libraries(pmh-s-cp ortools::ortools ${Boost_LIBRARIES} emon)

add_executable(pmh-s-dp ${pmh-s-dp_src} ${pmh-s-dp_hdr})
target_link_libraries(pmh-s-dp ${Boost_LIBRARIES} emon)
