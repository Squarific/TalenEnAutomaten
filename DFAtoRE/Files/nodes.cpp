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
	string repeating = "(";
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

	repeating += ")*";

	// Create a new connection for every start state
	// and go to every end state
	for(auto &tocon : toState) {
		for(auto &fromcon : fromState) {
			string newLabel = "(" + tocon.label + ")";
			if (!firstRepeating) {
				newLabel += repeating;
			}
			newLabel += "(" + fromcon.label + ")";

			Connection newconnection = Connection(newLabel, tocon.from, fromcon.to);			

			tocon.from->connections.push_back(newconnection);
			fromcon.to->connections.push_back(newconnection);
	    }
	}

	// Remove all connections to the removed state from the other nodes
	for(auto &tocon : toState) {
		for(vector<Connection>::iterator it = tocon.from->connections.begin(); it != tocon.from->connections.end(); ++it) {
			if ((*it).from == state || (*it).to == state) {
				tocon.from->connections.erase(it);
			}
		}
	}

	for(auto &fromcon : fromState) {
		for(vector<Connection>::iterator it = fromcon.to->connections.begin(); it != fromcon.to->connections.end(); ++it) {
			if ((*it).from == state || (*it).to == state) {
				fromcon.to->connections.erase(it);
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

	// Generate the repeating string, the
	// incoming regex and the outgoing regex
	string repeating = "(";
	string incoming = "";
	string outgoing = "";

	bool firstIncoming = true;
	bool firstOutgoing = true;
	bool firstRepeating = true;
	bool hasOutgoing = false;
	bool hasRepeating = false;

	for(auto &con : acceptState->connections) {
	    if (con.from == con.to && con.from == acceptState) {
	    	if (!firstRepeating) {
	    		repeating += "+";
	    	} else {
	    		hasRepeating = true;
	    		firstRepeating = false;
	    	}
	    	repeating += "+" + con.label;
	    } else if (con.from == acceptState) {
	    	if (!firstOutgoing) {
	    		outgoing += "+";
	    	} else {
	    		firstOutgoing = false;
	    		hasOutgoing = true;
	    	}
	    	outgoing += con.label;
	    } else if (con.to == acceptState) {
	    	if (!firstIncoming) {
	    		incoming += "+";
	    	} else {
	    		firstIncoming = false;
	    	}
	    	incoming += con.label;
	    } else {
	    	cerr << "There was a state added to a node that doesn't connect to this node" << endl;
	    }
	}

	repeating += ")*";

	string regex = incoming;
	
	if (hasOutgoing)
		regex += "(" + outgoing + incoming + ")*";

	if (hasRepeating)
		regex += repeating;

	return regex;
}