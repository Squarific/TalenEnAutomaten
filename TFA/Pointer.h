#ifndef POINTER_H
#define POINTER_H

#include <string>

using namespace std;

class State;

class Pointer {
	private:
		string input; // variables.
		State *nextState;
		
	public:
		Pointer(string input, State *nextState) { // initiate the state.
			this->input = input;
			this->nextState = nextState;
		}
		
		State* get_nextState();
		
		string get_input();
};

#endif