#include "solution.h"

#include <iostream>
#include <cstdlib>
#include <c++/cmath>
#include <functional>

solution::solution(precision p) {
    unsigned int bitsize = static_cast<unsigned int>(p);
    data.resize(bitsize);

    for(unsigned int i=0;i<bitsize;i++)
        data[i] = (rand() % 2 == 1);
}

double solution::decode(double min, double max) {
    double value = min, step = max - min;

    switch(data.size()) {
        case 64:
            step /= 65536;
            step /= 65536;
        case 32:
            step /= 65536;
        case 16:
            step /= 65536;
            break;
        case 8:
            step /= 256;
            break;
        default:
            return nan("");
    }

    unsigned int i;
    for(i=0;i<16&&i<data.size();i++)
        if(data[i]) value += step * (1 << i);
    for(i=16;i<32&&i<data.size();i++)
        if(data[i]) value += step * (1 << (i - 16)) * (1 << 16);
    for(i=32;i<48&&i<data.size();i++)
        if(data[i]) value += step * (1 << (i - 32)) * (1 << 16) * (1 << 16);
    for(i=48;i<64&&i<data.size();i++)
        if(data[i]) value += step * (1 << (i - 48)) * (1 << 16) * (1 << 16) * (1 << 16);

    return value;
}

void solution::setRawFitness(double raw_fitness) {
    _raw_fitness = raw_fitness;
}

void solution::setDensity(double density) {
    _density = density;
}

void solution::setFitness(double fitness) {
    _fitness = fitness;
}

double solution::getRawFitness() {
    return _raw_fitness;
}

double solution::getDensity() {
    return _density;
}

double solution::getFitness() {
    return _fitness;
}

void solution::clearObjectives() {
    objectives.clear();
}

void solution::setNumObjectives(unsigned int n) {
    objectives.clear();
    objectives.resize(n);
}

void solution::setObjective(int index, double value) {
    objectives[index] = value;
}

void solution::decodeObjective(int index, double min, double max, const std::function<double(double)> &f) {
    objectives[index] = f(decode(min, max));
}

int solution::getNumObjectives() {
    return objectives.size();
}

double solution::getObjective(int index) {
    return objectives[index];
}

void solution::clearDominates() {
    _dominates.clear();
}

void solution::setDominates(int index) {
    _dominates.insert(index);
}

double solution::getDominatesSize() {
    return static_cast<double>(_dominates.size());
}

double solution::euclideanDistance(solution &s2) {
    double sum = 0;

    if(getNumObjectives() != s2.getNumObjectives())
        return nan("");

    int i;
    for(i=0;i<getNumObjectives();i++)
        sum += (getObjective(i) - s2.getObjective(i)) * (getObjective(i) - s2.getObjective(i));

    return sqrt(sum);
}

bool solution::dominates(solution &s2) {
    if(getNumObjectives() != s2.getNumObjectives())
        return false;

    int i;
    for(i=0;i<getNumObjectives();i++)
        if(getObjective(i) > s2.getObjective(i))
            return false;

    return true;
}

void solution::crossoverAndMutate(solution &s2, solution *result, float p_cross) {
    double r = (static_cast<double>(rand()) / (RAND_MAX)), m_rate = 1/data.size();
    if(r > p_cross)
        result->data = data;
    else {
        unsigned int i;
        result->data.resize(data.size());
        for(i=0;i<data.size();i++) {
            // crossover
            r = (static_cast<double>(rand()) / (RAND_MAX));
            result->data[i] = (r < 0.5)?data[i]:s2.data[i];

            // mutate
            r = (static_cast<double>(rand()) / (RAND_MAX));
            if(r < m_rate)
                result->data[i] = !result->data[i];
        }
    }
}

bool solution::operator ==(solution &s2) {
    if(data.size() != s2.data.size())
        return false;

    unsigned int i;
    for(i=0;i<data.size();i++)
        if(data[i] != s2.data[i])
            return false;

    return true;
}

bool solution::operator <(solution &s2) {
    double sum1 = 0.0f, sum2 = 0.0f;

    if(getNumObjectives() != s2.getNumObjectives())
        return false;

    int i;
    for(i=0;i<getNumObjectives();i++) {
        sum1 += getObjective(i);
        sum2 += s2.getObjective(i);
    }

    return sum1 < sum2;
}

std::ostream &operator <<(std::ostream &os, solution &s) {
    os << '[';
    for(auto it=s.data.rbegin();it!=s.data.rend();it++)
        os << (*it?'1':'0');
    os << "] (" << *(s.objectives.begin());
    for(auto it=s.objectives.begin() + 1;it!=s.objectives.end();it++)
        os << ", " << *it;
    os << ") => { raw_fitness: " << s._raw_fitness << ", density: " << s._density << ", fitness: " << s._fitness << " }";
    return os;
}
