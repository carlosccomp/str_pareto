#include "str_pareto.h"

#include <cstdlib>
#include <ctime>

int main(int argc, char *argv[]) {
    srand(static_cast<unsigned int>(time(NULL)));

    str_pareto problem;

    problem.run();

    return 0;
}