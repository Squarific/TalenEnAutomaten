#include <fstream>
#include <string>
#include <iostream>
#include "Automaton.h"

void Automaton::parseAutomaton(const char* filename) {
	std::string name, stateFrom, stateTo;
	bool name_set = false;
	bool start_set = false;

	int i = 0;
	
	std::ifstream input;
	input.open(filename);
	
	for(std::string line; getline(input, line); ) {
		// Name should be on the first line
		if (i == 0) {
			if (line.find("digraph ") == 0) {
				name = line;
				name.erase(0, 9); // Erase: digraph "
				name.erase(name.find("\""), name.size()); // Erase everything that comes after the second quotation mark
				this->setName(name);
				name_set = true;
			}
			else {
				std::cerr << "PARSING ERROR: graph must be digraph." << std::endl;
			}
		}
		// Transitions
		else if (line.find("->") != -1) {
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

				// Add states if they don't exist yet
				if (! this->stateExists(stateFrom)) {
					this->addState(State(stateFrom));

					// If this is the first state to be added, make it the start state.
					if (state_amount == 1) {
						start_set = true;
						this->setStart(stateFrom);
					}

				}
				if (! this->stateExists(stateTo)) {
					this->addState(State(stateTo));
				}

				// Add the transition
				this->addTransition(stateFrom, stateTo, name);
			}
			else {
				std::cerr << "PARSING ERROR: Line " << i + 1 << " has incorrect syntax: " << std::endl << line << std::endl
				<< "No label found." << std::endl << std::endl;				
			}
		}
		else if (line.find("}") != -1) {
			break;
		}
		else {
			std::cerr << "PARSING ERROR: Line " << i + 1 << " has incorrect syntax: " << std::endl << line << std::endl
			<< "No '->' found." << std::endl << std::endl;
		}

		i++;
	}
	
	if ((!name_set) || (!start_set))
		std::cerr << "PARSING ERROR: Name/Start not set." << std::endl << std::endl;

	input.close();
}