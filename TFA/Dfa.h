#ifndef DFA_H
#define DFA_H

#include "State.h"
#include <vector>

using namespace std;

class Dfa {
	private:
		std::vector <State> states;
		
	public:
		
		Dfa(vector<State> states) {
			this->states = states;
			
		}
		
		int get_size();
		
		vector <int> find_acceptstates();
		
		vector <vector <int> > get_pointers (vector <int> input);
		
		vector<State> get_states(vector<vector<int> > state_numbers);
};

#endif