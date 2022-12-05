#ifndef ISIMULATION_H
#define ISIMULATION_H

#include "inputData.h"
#include <string>
#include <vector>
#include <array>

using std::string;
using std::array;
using std::vector;

/**
 * @brief The ISimulation class
 */
class ISimulation
{
public:
    explicit ISimulation(const string &fileName);
    void calculateStagesVector(int i);    //ta, tb, tc
    double calculateS_t(int i, double t);
    double lambda(int i, double t);
    void calculateTimeIntervals(int i);
    double currentVelocity(int i, double t);
    void calculateCurrentCameraPositions(int i);
    void calculateCurrentCameraPositions(int i,double t);
    void calculateLengthSteelCables(int i);
    double calculateCurrentDelta(int i);
    void currentLine_G(int i);

    void init();
    void init(int i);

    void pointToString(const array<double,3>&point);
    void commandToString(const array<double,4>&command);

    void virtual simulate()=0;
    void virtual executeCommand(int i)=0;

protected:
    string fileName;
    IOData iodata;
    double t_a, t_b, t_c, st_a;
    double currentStartTime;
    double nextStartTime;
    double currentExecutionTime;

    array<double,3>currentPosition;
    array<double,3>currentStartPoint;
    array<double,3>currentEndPoint;
    array<double,3>anchoragePoint_R1;
    array<double,3>anchoragePoint_R2;
    array<double,3>anchoragePoint_R3;
    array<double,3>anchoragePoint_R4;
    array<double,3>stagesVector;    //PhasenVector<t_a, t_b, t_c>

    vector<double>timeIntervals;    //t0, ti, ..., tn
    vector<vector<double>>currentCameraPositions;
    vector<vector<double>>lengthSteelCables;
    vector<array<double,4>>currentData;
};

#endif // ISIMULATION_H


