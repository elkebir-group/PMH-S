/*
 * cpmain.cpp
 *
 *  Created on: 30-apr-2018
 *      Author: M. El-Kebir
 */

#include "utils.h"
#include "clonetree.h"
#include "migrationgraph.h"
#include "cpsolver.h"
#include <lemon/arg_parser.h>
#include <fstream>

int main(int argc, char** argv)
{
  std::string primary, outputDirectory;
  int timeLimit = -1;
  
  lemon::ArgParser ap(argc, argv);
  ap.other("T", "Clone tree")
    .other("leaf_labeling", "Leaf labeling")
    .refOption("l", "Time limit in seconds (default: -1, no time limit)", timeLimit)
    .refOption("o", "Output prefix" , outputDirectory)
    .refOption("p", "Primary location", primary, true);
  ap.parse();
  
  if (ap.files().size() != 2)
  {
    std::cerr << "Error: <T> and <leaf_labeling> must be specified" << std::endl;
    return 1;
  }
  
  std::string filenameT = ap.files()[0];
  std::string filenameLeafLabeling = ap.files()[1];
  
  std::ifstream inT(filenameT.c_str());
  if (!inT.good())
  {
    std::cerr << "Could not open '" << filenameT << "' for reading" << std::endl;
    return 1;
  }
  
  std::ifstream inLeafLabeling(filenameLeafLabeling.c_str());
  if (!inLeafLabeling.good())
  {
    std::cerr << "Could not open '" << filenameLeafLabeling << "' for reading"
    << std::endl;
    return 1;
  }
  
  CloneTree T;
  if (!T.read(inT)) return 1;
  if (!T.readLeafLabeling(inLeafLabeling)) return 1;
  
  if (T.getLocations().count(primary) == 0)
  {
    std::cerr << "Error: " << primary << " is not a location" << std::endl;
    return 1;
  }
  
  CpSolver solver(T, primary);
  auto colorMap = T.generateColorMap();
  solver.run(solver, T, primary, outputDirectory, "CP-", colorMap, 1, timeLimit);
  
  return 0;
}
