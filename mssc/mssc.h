#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <stdlib.h>
#include <algorithm> 
#include <stack>
#include <deque>
#include <vector>
#include "../REtoNFA/classes.h"
#include "dfArrow.h"
#include "dfState.h"

using namespace std;

typedef deque<state> states;
typedef deque<dfState> dfStates;

void msscWriteToDotFile(dfStates &outputstates, vector<char>& alfabet);

bool stateexixsts(states &inputstates, int stateid);

state* findstate(states &inputstates, int stateid);

void readdotfile(string path, states &inputstates, vector<char> &alfabet);

void printstates(states& inputstates);

void printalfabet(vector<char> &alfabet);

void eclose(states& inputstates, int id, vector<int> &ecloseids);

void removedoubles(vector<int> &ints);

void alleclose(states& inputstates, vector<vector<int> > &allecloseids);

void printalleclose(vector<vector<int> > &allecloseids);

bool intVectorEqTest(vector<int> &ints, vector<int> &ints2);

void mssc(states& inputstates, dfStates& outputstates, vector<char>& alfabet);

void ExecuteMssc(string inputpath);