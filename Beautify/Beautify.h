#ifndef BEAUTIFY_H
#define BEAUTIFY_H

#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "../Product/PA.h"

void addNewName(std::vector<std::string> &vec);

void readStates(const char* infile, std::vector<std::string> &stateNames, std::vector<std::string> &newNames);

void writeStates(const char* infile, const char* outfile, std::vector<std::string> &stateNames, std::vector<std::string> &newNames);

void beautify(const char* infile, const char* outfile);

#endif