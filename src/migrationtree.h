/*
 * migrationtree.h
 *
 *  Created on: 19-oct-2016
 *      Author: M. El-Kebir
 */

#ifndef MIGRATIONTREE_H
#define MIGRATIONTREE_H

#include "utils.h"
#include "basetree.h"

class MigrationTree;

/// List of migration trees
typedef std::list<MigrationTree> MigrationTreeList;

/// Vector of edge lists
typedef std::vector<StringPairList> EdgeListVector;

/// This class models a migration (multi)tree
class MigrationTree : public BaseTree
{
public:
  /// Default constructor
  MigrationTree();
  
  /// Constructor
  ///
  /// @param locationSet Set of locations
  /// @param primary Primary location
  /// @param pruferSeq Prufer sequence
  MigrationTree(const StringSet& locationSet,
                const std::string& primary,
                const StringVector& pruferSeq);
  
  /// Copy constructor
  MigrationTree(const MigrationTree& other);
  
  /// Constructor
  ///
  /// @param T Directed graph
  /// @param root Root node
  /// @param id Node identifier
  MigrationTree(const SubDigraph& T,
                Node root,
                const StringNodeMap& id);
  
  /// Destructor
  ~MigrationTree();
  
  /// Return the leaf label of the given node
  ///
  /// @param x Node
  const std::string& l(Node x) const
  {
    return _nodeToId[x];
  }
  
  /// Read vertex labeling
  ///
  /// @param in Input stream
  bool readVertexLabeling(std::istream& in);
  
  /// Write vertex labeling
  ///
  /// @param out Output stream
  void writeVertexLabeling(std::ostream& out) const;
  
  /// Print tree in DOT format
  ///
  /// @param out Output stream
  void writeDOT(std::ostream& out) const;
  
  /// Print tree in DOT format
  ///
  /// @param out Output stream
  /// @param colorMap Color map
  void writeDOT(std::ostream& out,
                const StringToIntMap& colorMap) const;
  
  /// Return number of locations
  int getNrLocations() const
  {
    return _idToNode.size();
  }
  
  /// Return location labels
  StringSet getLocations() const
  {
    StringSet res;
    for (const auto& kv : _idToNode)
    {
      res.insert(kv.first);
    }
    return res;
  }
  
  /// Deserialize
  ///
  /// @param in Input stream
  /// @param primary Primary tumor
  bool readWithPrimary(std::istream& in, const std::string& primary);
  
  /// Enumerate all migration trees given location set and primary location
  ///
  /// @param primary Primary location
  /// @param locationSet Set of locations
  /// @param listBarS Placeholder for storing all enumerated migration trees
  static void enumerate(const std::string& primary,
                        const StringSet& locationSet,
                        MigrationTreeList& listBarS);
  
  /// Enumerate all migration trees given anatomical sites
  ///
  /// @param primary Primary tumor anatomical site
  /// @param metastases Metastatic anatomical sites
  /// @param migrationTrees Placeholder for storing all enumerated migration trees
  static void enumerate(const std::string& primary,
                        const StringSet& metastases,
                        EdgeListVector& migrationTrees);
  
  /// Generate next Prufer sequence
  ///
  /// @param seq Current Prufer sequence
  static bool next(IntVector& seq)
  {
    const int m = seq.size() + 2;
    int i = 0;
    for (; i < seq.size() && seq[i] == m - 1; ++i);
    
    if (i == seq.size())
    {
      return false;
    }
    else
    {
      ++seq[i];
      for (int j = 0; j < i; ++j)
      {
        seq[j] = 0;
      }
      return true;
    }
  }

private:
  /// Serialize migration trees
  ///
  /// @param out Output stream
  /// @param listBarS List of migration trees
  friend std::ostream& operator<<(std::ostream& out,
                                  const MigrationTreeList& listBarS);

  /// Deserialize migration trees
  ///
  /// @param in Input stream
  /// @param listBarS Placeholder for storing migration trees
  friend std::istream& operator>>(std::istream& in,
                                  MigrationTreeList& listBarS);
};

/// Serialize
///
/// @param out Output stream
/// @param listBarS Migration trees
std::ostream& operator<<(std::ostream& out,
                         const MigrationTreeList& listBarS);

/// Deserialize
///
/// @param in Input stream
/// @param listBarS Placeholder for storing migration trees
std::istream& operator>>(std::istream& in,
                         MigrationTreeList& listBarS);

/// Deserialize
///
/// @param in Input stream
/// @param listBarS Placeholder for storing migration trees
std::istream& operator>>(std::istream& in,
                         EdgeListVector& listBarS);

#endif // MIGRATIONTREE_H
