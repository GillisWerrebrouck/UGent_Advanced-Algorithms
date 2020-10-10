#include "stroomnet.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include "graaf.h"
#include <vector>
#include <limits>

using namespace std;

Graaf<ONGERICHT> buildGraph(string filename) {
  ifstream file(filename);
  string line;

  if (file.is_open()) {
    getline(file, line);
    Graaf<ONGERICHT> g(stoi(line));

    while (getline(file, line)) {
      const int from = stoi(line.substr(0, line.find(' ')));
      const int to = stoi(line.substr(line.find(' ') + 1));
      g.voegVerbindingToe(from, to);
    }
    return g;
  } else {
    cout << "file could not be opened" << endl;
    return Graaf<ONGERICHT>();
  }
}

bool hasAlternatingPath(Graaf<ONGERICHT> &g, int nodeIndex, int rootNodeIndex, Pad<int> &alternatingPath, std::vector<int> &matchings, bool matched) {
    // current node isn't part of a matching and is not the root
    if (matchings[nodeIndex] == -1 && nodeIndex != rootNodeIndex)
        return true;

    // add current node to alternating path
    alternatingPath.push_back(nodeIndex);

    Graaf<ONGERICHT>::Burenlijst neighbourList = g[nodeIndex];
    for (const auto &[nNodeIndex, nEdgeIndex] : neighbourList) {
      // neighbour node can not be part of the current alternating path
      if (std::find(alternatingPath.begin(), alternatingPath.end(), nNodeIndex) == alternatingPath.end()) {
        if ((!matched && matchings[nodeIndex] != nNodeIndex) || (matched && matchings[nodeIndex] == nNodeIndex)) {
          // expand alternating path and invert matched
          if (hasAlternatingPath(g, nNodeIndex, rootNodeIndex, alternatingPath, matchings, !matched)) {
            if (!matched) {
              matchings[nodeIndex] = nNodeIndex;
              matchings[nNodeIndex] = nodeIndex;
            }
            return true;
          }
        }
      }
    }

    // alternating path can not be expanded because all leaves of the tree are matched
    return false;
}

template <RichtType RT>
int countMatchingsKAlternatingPath(Graaf<RT> &g) {
  int numberOfMatchings = 0;
  vector<int> matchings(g.aantalKnopen(), -1);

  // count all matchings by construcing an alternating path starting from each node
  for (int i = 0; i < g.aantalKnopen(); i++) {
    Pad<int> alternatingPath;
    if (matchings[i] == -1 && hasAlternatingPath(g, i, i, alternatingPath, matchings, false))
      numberOfMatchings++;
  }

  return numberOfMatchings;
}

template <RichtType RT_1, RichtType RT_2, typename T>
void divideInTwoGroups(Graaf<RT_1> &g, GraafMetTakdata<RT_2, T> &gGericht, int currentNodeIndex, vector<int> &L, vector<int> &R, bool isLeft) {
  // add the node to the appropriate vector
  if (isLeft)
    L.push_back(currentNodeIndex);
  else
    R.push_back(currentNodeIndex);
  
  auto it = g[currentNodeIndex].begin();
  while (it != g[currentNodeIndex].end()) {
    // add a connection in the correct direction (L --> R)
    if (isLeft && gGericht.verbindingsnummer(currentNodeIndex, it->first) == -1)
      gGericht.voegVerbindingToe(currentNodeIndex, it->first, 1);
    else if (gGericht.verbindingsnummer(it->first, currentNodeIndex) == -1)
      gGericht.voegVerbindingToe(it->first, currentNodeIndex, 1);

    // continue with next if neighbouring node already in left or right
    if (find(L.begin(), L.end(), it->first) != L.end() || find(R.begin(), R.end(), it->first) != R.end()) {
      it++;
      continue;
    }

    // recursively divide the next nodes
    divideInTwoGroups(g, gGericht, it->first, L, R, !isLeft);
    it++;
  }
}

template <RichtType RT_1, RichtType RT_2, typename T>
int countMatchingsNetworkFlows(Graaf<RT_1> &g, GraafMetTakdata<RT_2, T> &gGericht) {
  vector<int> L;
  vector<int> R;

  divideInTwoGroups(g, gGericht, 0, L, R, true);

  int startNode = gGericht.voegKnoopToe();
  int endNode = gGericht.voegKnoopToe();

  // add connections between the start node and all left nodes
  for (int LNode : L)
    gGericht.voegVerbindingToe(startNode, LNode, 1);

  // add connections between all right nodes and the end node
  for (int RNode : R)
    gGericht.voegVerbindingToe(RNode, endNode, 1);

  Stroomnetwerk<int> sn(gGericht, startNode, endNode);
  Stroomnetwerk<int> stroom = sn.geefStroom();

  // the network capacity (all capacity 1) is equal to the max matching
  return stroom.geefCapaciteit();
}

int main()
{
  Graaf<ONGERICHT> g = buildGraph("../bipartitegraaf");
  GraafMetTakdata<GERICHT, int> gGericht(g.aantalKnopen());

  cout << "Total number of matchings: " << countMatchingsKAlternatingPath(g) << endl;
  cout << "Total number of matchings: " << countMatchingsNetworkFlows(g, gGericht) << endl;

  return 0;
}
