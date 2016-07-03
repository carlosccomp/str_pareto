#ifndef STR_PARETO_STR_PARETO_H
#define STR_PARETO_STR_PARETO_H

#include <vector>

#include "solution.h"

class str_pareto {
private:
    int problem_size = 1;
    double search_space_min = -10;
    double search_space_max = 10;
    int max_gens = 10;
    unsigned int pop_size = 10;
    unsigned int archive_size = 3;
    float p_cross = 0.9;

    unsigned int n_params = 2;
    precision bitsize = STD_PRECISION;

    std::vector <solution> pop, archive;
public:
    void calculate_objectives();
    void calculate_dominated(std::vector <solution *> &pop_);
    double calculate_raw_fitness(solution &s, std::vector <solution *> &pop_);
    double calculate_density(solution &s, std::vector <solution *> &pop_);
    void calculate_fitness();
    void environmental_selection();
    void reproduce();

    void run();
};

#endif
