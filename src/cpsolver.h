/*
 * cpsolver.h
 *
 *  Created on: 30-apr-2018
 *      Author: M. El-Kebir
 */

#ifndef CPSOLVER_H

#include <ortools/constraint_solver/constraint_solveri.h>
#include "solver.h"

namespace google_or = operations_research;

/// This class models a constraint program for solving the PMH problem
/// under the restriction that the migration graph is a tree
class CpSolver : public Solver
{
public:
  /// Constructor
  ///
  /// @param CloneTree Clone tree
  /// @param primary Primary location
  CpSolver(const CloneTree& T,
           const std::string& primary);

  /// Solve and return migration number
  ///
  /// @param nrThreads Number of threads
  /// @param timeLimit Time limit in seconds
  int solve(int nrThreads,
            int timeLimit);
  
  double LB() const
  {
    return _mu;
  }
  
  double UB() const
  {
    return _mu;
  }
  
private:
  typedef std::vector<google_or::IntVar*> IntVarArray;
  typedef Digraph::NodeMap<google_or::IntVar*> IntVarNodeMap;
  typedef Digraph::ArcMap<google_or::IntVar*> IntVarArcMap;
  typedef std::map<std::string, int> StringToIntMap;
  
  /// Initialize variables
  void initVariables();
  
  /// Initialize constraints
  void initConstraints();
  
private:
  /// Vertex labeling: lStarVar[i] is location label of vertex v_i
  IntVarNodeMap _lStarVar;
  /// Migration indicator
  IntVarArcMap _migrationVar;
  /// Migration number
  google_or::IntVar* _objVar;
  /// All variables
  IntVarArray _allVars;
  /// Location to index mapping
  StringToIntMap _locationToIdx;
  /// Index to location mapping
  StringVector _idxToLocation;
  /// Index of the primary location
  int _primaryIndex;
  /// Constraint programming solver
  google_or::Solver _solver;
  /// Migration number
  int _mu;
};

#endif // CPSOLVER_H
