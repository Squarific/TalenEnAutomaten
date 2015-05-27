#include "Pointer.h"
#include <string>

using namespace std;
		
State* Pointer::get_nextState() {
	return nextState;
	
}

string Pointer::get_input() {
	return input;
	
}