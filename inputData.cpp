#include "inputData.h"
using std::ifstream;
#include <iostream>
#include <fstream>
#include <sstream>      // std::stringstream
#include <stdexcept>    // std::out_of_range
//ios::exceptions

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;
using std::stringstream;

/**
 * @brief Construct a new Input Data:: Input Data object
 * 
 * @param fileName : the file to be read
 */
string IOData::getfileName() const
{
    return fileName;
}

int IOData::getVmax() const
{
    return vmax;
}

int IOData::getAmax() const
{
    return amax;
}

int IOData::getFreq() const
{
    return freq;
}

vector<array<double, 4> > IOData::getData() const
{
    return data;
}

array<double, 3> IOData::getDim() const
{
    return dim;
}

array<double, 3> IOData::getStart() const
{
    return start;
}

IOData::IOData(const string &fileName):
    fileName(fileName)
{
    einlesen(fileName);
}


/**
 * @brief initialize the data structure to store the content of the file
 * 
 * @param dataVector 
 */
void IOData::init(const vector<string> &dataVector){

    try {

        for(size_t i =1; i<dataVector.size(); i++){
            if (dataVector.at(0)=="dim")
                dim[i-1]=std::stoi(dataVector.at(i));
            else if (dataVector.at(0)=="start")
                start[i-1]=std::stoi(dataVector.at(i));
            else if (dataVector.at(0)=="vmax")
                vmax=std::stoi(dataVector.at(i));
            else if (dataVector.at(0)=="amax")
                amax=std::stoi(dataVector.at(i));
            else if (dataVector.at(0)=="freq")
                freq=std::stoi(dataVector.at(i));
        }
        if(isdigit(dataVector.at(0)[0])){
            array<double,4>lineData;
            for(size_t i =0; i<dataVector.size(); i++){
                lineData[i]=std::stod(dataVector.at(i));
            }
            data.push_back(lineData);
        }
    }
    catch (const std::out_of_range& oor) {
        std::cerr << "Out of Range error: " << oor.what() << '\n';
    }
}

/**
 * @brief Simulation::output generates the 2 output files of the simulation
 * @param datavector :
 */
void IOData::output(const vector<vector<double>> &dataVector)
{
    string file_out1{fileName+"_1.out"};
    std::ofstream ofs_file_out1(file_out1);

    ofs_file_out1.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    try
    {
        if (ofs_file_out1)
        {
            for (const vector<double> &element : dataVector)
            {
                std::string delim = "";
                for (auto temp : element)
                {
                    ofs_file_out1 <<std::showpoint<<delim << temp;
                    delim = ", ";
                }
                ofs_file_out1 << std::endl;
            }
            ofs_file_out1.close();
        }
    }
    catch (std::ofstream::failure &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * @brief Simulation::output generates the 2 output files of the simulation
 * @param : vector_t
 * @param : dataVector
 */
void IOData::output(const vector<double> &vector_t,
                    const vector<vector<double>> &dataVector)
{
    string file_out2{fileName+"_2.out"};
    std::ofstream ofs_file_out2(file_out2);

    ofs_file_out2.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    try
    {
        if (ofs_file_out2)
        {
            std::string delim = "";
            for (double element : dim)
            {
                ofs_file_out2 <<std::showpoint<<delim << element;
                delim = ", ";
            }
            ofs_file_out2 << std::endl;

            delim = "";
            for (double element : vector_t)
            {
                ofs_file_out2 <<std::showpoint<<delim << element;
                delim = ", ";
            }
            ofs_file_out2 << std::endl;

            for (const vector<double> &element : dataVector)
            {
                delim = "";
                for (double temp : element)
                {
                    ofs_file_out2 <<std::showpoint<<delim << temp;
                    delim = ", ";
                }
                ofs_file_out2 << std::endl;
            }
            ofs_file_out2.close();
        }
    }
    catch (std::ofstream::failure &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * @brief this function processes a given
 * string by removing the comment character from it.
 *
 * @param s: string to be processed.
 * @return the string left to s
 */
std::string IOData::process(std::string const &s)
{
    std::string::size_type pos = s.find('#');

    if (pos != std::string::npos)
    {
       return s.substr(0, pos);
    }
    else
       return s;
}

/**
 * @brief Read a file and extract its content
 * 
 * @param fileName : the file to be read
 */
void IOData::einlesen(const string &fileName)
{
    ifstream file_in(fileName);
    file_in.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
    try {
        file_in.is_open();
        
        string line;
        while (getline(file_in, line))
        {
            if (!process(line).empty() && line.find('\n')==std::string::npos){
                vector<string> dataVector;
                string word;
                stringstream ss(process(line));  // Takes only space separated C++ strings
                while(ss >> word) {     //extract word from the stream
                    dataVector.push_back(word);
                }
                init(dataVector);
            }
        }
        file_in.close();
    } catch (std::ifstream::failure &e) {
        std::cerr << e.what()<<std::endl;
    }
}

/**
 * @brief IOData:split a string
 * @param s: string to be splitted
 * @param delimiter
 * @return
 */
vector<string> IOData::split(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}
