#include "euler.h"

bool heeft_eulercircuit(const Graaf<ONGERICHT>& g){
    int odd = 0;

    // iterate over all nodes
    for (int nodeIndex = 0; nodeIndex < g.aantalKnopen(); nodeIndex++) {
        int childCount = 0;
        auto childIt = g[nodeIndex].begin();
        // count the edges for this node
        while (childIt++ != g[nodeIndex].end())
            childCount++;
        // check if the amount of edges is odd
        if (childCount % 2 != 0) odd++;
    }

    // an euler circuit can't have any node with an odd number of edges
    return odd == 0;
}

bool heeft_eulercircuit(const Graaf<GERICHT>& g){
    std::vector<int> outgoingCount(g.aantalKnopen(), 0);
    std::vector<int> incomingCount(g.aantalKnopen(), 0);

    for (int nodeIndex = 0; nodeIndex < g.aantalKnopen(); nodeIndex++) {
        // get outgoing edge count
        outgoingCount[nodeIndex] = g[nodeIndex].size();

        auto childIt = g[nodeIndex].begin();
        while (childIt != g[nodeIndex].end()) {
            // check for outgoing edge and increase incoming edge for neighbours
            if (-1 < g.verbindingsnummer(nodeIndex, childIt->first)) incomingCount[childIt->first]++;

            childIt++;
        }
    }

    // check if all nodes have an equal number of outgoing edges as incoming edges
    for (int nodeIndex = 0; nodeIndex < g.aantalKnopen(); nodeIndex++) {
        if (outgoingCount[nodeIndex] != incomingCount[nodeIndex]) return false;
    }

    return true;
}

// recursively build an euler circuit with the given start node
bool eulercircuitRecursive(const Graaf<GERICHT>& g, int nodeIndex, std::vector<int> &circuit, std::map<int, bool> visited){
    // the euler circuit is complete if it contains all edges from the graph
    if (circuit.size() == g.aantalVerbindingen()) {
        return true;
    }

    auto childIt = g[nodeIndex].begin();
    // iterate over all child nodes
    while (childIt != g[nodeIndex].end()) {
        int edgeNr = g.verbindingsnummer(nodeIndex, childIt->first);
        // check if not yet visited before
        if (visited.find(edgeNr) != visited.end()) {
            childIt++;
            continue;
        }

        // add as a possible next edge
        circuit.push_back(edgeNr);
        visited.insert(std::pair<int, bool>(edgeNr, true));

        // build the rest of this circuit
        bool found = eulercircuitRecursive(g, childIt->first, circuit, visited);
        if (found) return true;

        // remove the last edge (backtrack)
        circuit.pop_back();
        visited.erase(visited.find(edgeNr));

        childIt++;
    }

    return false;
}

/**
 * Geeft een mogelijk Eulercircuit terug
 * 
 * @returns een geordende lijst met verbindingsnummers die
 *          een Eulercircuit vormen
 */ 
std::vector<int> eulercircuit(const Graaf<GERICHT>& g){
    if(!heeft_eulercircuit(g)){
        throw std::domain_error("De opgegeven graaf bevat geen Eulercircuit");
    }

    std::vector<int> circuit;
    std::map<int, bool> visited;

    // try each node as a possible starting node
    for (int nodeIndex = 0; nodeIndex < g.aantalKnopen(); nodeIndex++) {
        // try to build an euler circuit with the current node as starting node
        bool found = eulercircuitRecursive(g, nodeIndex, circuit, visited);
        if (found) break;
    }

    return circuit;
}
