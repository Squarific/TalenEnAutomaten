#include "Automaton.h"

bool vectorContains(std::vector<std::string> &vec, std::string str) {
	for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); ++it) {
		if (*it == str)
			return true;
	}
	
	return false;
}

Automaton productAutomaton(Automaton& automaton_1, Automaton& automaton_2) {
	Automaton product_automaton;
	std::vector<std::string> transition_names;
	int t_amount = 0;
	
	product_automaton.setName(automaton_1.name + "_" + automaton_2.name);
	
	// Add all states
	for (int i = 0; i < automaton_1.state_amount; i++) {
		for (int j = 0; j < automaton_2.state_amount; j++) {
			product_automaton.addState(State(automaton_1.states[i].name + automaton_2.states[j].name));
		}
	}
	
	// Get transition names
	for (int i = 0; i < automaton_1.transition_amount; i++) {
		if (!vectorContains(transition_names, automaton_1.transitions[i].name)) {
			transition_names.push_back(automaton_1.transitions[i].name);
			t_amount++;
		}
	}

	// Add transitions
	for (int i = 0; i < automaton_1.state_amount; i++) {
		for (int j = 0; j < t_amount; j++) {
			if (automaton_1.transitionExists(transition_names[j], automaton_1.states[i].name)) {
				for (int k = 0; k < automaton_2.state_amount; k++) {
					if (automaton_2.transitionExists(transition_names[j], automaton_2.states[k].name)) {
						std::string from = automaton_1.states[i].name + automaton_2.states[k].name;
						std::string to = automaton_1.stateTransition(automaton_1.states[i].name, transition_names[j]) + automaton_2.stateTransition(automaton_2.states[k].name, transition_names[j]);
						product_automaton.addTransition(from, to, transition_names[j]);
					}
				}
			}
		}
	}
	
	// Add start state
	product_automaton.setStart(automaton_1.getStart()->name + automaton_2.getStart()->name);
	
	return product_automaton;
}