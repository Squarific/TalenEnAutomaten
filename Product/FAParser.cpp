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

		// Start state
		else if (line.find("-1->") != -1) {
			stateFrom = line;
			stateFrom.erase(0, stateFrom.find("\"") + 1);
			stateFrom.erase(stateFrom.find("\""), stateFrom.size());

			this->addState(PState(stateFrom));
			this->setStart(stateFrom);
			start_set = true;
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
					this->addState(PState(stateFrom));

				}
				if (! this->stateExists(stateTo)) {
					this->addState(PState(stateTo));
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
		else if (line.find("rankdir") != -1) {
			continue;
		}
		else if (line.find("-1[style") != -1) {
			continue;
		}
		else if (line.find("doublecircle") != -1) {
			stateFrom = line;
			stateFrom.erase(0, stateFrom.find("\"") + 1);
			stateFrom.erase(stateFrom.find("\""), stateFrom.size());
			this->getState(stateFrom)->setEnd(true);
		}
		else {
			std::cerr << "PARSING ERROR: Line " << i + 1 << " has incorrect syntax: " << std::endl << line << std::endl;
		}

		i++;
	}
	
	if ((!name_set) || (!start_set))
		std::cerr << "PARSING ERROR: Name/Start not set." << std::endl << std::endl;

	input.close();
}