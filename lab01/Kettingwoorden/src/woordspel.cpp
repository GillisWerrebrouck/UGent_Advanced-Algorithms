#include "graaf.hpp"
#include "keten.hpp"
#include <unordered_set>
#include <algorithm>
#include <stack>
#include <optional>
#include <string>
#include <map>

GraafMetKnoopEnTakdata<GERICHT, std::string, std::string> leesGraaf(std::string woordenlijstFilename, std::string takkenlijstFilename)
{
    GraafMetKnoopEnTakdata<GERICHT, std::string, std::string> woorden;
    std::unordered_map<std::string, int> woord_nr;

    std::ifstream in(woordenlijstFilename);
    std::string woord;
    while (in >> woord)
    {
        woord_nr[woord] = woorden.voegKnoopToe(woord);
    }

    std::cout << woord_nr.size() << " woorden ingelezen" << std::endl;

    in.close();
    in.clear();
    in.open(takkenlijstFilename);

    int genegeerd_count = 0;

    std::string van;
    std::string naar;
    while (in >> van >> naar >> woord)
    {
        int van_nr = woord_nr.at(van);
        int naar_nr = woord_nr.at(naar);

        if (woorden.verbindingsnummer(van_nr, naar_nr) == -1)
        {
            woorden.voegVerbindingToe(van_nr, naar_nr, woord);
        }
        else
        {
            genegeerd_count++;
        }
        // this is commented out to make the unit test execution result better readable
        // std::cout << std::endl;
    }

    return woorden;
}

// find the component with the most nodes
int grootste_component_nummer(const vector<int> &component_nrs)
{
    int numberOfComponents = *std::max_element(component_nrs.begin(), component_nrs.end()) + 1;
    vector<int> freq_table(numberOfComponents, 0);
    int maxValue = -1;
    int maxIndex = -1;

    // build a frequency table and keep track of the max value/index
    for (int i = 0; i < component_nrs.size(); i++) {
        freq_table[component_nrs[i]]++;
        if (maxValue < freq_table[component_nrs[i]]) {
            maxValue = freq_table[component_nrs[i]];
            maxIndex = component_nrs[i];
        }
    }

    return maxIndex;
}

// recursive backtracking algorithm to build the alphabetically first chain of words
bool findWordChainRecursive(const GraafMetKnoopEnTakdata<GERICHT, std::string, std::string> &g,
                                int nodeIndex,
                                const vector<int> &component_nrs,
                                int component_nr,
                                std::map<int, bool> &visited,
                                int firstNodeIndex,
                                vector<string> &chain)
{
    // stop if the chain is back at the first word
    if (0 < chain.size() && firstNodeIndex == nodeIndex) {
        return true;
    }

    // iterate over all child nodes of the current word
    auto child_it = g[nodeIndex].begin();
    while (child_it != g[nodeIndex].end()) {
        // only child nodes with the correct component number and that haven't been visited yet are considered
        if (component_nrs[child_it->first] == component_nr && visited.find(child_it->first) == visited.end()) {
            // add node index to visited map
            visited.insert(std::pair<int, bool>(child_it->first, true));

            // add to chain
            chain.push_back(*g.geefTakdata(nodeIndex, child_it->first));

            // recursively build chain with current child node
            bool finished = findWordChainRecursive(g, child_it->first, component_nrs, component_nr, visited, firstNodeIndex, chain);
            if (finished) return true;

            // remove from chain
            chain.pop_back();

            auto visited_it = visited.find(child_it->first);
            // remove from visited map
            visited.erase(visited_it);
        }

        // go to next child node
        child_it++;
    }

    // no more suitable child nodes, backtrack to parent node
    return false;
}

Keten eersteKringKetting(const GraafMetKnoopEnTakdata<GERICHT, std::string, std::string> &g,
                            const vector<int> &component_nrs,
                            int component_nr)
{
    // find the alphabetically first node of the 
    int nodeIndex = -1;
    while (nodeIndex++ < g.aantalKnopen()) {
        if (component_nrs[nodeIndex] == component_nr) break;
    }

    // initialize a vector to keep track of the connections between nodes of the chain
    vector<string> chain;
    // a hashmap to keep track of the visited nodes that are in the chain
    std::map<int, bool> visited;
    // build the chain
    findWordChainRecursive(g, nodeIndex, component_nrs, component_nr, visited, nodeIndex, chain);

    return Keten{chain};
}
