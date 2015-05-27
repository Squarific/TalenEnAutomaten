#include <vector>
#include "Dfa.h"
#include "State.h"

using namespace std;
		
int Dfa::get_size() {
	return states.size();
	
}


vector <int> Dfa::find_acceptstates() {
	vector<int> acceptstates;
	for (int place = 0; place < states.size(); place++){
		if (states[place].is_acceptstate() == true) {
			acceptstates.push_back(place);
			
		}
	}
	return acceptstates;
}

vector <vector <int> > Dfa::get_pointers (vector <int> input) {
	vector <vector <int> > places;
	for (int pointers = 0; pointers < states[input[0]].get_pointers().size(); pointers++){
		vector <int> place;
		for (int state = 0; state < states.size(); state++) {
			if (states[input[0]].get_pointers()[pointers].get_nextState()->get_name() == "ERROR") {
				place.push_back(state);
			}
			else if (states[input[0]].get_pointers()[pointers].get_nextState()->get_name() == states[state].get_name()) {
				place.push_back(state);
				
			}
			if (states[input[1]].get_pointers()[pointers].get_nextState()->get_name() == "ERROR") {
				place.push_back(state);
			}
			else if (states[input[1]].get_pointers()[pointers].get_nextState()->get_name() == states[state].get_name()) {
				place.push_back(state);
				
			}	
		}
		places.push_back(place);
	}
	return places;
}

vector<State> Dfa::get_states(vector<vector<int> > state_numbers) {
	vector<State> new_states;
	for (int state_number = 0; state_number < state_numbers.size(); state_number++) {
		string state_name = "";
		for (int state_size = 0; state_size < state_numbers[state_number].size(); state_size++) {
			state_name = state_name + states[state_numbers[state_number][state_size]].get_name();
		}
		new_states.push_back(State(state_name));
		
	}
	for (int amount = 0; amount < new_states.size(); amount++) {
		for (int int_pointers = 0; int_pointers < states[state_numbers[amount][0]].get_pointers().size(); int_pointers++) {
			for (int state = 0; state < states.size(); state++) {
				if (states[state_numbers[amount][0]].get_pointers()[int_pointers].get_nextState()->get_name() == states[state].get_name()) {
					for (int state_number = 0; state_number < state_numbers.size(); state_number++) {
						for (int state_size = 0; state_size < state_numbers[state_number].size(); state_size++) {
							if (state_numbers[state_number][state_size] == state) {
								new_states[amount].add_pointer(states[state_numbers[amount][0]].get_pointers()[int_pointers].get_input(), &(new_states[state_number]));
								
							}
						}
					} 
				}
				
			}
			
		}
		
	}
	for (int old_states = 0; old_states < states.size(); old_states++) {
		if (states[old_states].is_startstate() == true) {
			for (int state_number = 0; state_number < state_numbers.size(); state_number++) {
				for (int state_size = 0; state_size < state_numbers[state_number].size(); state_size++) {
					if (old_states == state_numbers[state_number][state_size]) {
						new_states[state_number].set_startstate(true);
						
					}
				}
			}
		}
		if (states[old_states].is_acceptstate() == true) {
			for (int state_number = 0; state_number < state_numbers.size(); state_number++) {
				for (int state_size = 0; state_size < state_numbers[state_number].size(); state_size++) {
					if (old_states == state_numbers[state_number][state_size]) {
						new_states[state_number].set_acceptstate(true);
						
					}
				}
			}
		}
		
	}
	
	return new_states;
	
}