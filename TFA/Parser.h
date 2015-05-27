#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "State.h"

using namespace std;

vector<State> parseAutomaton(const char* filename, State Er_state);

#endif