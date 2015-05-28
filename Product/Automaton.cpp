#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Automaton.h"

void PState::setEnd(bool b) {
	isEnd = b;
}

void Automaton::setName(std::string n) {
	name = n;
}

void Automaton::addState(PState s) {
	states.push_back(s);
	state_amount++;
}

PState* Automaton::getState(std::string n) {
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

PState* Automaton::getStart() {
	return this->getState(start);
}

void Automaton::writeToFile(const char* filename) {
	std::ofstream output;
	output.open(filename);

	// Graph name + opening bracket
	output << "digraph \"" << name << "\" {" << std::endl;

	// rankdir
	output << "\trankdir=LR" << std::endl;

	// Start state
	output << "\t-1[style=\"invis\"];" << std::endl;
	output << "\t-1->\"" << this->getStart()->name << "\";" << std::endl;

	// Transitions
	for (int i = 0; i < transitions.size(); i++) {
		output << "\t\"" << transitions[i].from << "\" -> \""
		<< transitions[i].destination << "\" [label=\"" << transitions[i].name << "\"]"
		<< std::endl;
	}

	// End states
	for (int i = 0; i < states.size(); i++) {
		if (states[i].isEnd) {
			output << "\t\"" << states[i].name << "\"[shape=\"doublecircle\"];" << std::endl;
		}
	}

	// Closing bracket
	output << "}" << std::endl;

	std::cout << "PRODUCT: File saved." << std::endl;
}