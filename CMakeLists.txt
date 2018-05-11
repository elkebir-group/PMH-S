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
