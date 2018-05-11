/*
 * cpsolver.cpp
 *
 *  Created on: 30-apr-2018
 *      Author: M. El-Kebir
 */

#include "cpsolver.h"
#include "utils.h"

CpSolver::CpSolver(const CloneTree& T,
                   const std::string& primary)
  : Solver(T, primary)
  , _lStarVar(_T.tree(), NULL)
  , _migrationVar(_T.tree(), NULL)
  , _objVar(NULL)
  , _allVars()
  , _locationToIdx()
  , _idxToLocation()
  , _primaryIndex(-1)
  , _solver("PMH-S-CP")
{
}

void CpSolver::initVariables()
{
  char buf[1024];
  
  const StringSet& Sigma = _T.getLocations();
  
  _locationToIdx.clear();
  _idxToLocation.clear();
  for (const std::string& s : Sigma)
  {
    _locationToIdx[s] = _idxToLocation.size();
    _idxToLocation.push_back(s);
  }
  
  _primaryIndex = _locationToIdx.find(_primary)->second;
  
  _allVars.clear();
  for (ArcIt a(_T.tree()); a != lemon::INVALID; ++a)
  {
    snprintf(buf, 1024, "x:%s:%s",
             _T.label(_T.tree().source(a)).c_str(),
             _T.label(_T.tree().target(a)).c_str());
    _migrationVar[a] = _solver.MakeIntVar(0, 1, buf);
    _allVars.push_back(_migrationVar[a]);
  }
  
  for (NodeIt v(_T.tree()); v != lemon::INVALID; ++v)
  {
    snprintf(buf, 1024, "l:%s", _T.label(v).c_str());
    _lStarVar[v] = _solver.MakeIntVar(0, Sigma.size() - 1, buf);
    _allVars.push_back(_lStarVar[v]);
  }
  
  _objVar = _solver.MakeIntVar(0, lemon::countArcs(_T.tree()), "obj");
  _allVars.push_back(_objVar);
}

void CpSolver::initConstraints()
{
  for (Node u : _T.leafSet())
  {
    const std::string& s = _T.l(u);
    assert(_locationToIdx.count(s) == 1);
    const int ss = _locationToIdx.find(s)->second;
    _solver.AddConstraint(_solver.MakeEquality(_lStarVar[u], ss));
  }
  
  _solver.AddConstraint(_solver.MakeEquality(_lStarVar[_T.root()], _primaryIndex));
  
  IntVarArray migrationVars;
  for (ArcIt a(_T.tree()); a != lemon::INVALID; ++a)
  {
    Node u = _T.tree().source(a);
    Node v = _T.tree().target(a);
    _solver.AddConstraint(_solver.MakeIsDifferentCt(_lStarVar[u], _lStarVar[v], _migrationVar[a]));
    migrationVars.push_back(_migrationVar[a]);
    
    for (ArcIt b(_T.tree()); b != lemon::INVALID; ++b)
    {
      Node uu = _T.tree().source(b);
      Node vv = _T.tree().target(b);
      
      if (u == uu) continue;
      
      if (_T.tree().id(a) < _T.tree().id(b))
      {
        IntVarArray expression;
        expression.push_back(_solver.MakeIsEqualVar(_lStarVar[u], _lStarVar[v]));
        expression.push_back(_solver.MakeIsEqualVar(_lStarVar[uu], _lStarVar[vv]));
        expression.push_back(_solver.MakeIsDifferentVar(_lStarVar[v], _lStarVar[vv]));
        expression.push_back(_solver.MakeIsEqualVar(_lStarVar[u], _lStarVar[uu]));
        _solver.AddConstraint(_solver.MakeEquality(_solver.MakeMax(expression), 1));
      }
    }
  }
  
  _solver.AddConstraint(_solver.MakeEquality(_objVar, _solver.MakeSum(migrationVars)));
}

int CpSolver::solve(int nrThreads,
                    int timeLimit)
{
  initVariables();
  initConstraints();
  
  IntVarArray varSubSet;
  for (NodeIt v(_T.tree()); v != lemon::INVALID; ++v)
  {
    varSubSet.push_back(_lStarVar[v]);
  }
  
  // Create decision builder to search for solutions.
  google_or::DecisionBuilder* const db = _solver.MakePhase(varSubSet,
                                                          google_or::Solver::CHOOSE_FIRST_UNBOUND,
                                                          google_or::Solver::ASSIGN_MIN_VALUE);
  // Search.
  google_or::OptimizeVar* pObjective = _solver.MakeMinimize(_objVar, 1);
//  google_or::SolutionCollector* pCollector = _solver.MakeAllSolutionCollector();
  google_or::SolutionCollector* pCollector = _solver.MakeLastSolutionCollector();
  pCollector->Add(_allVars);
  pCollector->AddObjective(_objVar);
  
  if (timeLimit <= 0)
  {
    _solver.Solve(db, pCollector, pObjective);
  }
  else
  {
    google_or::SearchLimit* pLimit = _solver.MakeTimeLimit(timeLimit * 1000);
    _solver.Solve(db, pCollector, pObjective, pLimit);
  }
  
  assert(pCollector->solution_count() > 0);
  
  for (NodeIt v(_T.tree()); v != lemon::INVALID; ++v)
  {
    _lPlus[v] = _idxToLocation[pCollector->Value(0, _lStarVar[v])];
  }
  
  _mu = pCollector->Value(0, _objVar);
  return _mu;

//  for (int count = 0; count <  pCollector->solution_count(); ++count)
//  {
//    std::cout << "Solution " << count + 1;
//    for (google_or::IntVar* x : _allVars)
//    {
//      if (x->name()[0] != 'x')
//      {
//        std::cout << " " << x->name() << "=" << pCollector->Value(count, x);
//      }
//    }
//    std::cout << std::endl;
//  }
}
