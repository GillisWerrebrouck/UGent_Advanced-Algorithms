#pragma once

#include "simulatedannealing.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <math.h>  

/**
 * Zoekt een oplossing voor Travelling Salesman Problem, 
 * gegeven een vierkante vector<vector<double>> van afstanden
 */
class kerstman_sa : public SimulatedAnnealing<std::vector<int>>
{
public:
    kerstman_sa(std::vector<std::vector<double>> afstanden) : d(afstanden){};

    virtual std::vector<int> beginOplossing();
    virtual std::vector<int> neemRandom(const std::vector<int> &s);

    virtual double f(const std::vector<int> &s);
    virtual double p(double T, double deltaf);

    virtual void updateT();
    virtual double beginTemperatuur(); // start temperatuur
    virtual bool stopConditie();       // true als simulated annealing moet stoppen

    std::vector<std::vector<double>> d;
};
