/*
 * solver.cpp
 *
 *  Created on: 10-may-2018
 *      Author: M. El-Kebir
 */

#include "solver.h"
#include "migrationgraph.h"
#include <lemon/time_measure.h>

Solver::Solver(const CloneTree& T,
               const std::string& primary)
  : _T(T)
  , _primary(primary)
  , _lPlus(T.tree())
{
}

int Solver::run(Solver& solver,
                const CloneTree& T,
                const std::string& primary,
                const std::string& outputDirectory,
                const std::string& outputPrefix,
                const StringToIntMap& colorMap,
                int nrThreads,
                int timeLimit)
{
  char buf[1024];
  
  lemon::Timer timer;
  bool solved = solver.solve(nrThreads, timeLimit);
  if (!solved)
  {
    std::cout << outputPrefix << "\t"
              << "-" << "\t"
              << "-" << "\t"
              << "-" << "\t"
              << "-" << "\t"
              << timer.realTime()
              << std::endl;

    return -1;
  }

  MigrationGraph G(T, solver.lPlus());
  
  int mu = G.getNrMigrations();
  int gamma = G.getNrComigrations(T, solver.lPlus());
  
  std::cout << outputPrefix << "\t"
            << mu << "\t"
            << gamma << "\t"
            << solver.LB() << "\t"
            << solver.UB() << "\t"
            << timer.realTime()
            << std::endl;
  
  if (!outputDirectory.empty())
  {
    snprintf(buf, 1024, "%s/%sT-%s.dot",
             outputDirectory.c_str(),
             outputPrefix.c_str(),
             primary.c_str());
    std::ofstream outT(buf);
    T.writeDOT(outT, solver.lPlus(), colorMap);
    outT.close();
    
    snprintf(buf, 1024, "%s/%sG-%s.dot",
             outputDirectory.c_str(),
             outputPrefix.c_str(),
             primary.c_str());
    std::ofstream outG(buf);
    G.writeDOT(outG, colorMap);
    outG.close();

    snprintf(buf, 1024, "%s/%sG-%s.tree",
             outputDirectory.c_str(),
             outputPrefix.c_str(),
             primary.c_str());
    std::ofstream outGraph(buf);
    G.write(outGraph);
    outGraph.close();
    
    snprintf(buf, 1024, "%s/%sT-%s.tree",
             outputDirectory.c_str(),
             outputPrefix.c_str(),
             primary.c_str());
    std::ofstream outTree(buf);
    T.write(outTree);
    outTree.close();
    
    snprintf(buf, 1024, "%s/%sT-%s.labeling",
             outputDirectory.c_str(),
             outputPrefix.c_str(),
             primary.c_str());
    std::ofstream outLabeling(buf);
    T.writeVertexLabeling(outLabeling, solver.lPlus());
    outLabeling.close();
  }
  
  return mu;
}
