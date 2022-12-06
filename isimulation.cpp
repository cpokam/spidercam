#include "isimulation.h"
// ios::exceptions
#include <stdexcept> // std::out_of_range
#include <iostream>
#include <fstream>
#include <cmath>

/**
 * @brief Simulation::Simulation constructs the simulation class
 * @param fileName : the input file for the simulation
 */
ISimulation::ISimulation(const string &fileName_) :
    fileName(fileName_),
    iodata(IOData(fileName_)),
    currentStartTime(0),
    nextStartTime(0),
    currentExecutionTime(0)
{
    init();
}

/**
 * @brief Simulation::calculateStagesVector for the 3 stages Ta, Tb and Tc.
 * @param i : command level
 */
void ISimulation::calculateStagesVector(int i)
{
    t_a = iodata.getVmax() / iodata.getAmax();
    st_a = (iodata.getAmax() / 2) * pow(t_a, 2);
    t_b = t_a + ((calculateCurrentDelta(i) - 2 * st_a) / iodata.getVmax());
    t_c = t_a + t_b;
    stagesVector = {t_a, t_b, t_c};
}

/**
 * @brief Simulation::calculateS_t calculates the distance traveled
 * @param i: command level
 * @param t: time
 * @return the calculated distance
 */
double ISimulation::calculateS_t(int i, double t)
{
    calculateStagesVector(i);
    double result{};
    if ((0 <= t) && (t <= t_a))
        result = (iodata.getAmax() / 2) * pow(t, 2);
    else if ((t_a <= t) && (t <= t_b))
        result = st_a + iodata.getVmax() * (t - t_a);
    else if ((t_b <= t) && (t <= t_c))
        result = calculateCurrentDelta(i) - ((iodata.getAmax() / 2) * (pow((t - t_c), 2)));

    return result;
}

/**
 * @brief Simulation::lambda Variable for the camera's line mouvement.
 * @param i:command level
 * @param t: time
 * @return the calculated lambda variable
 */
double ISimulation::lambda(int i, double t)
{
    return calculateS_t(i, t) / calculateCurrentDelta(i);
}

/**
 * @brief Simulation::calculateTimeIntervals the discrete
 *        times intervals for the camera
 * @param i : command level
 */
void ISimulation::calculateTimeIntervals(int i)
{
    calculateStagesVector(i);
    timeIntervals.clear();
    for (int j = 0; j <= std::floor(t_c * iodata.getFreq()); j++)
        timeIntervals.push_back((float)j / iodata.getFreq());
}

/**
 * @brief Simulation::currentVelocity calculates the actual speed of the camera
 * @param i : current command
 * @param t : current time
 * @return  the speed of the camera.
 */
double ISimulation::currentVelocity(int i, double t)
{
    init(i);
    double result;
    if (0 <= t && t <= t_a)
        result = iodata.getAmax() * t;
    else if (t_a < t && t < t_b)
        result = iodata.getVmax();
    else
        result = (iodata.getVmax() - iodata.getAmax()) * (t - t_a);

    return result;
}

/**
 * @brief Simulation::currentLine_G : The line function of the camera's mouvement.
 * @param i : the command level
 */
void ISimulation::currentLine_G(int i)
{
    init(i);
    vector<double> element;
    currentCameraPositions.clear();

    for (size_t k = 0; k < currentStartPoint.size(); k++)
    {
        for (size_t j = 0; j < timeIntervals.size(); j++)
        {
            element.push_back(currentStartPoint[k] + lambda(i, timeIntervals.at(j)) * (currentEndPoint[k] - currentStartPoint[k]));
        }
        currentCameraPositions.push_back(element);
        element.clear();
    }
}

/**
 * @brief Simulation::calculateCurrentCameraPosition :
 *          The camera's positions at a given command level
 * @param i : the command level
 */
void ISimulation::calculateCurrentCameraPositions(int i)
{
    currentLine_G(i);
}

/**
 * @brief calculate the current camera position
 *
 * @param i : command level
 * @param t : time
 */
void ISimulation::calculateCurrentCameraPositions(int i, double t) {
    init(i);
    for (size_t k = 0; k < currentStartPoint.size(); k++)
        currentPosition.at(k)=(currentStartPoint[k] + lambda(i, t) * (currentEndPoint[k] - currentStartPoint[k]));
}

/**
 * @brief Simulation::calculateLengthSteelCables :
 *          The length of the steel cables for the spidercam
 * @param i : the command level
 */
void ISimulation::calculateLengthSteelCables(int i)
{
    calculateCurrentCameraPositions(i);
    vector<double> element;
    double result;
    lengthSteelCables.clear();

    for (size_t j = 0; j < timeIntervals.size(); j++)
    {
        result = sqrt(std::pow(currentCameraPositions[0][j] - anchoragePoint_R1[0], 2) +
                      std::pow(currentCameraPositions[1][j] - anchoragePoint_R1[1], 2) +
                      std::pow(currentCameraPositions[2][j] - anchoragePoint_R1[2], 2));
        element.push_back(result);
    }
    lengthSteelCables.push_back(element);
    element.clear();

    for (size_t j = 0; j < timeIntervals.size(); j++)
    {
        result = sqrt(std::pow(currentCameraPositions[0][j] - anchoragePoint_R2[0], 2) +
                      std::pow(currentCameraPositions[1][j] - anchoragePoint_R2[1], 2) +
                      std::pow(currentCameraPositions[2][j] - anchoragePoint_R2[2], 2));
        element.push_back(result);
    }
    lengthSteelCables.push_back(element);
    element.clear();

    for (size_t j = 0; j < timeIntervals.size(); j++)
    {
        result = sqrt(std::pow(currentCameraPositions[0][j] - anchoragePoint_R3[0], 2) +
                      std::pow(currentCameraPositions[1][j] - anchoragePoint_R3[1], 2) +
                      std::pow(currentCameraPositions[2][j] - anchoragePoint_R3[2], 2));
        element.push_back(result);
    }
    lengthSteelCables.push_back(element);
    element.clear();

    for (size_t j = 0; j < timeIntervals.size(); j++)
    {
        result = sqrt(std::pow(currentCameraPositions[0][j] - anchoragePoint_R4[0], 2) +
                      std::pow(currentCameraPositions[1][j] - anchoragePoint_R4[1], 2) +
                      std::pow(currentCameraPositions[2][j] - anchoragePoint_R4[2], 2));
        element.push_back(result);
    }
    lengthSteelCables.push_back(element);
    element.clear();
}

/**
 * @brief Simulation::calculateCurrentDelta : The distance traveled at a given command level
 * @param i : command level
 * @return the calculated distance
 */
double ISimulation::calculateCurrentDelta(int i)
{
    double result{};
    init(i);
    result = sqrt(std::pow(currentEndPoint[0] - currentStartPoint[0], 2) +
                  std::pow(currentEndPoint[1] - currentStartPoint[1], 2) +
                  std::pow(currentEndPoint[2] - currentStartPoint[2], 2));
    return result;
}

/**
 * @brief Simulation::init initializes the coordinates
 *        of the camera at a given command level
 * @param i : the command level
 */
void ISimulation::init(int i)
{
    currentStartPoint = {iodata.data[i][1], iodata.data[i][2], iodata.data[i][3]};
    if ((size_t)i < iodata.data.size() - 1)
        currentEndPoint = {iodata.data[i + 1][1], iodata.data[i + 1][2], iodata.data[i + 1][3]};
}
/**
 * @brief Simulation::init Initialization of some constants
 */
void ISimulation::init()
{
    t_a = iodata.getVmax() / iodata.getAmax();
    t_b = t_c = st_a = 0;
    array<double, 4> startCommand = {0, iodata.getStart()[0], iodata.getStart()[1], iodata.getStart()[2]};
    iodata.data.insert(iodata.data.begin(), startCommand);
    anchoragePoint_R1 = {0, 0, iodata.getDim().at(2)};
    anchoragePoint_R2 = {iodata.getDim().at(0), 0, iodata.getDim().at(2)};
    anchoragePoint_R3 = {0, iodata.getDim().at(1), iodata.getDim().at(2)};
    anchoragePoint_R4 = {iodata.getDim().at(0), iodata.getDim().at(1), iodata.getDim().at(2)};
}

/**
 * @brief Simulation::pointToString prints a point
 * @param point : Point to be printed
 */
void ISimulation::pointToString(const array<double, 3> &point)
{
    std::cout << "[" << point[0] << ","
              << point[1] << ","
              << point[2] << "]" << std::endl;
}

/**
 * @brief Simulation::commandToString prints a command to the screen
 * @param point : the command to be printed
 */
void ISimulation::commandToString(const std::array<double, 4> &command)
{
    std::cout << command[0] << "s"
              << " at "
              << "[" << command[1] << ","
              << command[2] << ","
              << command[3] << "]" << std::endl;
}
