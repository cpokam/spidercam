#include "simulation.h"
// ios::exceptions
#include <stdexcept> //std::out_of_range
#include <iostream>
#include <fstream>
#include <cmath>


/**
 * @brief Simulation::Simulation constructs the simulation class
 * @param fileName : the input file for the simulation
 */
Simulation::Simulation(const string &fileName_) : ISimulation(fileName_)
{

}

/**
 * @brief Simulation::simulate starts the simulation's process
 */
void Simulation::simulate()
{
    static int i = 0;
    executeCommand(i);
}

/**
 * @brief Simulation::executeCommand : recursive method for the execution logic
 *                                     of the simulation
 * @param i : command level
 */
void Simulation::executeCommand(int i)
{
    if ((size_t)i >= iodata.data.size()-1)
        return;
    try
    {
        init(i);
        currentStartTime = iodata.data.at(i)[0];
        calculateStagesVector(i);
        currentExecutionTime = t_c;
        nextStartTime = iodata.data.at(i + 1)[0];
        if (nextStartTime == 0 || (currentStartTime + currentExecutionTime < nextStartTime))
        {
            std::cout << "command " << i <<" succeeds" << std::endl;
            std::cout << "from: " << std::endl;
            commandToString(iodata.data.at(i));
            std::cout << "to : " << std::endl;
            commandToString(iodata.data.at(i + 1));
            std::cout << "execution time: " << t_c << " seconds" << std::endl;
            calculateStagesVector(i);
            std::cout << "StageVector: "<<std::endl;
            pointToString(stagesVector);
            std::cout<<std::endl;

            calculateCurrentDelta(i);
            calculateTimeIntervals(i);
            currentLine_G(i);
            calculateLengthSteelCables(i);

            iodata.output(lengthSteelCables);
            iodata.output(timeIntervals,currentCameraPositions);
            i++;
            executeCommand(i);
        }
        else
        {
            std::cout << "command " << i <<" failed!" << std::endl;
            std::cout << "from: " << std::endl;
            commandToString(iodata.data.at(i));
            std::cout << "to : " << std::endl;
            commandToString(iodata.data.at(i + 1));
            std::cout << "command needs " << t_c << " seconds for the execution" << std::endl;
            double cameraStopTime = currentStartTime + (nextStartTime - currentStartTime) +
                                     currentVelocity(i, nextStartTime - currentStartTime) / iodata.getAmax();
            std::cout << "camera stopped at " << cameraStopTime<<"s";
            //calculateCurrentCameraPosition(i,cameraStopTime);
            //pointToString(currentPosition);
            std::cout<<std::endl;
            calculateStagesVector(i);
            std::cout << "StageVector: "<<std::endl;
            pointToString(stagesVector);
            std::cout<<std::endl;

            if ((size_t)i + 2 < iodata.data.size() - 1 && cameraStopTime > iodata.data.at(i + 2)[0])
            {
                iodata.data.at(i + 2)[0] = cameraStopTime;
                i += 2;
                executeCommand(i);
            }
        }
    }
    catch (const std::out_of_range &oor)
    {
        std::cerr << "Out of Range error: " << oor.what() << std::endl;
    }
}
