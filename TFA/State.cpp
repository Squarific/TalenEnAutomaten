#include <vector>
#include <string>
#include "State.h"
#include "Pointer.h"

using namespace std;
	
string State::get_name() {
	return name;
	
}

vector <Pointer> State::get_pointers() {
	return pointers;
	
}

bool State::is_startstate() {
	return startstate;
	
}

bool State::is_acceptstate() {
	return acceptstate;
	
}

void State::set_startstate(bool start) {
	startstate = start;
	
}

void State::set_acceptstate(bool accept) {
	acceptstate = accept;
	
}

void State::add_pointer(string input, State *nextState) { // a function that allows to set the pointers.
	pointers.push_back(Pointer(input, nextState));
}