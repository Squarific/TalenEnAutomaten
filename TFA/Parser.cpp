#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "State.h"

using namespace std;

vector<State> parseAutomaton(const char* filename, State Er_state) {
	std::string name, stateFrom, stateTo, stateName, line;

	std::ifstream input;
	input.open(filename);
	
	vector<pair<pair<State, string>, State > > pointers;
	vector<State> states; 
	
	int linenumber = 0;

	while (std::getline(input, line)) {
		if (line.find("->") != -1) {
			if (line.find("[label=\"") != -1) {
				// stateFrom
				stateFrom = line;
				stateFrom.erase(0, stateFrom.find("\"") + 1);
				stateFrom.erase(stateFrom.find("\""), stateFrom.size());

				// stateTo
				stateTo = line;
				for (int j = 0; j < 3; j++) {
					stateTo.erase(0, stateTo.find("\"") + 1);
				}
				stateTo.erase(stateTo.find("\""), stateTo.size());
				
				// Label
				name = line;
				for (int j = 0; j < 5; j++) {
					name.erase(0, name.find("\"") + 1);
				}
				name.erase(name.find("\""), name.size());

				pointers.push_back(make_pair(make_pair(State(stateTo), name), State(stateFrom)));
			}
			
			else {
				std::cerr << "PARSING ERROR: Line " << linenumber + 1 << " has incorrect syntax: " << std::endl << line << std::endl
				<< "No label found." << std::endl << std::endl;				
			}
		}
		else if (line.find("[shape=\"doublecircle\"];") != -1) {
			stateName = line;
			stateName.erase(0, stateName.find("\"") + 1);
			stateName.erase(stateName.find("\""), stateName.size());
			for (int state = 0; state < pointers.size(); state++) {
				if (pointers[state].second.get_name() == stateName) {
					pointers[state].second.set_acceptstate(true);
				}
			}
		}
		else if (line.find("}") != -1) {
			break;
		}
		else if (line.find("digraph") != -1) {
			continue;
		}
		else {
			std::cerr << "PARSING ERROR: Line " << linenumber + 1 << " has incorrect syntax: " << std::endl << line << std::endl
			<< std::endl << std::endl;
		}
		linenumber++;
		
	}
	
	vector<string> inputs;
	for (vector<pair<pair<State, string>, State > >::iterator pointer = pointers.begin(); pointer != pointers.end(); pointer++ ) {
		inputs.push_back(pointer->first.second);
	}
	
	std::sort(inputs.begin(), inputs.end());
    inputs.erase(std::unique(inputs.begin(), inputs.end()), inputs.end());
	
	vector<string> names;
	for (vector<pair<pair<State, string>, State > >::iterator pointer = pointers.begin(); pointer != pointers.end(); pointer++ ) {
		names.push_back(pointer->second.get_name());
	}
	
	std::sort(names.begin(), names.end());
    names.erase(std::unique(names.begin(), names.end()), names.end());
	
	vector<pair<pair<State, string>, State > > pointers2;
	
	for (vector<string>::iterator name = names.begin(); name != names.end(); name++) { 
		for (vector<string>::iterator input = inputs.begin(); input != inputs.end(); input++) { 
			int test = 0;
			for (vector<pair<pair<State, string>, State > >::iterator pointer = pointers.begin(); pointer != pointers.end(); pointer++) {
				if (*name == pointer->second.get_name()) {
					if (*input == pointer->first.second) {
						test++;
					}
				}
			}
			if (test == 0) {
				for (vector<pair<pair<State, string>, State > >::iterator pointer = pointers.begin(); pointer != pointers.end(); pointer++) {
					if (*name == pointer->second.get_name()){
						pointers2.push_back(make_pair(make_pair(Er_state, *input), pointer->second));
						break;
					}
				}
			}
		}
	}
	
	for (vector<pair<pair<State, string>, State > >::iterator pointer = pointers2.begin(); pointer != pointers2.end(); pointer++) {
		pointers.push_back(*pointer);
	}
	
	
	
	for (vector<pair<pair<State, string>, State > >::iterator pointer = pointers.begin(); pointer != pointers.end(); pointer++ ) {
		int tester = 0;
		for (vector<State>::iterator state = states.begin(); state != states.end(); state++) {
			if(pointer->second.get_name() == state->get_name()){
				tester++;
			}
		}
		if (tester == 0) {
			states.push_back(pointer->second);
			
		}
	}
	
	for (vector<pair<pair<State, string>, State > >::iterator pointer = pointers.begin(); pointer != pointers.end(); pointer++ ) {
		for (vector<State>::iterator state = states.begin(); state != states.end(); state++) {
			if (pointer->second.get_name() == state->get_name()) {
				for (vector<State>::iterator state2 = states.begin(); state2 != states.end(); state2++) {
					if (pointer->first.first.get_name() == "ERROR") {
						(*state).add_pointer(pointer->first.second, &Er_state);
						break;
					}
					else if (state2->get_name() == pointer->first.first.get_name()) {
						(*state).add_pointer(pointer->first.second, &(*state2));
						break;
					}
				}
			}
		}
	}
	return states;
}