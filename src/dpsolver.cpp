/*
 * dpsolver.cpp
 *
 *  Created on: 10-may-2018
 *      Author: M. El-Kebir
 */

#include "dpsolver.h"

DpSolver::DpSolver(const CloneTree& T,
                   const std::string& primary)
  : Solver(T, primary)
  , _mu(std::numeric_limits<int>::max())
{
}

int DpSolver::solve(int nrThreads, int timeLimit)
{
  StringSet locationSet = _T.getLocations();
  
  StringVector Sigma;
  for (const std::string& location : locationSet)
  {
    Sigma.push_back(location);
  }
  
  const int m = locationSet.size();
  IntVector seq(m - 2, 0);
  StringVector pruferSeq(m - 2);
  do
  {
    for (int i = 0; i < m - 2; ++i)
    {
      pruferSeq[i] = Sigma[seq[i]];
    }
    
    MigrationTree hatG(locationSet, _primary, pruferSeq);
    StringNodeMap labeling(_T.tree());
    int mu = 0;
    solve(hatG, labeling, _T.root(), mu);
    
    if (mu < _mu)
    {
      bool ok = true;
      for (Node v : _T.leafSet())
      {
        ok &= _T.l(v) == labeling[v];
      }
      if (ok)
      {
        lemon::mapCopy(_T.tree(), labeling, _lPlus);
        _mu = mu;
      }
    }
    
  } while (MigrationTree::next(seq));
  
  return _mu;
}

void DpSolver::solve(const MigrationTree& hatG,
                      StringNodeMap& labeling,
                      Node v,
                      int& mu)
{
  if (_T.root() == v)
  {
    labeling[_T.root()] = _primary;
  }
  else
  {
    StringSet locationSet;
    NodeSet locationNodeSet;
    for (Node leaf : _T.leafSubset(v))
    {
      const std::string& loc = _T.l(leaf);
      locationSet.insert(loc);
      Node x = hatG.getNodeByLabel(loc);
      assert(x != lemon::INVALID);
      
      locationNodeSet.insert(x);
    }
    
    Node u = _T.parent(v);
    assert(!labeling[u].empty());
    
    Node lca = hatG.getLCA(locationNodeSet);
    
    if (labeling[u] == hatG.l(lca))
    {
      labeling[v] = labeling[u];
    }
    else
    {
      NodeList path = hatG.path(hatG.getNodeByLabel(labeling[u]), lca);
      assert(path.size() > 1);
      labeling[v] = hatG.l(*(++path.begin()));
      ++mu;
    }
  }
  
  for (OutArcIt a(_T.tree(), v); a != lemon::INVALID; ++a)
  {
    Node w = _T.tree().target(a);
    solve(hatG, labeling, w, mu);
  }
}
