#include <iostream>
#include <string>
#include <vector>
#include "nodes.h"

using namespace std;

Connection::Connection (string label, Node* from, Node* to) {
	this->label = label;
	this->from = from;
	this->to = to;
}

Node::Node (bool begin, bool accept) {
	this->begin = begin;
	this->accept = accept;
}

Node::Node (bool begin, bool accept, string name) {
	this->begin = begin;
	this->accept = accept;
	this->name = name;
}

void Node::addConnection (Connection con) {
	this->connections.push_back(con);
}

void print_connections (vector<Connection> cons) {
	for(auto &con : cons) {
		cout << con.label << endl;
    }
}

bool isOneBlock (string regex) {
	// This function assumes the ( are paired with a ), if there is a syntax error
	// this function may not work as intended
	int open = 0;

	for (char& c : regex) {
		if (c == '(') open++;
		if (c == ')') open--;
		if (c == '+' && open < 1) return false;
	}

	return true;
}

void eliminateState (Node* state) {
	// toState are the connections
	// GOING TO the to be eliminated state
	vector<Connection> toState;
	vector<Connection> fromState;

	// Don't eliminate begin or accept state or we lose
	// part of the language
	if (state->begin || state->accept) {
		return;
	}

	// Check if there is a self reference in this state
	// and sort all connections into tostate and fromstate
	string repeating = "";
	bool firstRepeating = true;
	for(auto &con : state->connections) {
	    if (con.from == con.to && con.from == state) {
	    	if (!firstRepeating) {
	    		repeating += "+";
	    	} else {
	    		firstRepeating = false;
	    	}
	    	repeating += con.label;
	    } else if (con.from == state) {
	    	fromState.push_back(con);
	    } else if (con.to == state) {
	    	toState.push_back(con);
	    } else {
	    	cerr << "There was a state added to a node that doesn't connect to this node" << endl;
	    }
	}
	
	// If the string that needs repeating is a single char we only need to add a *
	if (repeating.length() > 1) {
		repeating = "(" + repeating + ")*";
	} else {
		repeating += "*";
	}

	// Create a new connection for every start state
	// and go to every end state
	for(auto &tocon : toState) {
		for(auto &fromcon : fromState) {
			string newLabel = "";
			newLabel += tocon.label;

			if (!firstRepeating) {
				if (!isOneBlock(newLabel))
					newLabel = "(" + newLabel + ")";

				newLabel += repeating;
			}

			// If the label contains a + then enclose it before adding
			if (newLabel.length() == 0 || isOneBlock(fromcon.label)){
				newLabel += fromcon.label;
			} else {
				newLabel += "(" + fromcon.label + ")";
			}
			
			Connection newconnection = Connection(newLabel, tocon.from, fromcon.to);

			tocon.from->connections.push_back(newconnection);
			fromcon.to->connections.push_back(newconnection);
	    }
	}

	// Remove all connections to the removed state from the other nodes
	// TODO: delete nodes for memory management
	for(auto &tocon : toState) {
		for (vector<Connection>::iterator it = tocon.from->connections.begin(); it != tocon.from->connections.end(); ++it) {
			if ((*it).to == state) {
				tocon.from->connections.erase(it);
				--it;
			}
		}
	}

	for(auto &fromcon : fromState) {
		for(vector<Connection>::iterator it = fromcon.to->connections.begin(); it != fromcon.to->connections.end(); ++it) {
			if ((*it).from == state) {
				fromcon.to->connections.erase(it);
				--it;
			}
		}
    }
}

string createRegex (Node* acceptState) {
	// Create a regex from a state machine with
	// a single begin and accept state

	if (!acceptState->accept) {
		cerr << "Error: Tried generating regex from non accept state" << endl;
	}

	Node* startState;

	// Generate the repeating string, the
	// incoming regex and the outgoing regex
	string repeating = "";
	string incoming = "";
	string outgoing = "";

	bool firstIncoming = true;
	bool firstOutgoing = true;
	bool firstRepeating = true;
	bool hasOutgoing = false;
	bool hasRepeating = false;

	bool beginFound = false;

	for(auto &con : acceptState->connections) {
    	if (con.from->begin) {
    		beginFound = true;
    		startState = con.from;
    	} else if (con.to->begin) {
    		beginFound = true;
    		startState = con.to;
    	}

    	if (con.label == string("")) {
    		continue;
    	}

	    if (con.from == con.to && con.from == acceptState) {
	    	if (!firstRepeating) {
	    		repeating += "+";
	    	} else {
	    		hasRepeating = true;
	    		firstRepeating = false;
	    	}

	    	if (repeating.length() > 0 && !isOneBlock(con.label)) {
	    		if (!isOneBlock(repeating)) {
		    		repeating = "(" + repeating + ")";
		    	}

	    		repeating += "(" + con.label + ")";
	    	} else {
	    		repeating += con.label;
	    	}

	    } else if (con.from == acceptState) {
	    	if (!firstOutgoing) {
	    		outgoing += "+";
	    	} else {
	    		firstOutgoing = false;
	    		hasOutgoing = true;
	    	}

	    	
	    	if (outgoing.length() > 0 && !isOneBlock(con.label)) {
	    		if (!isOneBlock(outgoing)) {
		    		outgoing = "(" + outgoing + ")";
		    	}

	    		outgoing += "(" + con.label + ")";
	    	} else {
	    		outgoing += con.label;
	    	}
	    } else if (con.to == acceptState) {
	    	if (!firstIncoming) {
	    		incoming += "+";
	    	} else {
	    		firstIncoming = false;
	    	}

	    	if (incoming.length() > 0 && !isOneBlock(con.label)) {
	    		if (!isOneBlock(incoming)) {
		    		incoming = "(" + incoming + ")";
		    	}

	    		incoming += "(" + con.label + ")";
	    	} else {
		    	incoming += con.label;	
	    	}
	    } else {
	    	cerr << "There was a state added to a node that doesn't connect to this node" << endl;
	    }
	}

	if (!beginFound) {
		std::cerr << "No start state when generating regex, did the parsing fail?" << std::endl;
		throw NoStartStateException();
	}

	string startRepeating = "";

	for (auto& con : startState->connections) {
		if (con.from == con.to && con.from == startState) {
			if (startRepeating.length() > 0) {
				startRepeating += "+" + con.label;
			} else {
				startRepeating += con.label;
			}
		}
	}

	if (startRepeating.length() > 1) {
		if (!isOneBlock(incoming))
			incoming = "(" + incoming + ")";

		startRepeating = "(" + startRepeating + ")*";
	}
	else if (startRepeating.length() == 1) {
		if (!isOneBlock(incoming))
			incoming = "(" + incoming + ")";

		startRepeating += "*";
	}	

	incoming += startRepeating;

	string together = "";
	if (hasOutgoing) {
		if (!isOneBlock(incoming))
			incoming = "(" + incoming + ")";

		if (!isOneBlock(outgoing))
			outgoing = "(" + outgoing + ")";

		together = outgoing + incoming;
	}

	if (hasRepeating) {
		if (!isOneBlock(repeating))
			repeating = "(" + repeating + ")";

		if (hasOutgoing)
			together += "+" + repeating;
		else
			together = repeating;
	}

	string regex = "";
	regex += incoming;

	if (together.length() > 1) {
		regex += "(" + together + ")*";
	} else if (together.length() == 1) {
		regex += together + "*";
	}

	return regex;
}