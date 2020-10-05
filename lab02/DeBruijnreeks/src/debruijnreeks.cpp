#include <string>
#include <cmath>
#include <vector>
#include <map>
#include "euler.h"

// recursively build the sequence
bool genereer_debruijnreeks_recursive(int alfabet_lengte, int orde, std::map<std::string, bool> &permutations, std::string &result){
    if (result.length() == std::pow(alfabet_lengte, orde)) {
        std::string temp = result[result.length()-1] + result.substr(0, orde-1);

        if (permutations.find(temp) == permutations.end())
            return true;
        return false;
    }

    // try each letter of the given alphabet
    for (int i = 0; i < alfabet_lengte; i++) {
        char currLetter = 'a' + i;

        // add the current letter to the result
        result.push_back(currLetter);

        if (result.size() < orde || permutations.find(result.substr(result.length()-orde, orde)) == permutations.end()) {
            // keep track of the permutations if the first (orde-1) characters have been seen
            if (result.size() >= orde)
                permutations.insert(std::pair<std::string, bool>(result.substr(result.length()-orde, orde), true));

            // recursively build the sequence
            bool finished = genereer_debruijnreeks_recursive(alfabet_lengte, orde, permutations, result);
            if (finished)
                return true;

            // remove the current permutation from history if the first (orde-1) characters have been seen
            if (result.size() >= orde) {
                auto permutationsIt = permutations.find(result.substr(result.length()-orde, orde));
                permutations.erase(permutationsIt);
            }
        }
        // remove the last letter (backtrack)
        result.pop_back();
    }

    return false;
}

// works as well but is less performant due to the use of recursion
std::string genereer_debruijnreeks_backtracking(int alfabet_lengte, int orde){
    std::map<std::string, bool> permutations;
    std::string result = "";

    genereer_debruijnreeks_recursive(alfabet_lengte, orde, permutations, result);

    return result;
}

// more efficient version of algorithm, compared to the one above
std::string genereer_debruijnreeks(int alfabet_lengte, int orde) {
    // create a graph with alfabet_lengte^(orde-1) nodes and edges between nodes as "state transitions"
    int nodeCount = std::pow(alfabet_lengte, orde-1); 
    GraafMetTakdata<GERICHT, int> g(nodeCount);

    // add edges
    for (int v = 0; v < nodeCount; v++) {
        int u = (v * alfabet_lengte) % nodeCount;

        for (int i = 0; i < alfabet_lengte; i++)
            g.voegVerbindingToe(v, (u + i), i);
    }

    // all nodes have an equal number of outgoing edges as incoming edges so there is an eulercircuit
    std::vector<int> circuit = eulercircuit(g);
    std::string result;

    // construct the result string from the circuit
    for(int i = 0; i < circuit.size(); i++)
        result.push_back('a' + *g.geefTakdata(circuit[i]));

    return result;
}
