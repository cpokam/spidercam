#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "simulation.h"

class Simulation;

int main(int argc, char *argv[]) 
{
    if (argc<2) {
        std::cout<<" Wrong argument, call the program with "<<argv[0]<<" fileName"<<std::endl;
    }
    else {
        Simulation sim = Simulation(std::string(argv[1]));
        sim.simulate();
    }
    return 0;
}
