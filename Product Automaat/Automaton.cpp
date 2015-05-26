#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Automaton.h"

void Automaton::setName(std::string n) {
	name = n;
}

void Automaton::addState(State s) {
	states.push_back(s);
	state_amount++;
}

State* Automaton::getState(std::string n) {
	for(int i = 0; i < state_amount; i++) {
		if (states[i].name == n) {
			return &states[i];
		}
	}
	
	std::cerr << "ERROR: State with name: " << n << " not found." << std::endl;
}

bool Automaton::stateExists(std::string n) {
	for(int i = 0; i < state_amount; i++) {
		if (states[i].name == n) {
			return true;
		}
	}
	
	return false;
}

Transition* Automaton::getTransition(std::string n, std::string s) {
	for(int i = 0; i < transition_amount; i++) {
		if ((transitions[i].name == n) && (transitions[i].from == s)) {
			return &transitions[i];
		}
	}
	
	std::cerr << "ERROR: Transition with name: " << n << " not found." << std::endl;
}

bool Automaton::transitionExists(std::string n, std::string s) {
	for(int i = 0; i < transition_amount; i++) {
		if ((transitions[i].name == n) && (transitions[i].from == s)) {
			return true;
		}
	}
	
	return false;
}

void Automaton::addTransition(std::string from, std::string to, std::string n) {
	transitions.push_back(Transition(from, to, n));
	transition_amount++;
}

std::string Automaton::stateTransition(std::string from, std::string n) {
	if (this->transitionExists(n, from))
		return (this->getTransition(n, from))->destination;
	else {
		std::cerr << "ERROR: Transition with name: " << n << " not found." << std::endl;
		return from;
	}
}

void Automaton::setStart(std::string n) {
	start = n;
}

State* Automaton::getStart() {
	return this->getState(start);
}

void Automaton::writeToFile(const char* filename) {
	std::ofstream output;
	output.open(filename);

	// Graph name + opening bracket
	output << "digraph \"" << name << "\" {" << std::endl;

	// Transitions
	for (int i = 0; i < transitions.size(); i++) {
		output << "    \"" << transitions[i].from << "\" -> \""
		<< transitions[i].destination << "\" [label=\"" << transitions[i].name << "\"]"
		<< std::endl;
	}

	// Closing bracket
	output << "}" << std::endl;
}