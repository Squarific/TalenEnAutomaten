#ifndef STATE_H
#define STATE_H

#include <vector>
#include <string>
#include "Pointer.h"

using namespace std;

class State {
	private:
		string name; // variables.
		std::vector <Pointer> pointers;
		bool startstate, acceptstate;
		
	public:
	
		State(string name) { // initiate the state.
			this->name = name;
			startstate = false;
			acceptstate = false;
		}
		
		string get_name();
		
		vector <Pointer> get_pointers();
		
		bool is_startstate();
		
		bool is_acceptstate();

		void set_startstate(bool start);
		
		void set_acceptstate(bool accept);
 
		void add_pointer(string input, State *nextState);
};

#endif