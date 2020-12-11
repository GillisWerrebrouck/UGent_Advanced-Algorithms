#include "kerstman_prob.h"
#include "probprob.h"
#include <iostream>
#include <algorithm> 
#include <unordered_set> 

using std::cout;
using std::endl;

using namespace std;

double kerstman_prob::f(const vector<int> &s)
{
    double totalDistance = 0;
    for (int i = 1; i < s.size(); i++) {
        totalDistance += this->d[s[i-1]][s[i]];
    }
    totalDistance += this->d[s[s.size() - 1]][s[0]];

    return totalDistance;
};

bool kerstman_prob::isOplossing(const vector<int> &deeloplossing)
{
    return deeloplossing.size() == this->d.size();
};

vector<pair<int, double>> kerstman_prob::geefVolgendePunt(const vector<int> &deeloplossing)
{
    vector<pair<int, double>> possibleNext;

    if (deeloplossing.size() == 0) {
        for (int i = 0; i < d.size(); i++) {
            possibleNext.push_back(pair<int, double>(i, 1));
        }
    } else {
        const int from = deeloplossing[deeloplossing.size() - 1];

        unordered_set<int> lookup;

        for (int i = 0; i < deeloplossing.size(); i++) {
            lookup.insert(deeloplossing[i]);
        }

        for (int i = 0; i < d.size(); i++) {
            if (-1 < this->d[from][i] && from != i && lookup.find(i) == lookup.end()) {
                possibleNext.push_back(pair<int, double>(i, this->d[from][i]));
            }
        }
    }

    return possibleNext;
};

vector<int> kerstman_prob::pickAtRandom(const vector<int> &oplossing)
{
    vector<int> newSolution(oplossing);

    int randA = rand() * ((double)(oplossing.size() - 1) / RAND_MAX);
    int randB = rand() * ((double)(oplossing.size() - 1) / RAND_MAX);

    int r = min(randA, randB);
    int l = max(randA, randB);

    while(r < l) {
        swap(newSolution[r++], newSolution[l--]);
    }

    return newSolution;
};

void kerstman_prob::verbeterLokaal(vector<int> &oplossing)
{
    srand(time(0));

    double currDistance, newDistance = 0;
    vector<int> newPossibleSolution;

    do {
        currDistance = f(oplossing);
        newPossibleSolution = pickAtRandom(oplossing);
        newDistance = f(newPossibleSolution);

        if (newDistance < currDistance) {
            oplossing = move(newPossibleSolution);
        }
    } while (750 < newDistance);
};
