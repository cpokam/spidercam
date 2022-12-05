#ifndef SIMULATION_H
#define SIMULATION_H

#include "isimulation.h"
#include <string>
#include <vector>
#include <array>

using std::string;
using std::array;
using std::vector;

class Simulation : public ISimulation
{

public:
    explicit Simulation(const string& fileName);
    ~Simulation(){};
    void simulate() override;
    void executeCommand(int i) override;
};

#endif // SIMULATION_H
