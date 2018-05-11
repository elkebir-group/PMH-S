/*
 * dpsolver.h
 *
 *  Created on: 10-may-2018
 *      Author: M. El-Kebir
 */

#ifndef DPSOLVER_H
#define DPSOLVER_H

#include "utils.h"
#include "migrationtree.h"
#include "solver.h"

class DpSolver : public Solver
{
public:
  /// Constructor
  ///
  /// @param CloneTree Clone tree
  /// @param primary Primary location
  DpSolver(const CloneTree& T,
           const std::string& primary);
  
  /// Solve and return migration number
  ///
  /// @param nrThreads Number of threads
  /// @param timeLimit Time limit in seconds
  int solve(int nrThreads,
            int timeLimit);

  /// Solve and return migration number
  ///
  /// @param hatG Migration tree
  int solve(const MigrationTree& hatG)
  {
    _mu = 0;
    solve(hatG, _lPlus, _T.root(), _mu);
    return _mu;
  }
  
  double LB() const
  {
    return _mu;
  }
  
  double UB() const
  {
    return _mu;
  }
 
private:
  /// Solve
  ///
  /// @param hatG Migration tree
  /// @param labeling Node labeling
  /// @param v Node
  /// @param mu Migration number
  void solve(const MigrationTree& hatG,
             StringNodeMap& labeling,
             Node v,
             int& mu);
  
private:
  /// Migration number
  int _mu;
};

#endif // DPSOLVER_H
