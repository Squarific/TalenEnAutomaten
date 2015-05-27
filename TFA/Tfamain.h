#ifndef TFAMAIN_H
#define TFAMAIN_H

#include <ostream>
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>
#include "Pointer.h"
#include "State.h"
#include "Dfa.h"
#include "Board.h"
#include "Parser.h"

using namespace std;

vector<State> find_reachable(vector<State> reachable, State testState);

vector<State> TFA(Board board, vector<int> states, Dfa dfa);

void writeOutput(const char* filename, vector<State> states);

vector<State> set_pointers(vector<pair<pair<State, string>, State > > pointers);

void executeTFA (const char* inputfile, const char* outputfile);

#endif