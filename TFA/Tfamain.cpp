#include <ostream>
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>
#include "Pointer.h"
#include "State.h"
#include "Dfa.h"
#include "Board.h"
#include "Parser.h"
#include "Tfamain.h"

using namespace std;

vector<State> find_reachable(vector<State> reachable, State testState) {
	int test = 0;
	for (vector<State>::iterator state = reachable.begin(); state != reachable.end(); state++) {
		if (testState.get_name() == state->get_name()) {
			test++;
			
		}
		
	}
	if (test == 0) {
		reachable.push_back(testState);
		
	}
	for (int i = 0; i < testState.get_pointers().size(); i++) {
		int test = 0;
		if (testState.get_pointers()[i].get_nextState()->get_name() == "ERROR") {
			test++;
			
		}
		for (vector<State>::iterator state = reachable.begin(); state != reachable.end(); state++) {
			if (testState.get_pointers()[i].get_nextState()->get_name() == state->get_name()) {
				test++;
			}
		}
		if (test == 0) {
			vector<State> temp = find_reachable(reachable, *testState.get_pointers()[i].get_nextState());
			reachable = temp;
		}
		
	}
	return reachable;
}

vector<State> TFA(Board board, vector<int> states, Dfa dfa) {
	board.first_step(states);
	while (true) {
		Board test_board = board;
		board.second_step(dfa);
		if (board.check_board(test_board) == true) {
			break;
		}
	}
	return dfa.get_states(board.third_step(dfa));
}

void writeOutput(const char* filename, vector<State> states) {
	ofstream outputFile;
	outputFile.open(filename);
	
	outputFile << "digraph \"new_DFA\" {" << endl;
	outputFile << "\trankdir=LR" << endl;
	outputFile << "\t-1[style=\"invis\"];" << endl;
	
	for (int state = 0; state < states.size(); state++) {
		if (states[state].is_startstate() == true) {
			outputFile << "\t-1->\"" << states[state].get_name() << "\";" << endl;
		}
	}
	
	for (int state = 0; state < states.size(); state++) {
		for (int pointer = 0; pointer < states[state].get_pointers().size(); pointer++) {
			outputFile << "\t\"" <<  states[state].get_name() << "\" -> \"" << states[state].get_pointers()[pointer].get_nextState()->get_name() << "\" [label=\"" << states[state].get_pointers()[pointer].get_input() << "\"]" << endl;
			
		}
	}
	for (int state = 0; state < states.size(); state++) {
		if (states[state].is_acceptstate() == true) {
			outputFile << "\t\"" <<  states[state].get_name() << "\"[shape=\"doublecircle\"];" << endl;
			
		}
	}
	outputFile << "}";

	cout << "TFA: File saved." << std::endl;
	
	outputFile.close();	
}

vector<State> set_pointers(vector<pair<pair<State, string>, State > > pointers) {
	vector<State> states;
	for (int pointer = 0; pointer < pointers.size(); pointer++) {
		int tester = 0;
		for (int state = 0; state < states.size(); state++) {
			if(pointers[pointer].second.get_name() == states[state].get_name()){
				tester++;
			}
		}
		if (tester == 0) {
			states.push_back(pointers[pointer].second);
			
		}
		for (int state = 0; state < states.size(); state++) {
			if (pointers[pointer].second.get_name() == states[state].get_name()) {
				for (int state2 = 0; state2 < states.size(); state2++) {
					if (states[state2].get_name() == pointers[pointer].first.first.get_name()) {
						states[state].add_pointer(pointers[pointer].first.second, &(states[state2]));
						
					}
				}
				
			}
			
		}
	}
	return states;
}

void executeTFA (const char* inputfile, const char* outputfile) {
	State Er_state = State("ERROR");
	
	vector<State> states = parseAutomaton(inputfile, Er_state);
	
	vector<State> temp;
	vector<State> reachable = find_reachable(temp, states[0]);
	
	vector<State> test;
	
	for (int i = 0; i < states.size(); i++) {
		for (int j = 0; j < reachable.size(); j++) {
			if (states[i].get_name() == reachable[j].get_name()) {
				test.push_back(states[i]);
				
			}
		}
	}
	
	Dfa dfa = Dfa(test);
	
	Board board = Board(dfa.get_size());
	
	vector<State> newStates = TFA(board, dfa.find_acceptstates(), dfa);
	
	writeOutput(outputfile, newStates);
}
