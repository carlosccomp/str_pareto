#ifndef STR_PARETO_SOLUTION_H
#define STR_PARETO_SOLUTION_H

#include <vector>
#include <set>
#include <iostream>
#include <functional>
#include <c++/cmath>

enum precision {
    LOW_PRECISION = 8,
    STD_PRECISION = 16,
    HIGH_PRECISION = 32,
    ULTRA_PRECISION = 64
};

class solution {
private:
    std::vector<bool> data;
    double _raw_fitness, _density, _fitness;
    std::vector <double> objectives;
    std::set <int> _dominates;
public:
    solution(precision p = STD_PRECISION);
    double decode(double min, double max);

    void setRawFitness(double raw_fitness);
    void setDensity(double density);
    void setFitness(double fitness);
    double getRawFitness();
    double getDensity();
    double getFitness();

    void clearObjectives();
    void setNumObjectives(unsigned int n);
    void setObjective(int index, double value);
    void decodeObjective(int index, double min, double max, const std::function<double(double)> &f);
    int getNumObjectives();
    double getObjective(int index);

    void clearDominates();
    void setDominates(int index);
    double getDominatesSize();

    double euclideanDistance(solution &s2);

    bool dominates(solution &s2);

    bool operator ==(solution &s2);
    bool operator <(solution &s2);
    friend std::ostream &operator <<(std::ostream &os, solution &s);
};

#endif
