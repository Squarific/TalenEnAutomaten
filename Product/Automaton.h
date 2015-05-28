#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class PState {
  private:
	
  public:
  	std::string name;
  	bool isEnd;

	PState(){}
	PState(std::string n) {
		name = n;
		isEnd = false;
	}

	void setEnd(bool e);
};

class Transition {
  private:
	
  public:
  	std::string name;
	std::string from;
	std::string destination;
	
    Transition(){}
	Transition(std::string f, std::string d, std::string n) {
		name = n;
		destination = d;
		from = f;
	}
};

class Automaton {
  private:
	std::string start; 
	
  public:
	std::string name;
	
	std::vector<PState> states;
	int state_amount;
	
	std::vector<Transition> transitions;
	int transition_amount;

	Automaton() {
		state_amount = 0;
		transition_amount = 0;
	}
	
	void setName(std::string n);
	
	void addState(PState s);
	PState* getState(std::string n);
	bool stateExists(std::string n);
	
	void addTransition(std::string from, std::string to, std::string n);
	Transition* getTransition(std::string n, std::string s);
	bool transitionExists(std::string n, std::string s);
	
	// When currently in state "from", using transition with name "n", returns pointer to destination state.
	std::string stateTransition(std::string from, std::string t);
	
	// Sets state with name "n" to start state.
	void setStart(std::string n);
	PState* getStart();
	
	// Parsing
	void parseAutomaton(const char* filename);

	// File output (dot language)
	void writeToFile(const char* filename);
};

#endif