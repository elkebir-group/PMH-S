/*
 * solver.h
 *
 *  Created on: 10-may-2018
 *      Author: M. El-Kebir
 */

#ifndef SOLVER_H
#define SOLVER_H

#include "utils.h"
#include "clonetree.h"

class Solver
{
public:
  /// Constructor
  ///
  /// @param CloneTree Clone tree
  /// @param primary Primary location
  Solver(const CloneTree& T,
         const std::string& primary);
  
  /// Destructor
  virtual ~Solver()
  {
  }
  
  /// Solve and return migration number
  ///
  /// @param nrThreads Number of threads
  /// @param timeLimit Time limit in seconds
  virtual int solve(int nrThreads,
                    int timeLimit) = 0;
  
  /// Return vertex labeling
  const StringNodeMap& lPlus() const
  {
    return _lPlus;
  }
  
  virtual double LB() const = 0;
  
  virtual double UB() const = 0;

  static int run(Solver& solver,
                 const CloneTree& T,
                 const std::string& primary,
                 const std::string& outputDirectory,
                 const std::string& outputPrefix,
                 const StringToIntMap& colorMap,
                 int nrThreads,
                 int timeLimit);
  
protected:
  /// Clone tree
  const CloneTree& _T;
  /// Primary location
  const std::string& _primary;
  /// Vertex labeling
  StringNodeMap _lPlus;
};

#endif // SOLVER_H
