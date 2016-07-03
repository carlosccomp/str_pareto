#include "str_pareto.h"

#include <cstdlib>
#include <ctime>
#include <fstream>

int main(int argc, char *argv[]) {
    srand(static_cast<unsigned int>(time(NULL)));

    std::cout << "opening file..." << std::endl;
    std::fstream file;
    file.open("matyas.m", std::fstream::out);
    if(file.is_open())
        std::cout << "file opened" << std::endl;
    else {
        std::cout << "file not opened" << std::endl;
        return 0;
    }

    std::cout << "starting..." << std::endl;
    file << "results_matyas = [" << std::endl;
    int i;
    for(i=0;i<200;i++) {
        str_pareto problem;
        problem.run(file);
        std::cout << "done iteration #" << (i+1) << std::endl;
    }
    file << "];" << std::endl;
    file.close();
    std::cout << "done!" << std::endl;

    return 0;
}
