#include <iostream>
#include <vector>
#include "./Automaton/parsers.h"
#include "./Automaton/nodes.h"

int main(int argc, char const *argv[]) {

	vector<Node*> allStates = nodesFromDotFile(argv[1]);

	// Create one endstate
	Node* newEndState = new Node(false, true);
	for(auto &state : allStates) {
		if (state->accept) {
			Connection oldToNew = Connection("ε", state, newEndState);

			newEndState->addConnection(oldToNew);
			state->addConnection(oldToNew);
			
			state->accept = false;
		}
	}
	allStates.push_back(newEndState);

	// Eliminate all states
	for(auto &state : allStates) {
		eliminateState(state);
	}

	// Find the end state and give the regex
	for(auto &state : allStates) {
		if (state->accept) {
			cout << createRegex(state) << endl;
		}
	}

	return 0;
}