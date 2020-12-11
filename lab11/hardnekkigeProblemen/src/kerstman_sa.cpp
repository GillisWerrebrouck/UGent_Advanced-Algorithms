#include "kerstman_sa.h"
#include <math.h>
#include <random>
#include <chrono>
#include <iostream>

using std::cout;
using std::endl;

using namespace std;

std::vector<int> kerstman_sa::beginOplossing()
{
    vector<int> solution;

    for (int i = 0; i < d.size(); i++) {
        solution.push_back(i);
    }

    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(solution.begin(), solution.end(), std::default_random_engine(seed));

    return solution;
};

std::vector<int> kerstman_sa::neemRandom(const std::vector<int> &s)
{
    vector<int> newSolution(s);

    int randA = rand() * ((double)(s.size() - 1) / RAND_MAX);
    int randB = rand() * ((double)(s.size() - 1) / RAND_MAX);

    int r = min(randA, randB);
    int l = max(randA, randB);

    while(r < l) {
        swap(newSolution[r++], newSolution[l--]);
    }

    return newSolution;
};

double kerstman_sa::f(const std::vector<int> &s)
{
    double totalDistance = 0;
    for (int i = 1; i < s.size(); i++) {
        totalDistance += this->d[s[i-1]][s[i]];
    }
    totalDistance += this->d[s[s.size() - 1]][s[0]];

    return totalDistance;
};

double kerstman_sa::p(double T, double deltaf)
{
    if (deltaf < 0) {
        return 1.0;
    }
    return exp(-deltaf/T);
};

void kerstman_sa::updateT()
{
    T *= 0.99999;
};

bool kerstman_sa::stopConditie() // true als simulated annealing moet stoppen
{
    return (T < 0.2);
};

double kerstman_sa::beginTemperatuur()
{
    return 100;
};
