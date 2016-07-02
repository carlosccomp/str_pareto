#include "str_pareto.h"

#include <iostream>
#include <algorithm>
#include <c++/cmath>

void str_pareto::calculate_objectives() {
    for(auto it=pop.begin();it!=pop.end();it++) {
        it->clearObjectives();
        it->setNumObjectives(2);
        it->decodeObjective(0, search_space_min, search_space_max, [](double x) { return x * x; });
        it->decodeObjective(1, search_space_min, search_space_max, [](double x) { return (x-2) * (x-2); });
    }
}

void str_pareto::calculate_dominated(std::vector<solution *> &pop_) {
    for(auto it=pop_.begin();it!=pop_.end();it++) {
        (*it)->clearDominates();
        for(auto it2=pop_.begin();it2!=pop_.end();it2++) {
            if((it - pop_.begin()) == (it2 - pop_.begin()))
                continue;

            if((*it)->dominates(*(*it2)))
                (*it)->setDominates(it2 - pop_.begin());
        }
    }
}

double str_pareto::calculate_raw_fitness(solution &s, std::vector<solution *> &pop_) {
    double sum = 0;

    for(auto it=pop_.begin();it!=pop_.end();it++) {
        // pequena alteração do algoritmo do livro
        if(s == *(*it))
            continue;
        // fim da pequena alteração

        if((*it)->dominates(s))
            sum += (*it)->getDominatesSize();
    }

    return sum;
}

double str_pareto::calculate_density(solution &s, std::vector<solution *> &pop_) {
    std::vector <double> dist;
    dist.resize(pop_.size());

    for(auto it=pop_.begin();it!=pop_.end();it++)
        dist[it - pop_.begin()] = (*it)->euclideanDistance(s);

    std::sort(dist.begin(), dist.end());
    int k = static_cast<int>(sqrt(pop_.size()));

    return 1.0f / (dist[k] + 2.0f);
}

void str_pareto::calculate_fitness() {
    calculate_objectives();

    std::vector <solution *> union_;
    union_.reserve(pop.size() + archive.size());
    for(auto it=pop.begin();it!=pop.end();it++)
        union_.push_back(&(*it));
    for(auto it=archive.begin();it!=archive.end();it++)
        union_.push_back(&(*it));
    calculate_dominated(union_);

    for(auto it=union_.begin();it!=union_.end();it++) {
        (*it)->setRawFitness(calculate_raw_fitness(*(*it), union_));
        (*it)->setDensity(calculate_density(*(*it), union_));
        (*it)->setFitness((*it)->getRawFitness() + (*it)->getDensity());
    }
}

void str_pareto::environmental_selection() {
    std::vector <solution> union_;
    union_.reserve(pop.size() + archive.size());
    union_.insert(union_.end(), pop.begin(), pop.end());
    union_.insert(union_.end(), archive.begin(), archive.end());

    for(auto it=union_.begin();it!=union_.end();it++)
        std::cout << *it << std::endl;

    std::cout << std::endl << "after selection: " << std::endl << std::endl;

    /*std::remove_if(union_.begin(), union_.end(), [](solution &s){ return s.getFitness() >= 1.0f; });

    for(auto it=union_.begin();it!=union_.end();it++)
        std::cout << *it << std::endl;

    std::cout << "end" << std::endl;*/
}

void str_pareto::run() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cerr.tie(nullptr);

    pop.resize(static_cast<unsigned int>(pop_size));

    int i = 0;
    //while(true) {
        calculate_fitness();
        environmental_selection();
        // TODO sorting

        //if(i >= max_gens) break;

        // TODO tournament
        // TODO reproduce
        //i++;
    //}
}
