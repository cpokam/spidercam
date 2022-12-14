#ifndef INPUTDATA_H
#define INPUTDATA_H

#include <string>
#include <vector>
#include <array>

using std::string;
using std::array;
using std::vector;

/**
* @brief The InputData class
*
* This class manages the input and output for the simulation of Spidercam.
* @author Christian Pokam
*
*/
class IOData {

string fileName;
array<double,3>dim{};
array<double,3>start{};
int vmax{}, amax{}, freq{};
vector<array<double,4>>instructions;

public:
explicit IOData(const string &fileName);

string getFileName()const;
int getVmax()const;
int getAmax()const;
int getFreq()const;

double getInstruction(int i, int j)const;
double& setInstruction(int i, int j);
array<double,4>getInstruction(int i)const;
array<double,4>& setInstruction(int i);
void insertInstruction(int i, array<double, 4>instruction);
int getInstructionsSize()const;

array<double,3>getDim()const;
array<double,3>getStart()const;

vector<string> split(const string& s, char delimiter);
void einlesen(const string &fileName);
std::string process(std::string const& s);
void init(const vector<string> &dataVector);
void output(const vector<vector<double>> &dataVector);
void output(const vector<double> &vector_t,
            const vector<vector<double>> &dataVector);
};

#endif // INPUTDATA
