#include "sodor.h"
#include "graaf.h"
#include "stroomnet.h"
#include <map>
#include <iostream>
#include <string>

using namespace std;

int zoekMinimaleStationsluitingen(const std::vector<sodor::TrainStation> &stations,
                                  const std::string &startStationNaam, const std::string &eindStationNaam){
    GraafMetTakdata<GERICHT, int> g;

    // station name => station index in file (not index in graph)
    map<string, int> mapping;

    // create 2 nodes with a connection for each station
    for (int i = 0; i < stations.size(); i++) {
        int stationIn = g.voegKnoopToe();
        int stationOut = g.voegKnoopToe();
        g.voegVerbindingToe(stationIn, stationOut, 1);
        // add station name to station index mapping
        mapping.insert(pair<string, int>(stations[i].name, i));
    }

    // create all connections between stations and their destinations
    for (int i = 0; i < stations.size(); i++) {
        // stationOut
        int stationIndex = i*2 + 1;

        for (int d = 0; d < stations[i].destinations.size(); d++) {
            // stationIn
            int destinationIndex = mapping.find(stations[i].destinations[d].name)->second*2;

            if (g.verbindingsnummer(stationIndex, destinationIndex) == -1)
                g.voegVerbindingToe(stationIndex, destinationIndex, 1);
        }
    }

    // create a flow network
    Stroomnetwerk<int> sn(g, mapping.find(startStationNaam)->second*2+1, mapping.find(eindStationNaam)->second*2);
    Stroomnetwerk<int> stroom = sn.geefStroom();

    // the capacity in the network is equal to the vertex-connectivity (here edge-connectivity, that's why each station has 2 nodes in the graph)
    return stroom.geefCapaciteit();
}