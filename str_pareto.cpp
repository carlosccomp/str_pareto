#include "str_pareto.h"

#include <iostream>
#include <algorithm>
#include <c++/cmath>
#include <cstdlib>

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
    std::vector <solution *> union_, environment;
    union_.reserve(pop.size() + archive.size());
    environment.reserve(pop.size() + archive.size());
    for(auto it=pop.begin();it!=pop.end();it++)
        union_.push_back(&(*it));
    for(auto it=archive.begin();it!=archive.end();it++)
        union_.push_back(&(*it));

    std::sort(union_.begin(), union_.end(), [](solution *s1, solution *s2){ return (*s1).getFitness() < (*s2).getFitness(); });

    auto it = union_.begin();
    for(;it!=union_.end();it++) {
        if((*it)->getFitness() < 1.0f)
            environment.push_back(*it);
    }

    if(environment.size() < archive_size) {
        unsigned int i;
        for(i=environment.size();i<archive_size;i++)
            environment.push_back(union_[i]);
    }
    else {
        while(environment.size() > archive_size) {
            std::vector <double> densities;
            densities.resize(environment.size());
            for(auto it2=environment.begin();it2!=environment.end();it2++)
                densities[it2 - environment.begin()] = calculate_density(*(*it2), environment);
            auto max_ = std::max_element(densities.begin(), densities.end());
            environment.erase(environment.begin() + (max_ - densities.begin()));
        }
    }

    std::vector <solution> archive_aux;
    for(auto it2=environment.begin();it2!=environment.end();it2++)
        archive_aux.push_back(*(*it2));
    archive.clear();
    archive.reserve(archive_size);
    for(auto it2=archive_aux.begin();it2!=archive_aux.end();it2++)
        archive.push_back(*it2);
}

void str_pareto::reproduce() {
    std::vector <solution> children;
    children.reserve(pop_size + 1);

    while(children.size() < pop_size) {
        int parent1 = rand() % archive_size, parent2 = rand() % archive_size;
        while(parent2 == parent1)
            parent2 = rand() % archive_size;

        solution s_children;
        archive[parent1].crossoverAndMutate(archive[parent2], &s_children, p_cross);
        children.push_back(s_children);
        archive[parent2].crossoverAndMutate(archive[parent1], &s_children, p_cross);
        children.push_back(s_children);
    }

    pop.clear();
    pop = children;
}

void str_pareto::run() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cerr.tie(nullptr);

    pop.resize(pop_size);

    int i = 0;
    while(true) {
        calculate_fitness();
        environmental_selection();
        std::sort(archive.begin(), archive.end(), [](solution &s1, solution &s2){ return s1 < s2; });

        std::cout << i << " => " << archive[0] << std::endl;

        if(i >= max_gens) break;

        reproduce();
        i++;
    }
}
