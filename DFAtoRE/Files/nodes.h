#ifndef NODES_H
#define NODES_H

using namespace std;

class Node;
class Connection {
public:
	Connection(string label, Node* from, Node* to);
	string label;
	Node* from;
	Node* to;
};

class Node {
public:
	Node(bool begin, bool accept);
	Node(bool begin, bool accept, string name);
	void addConnection(Connection con);
	string name;
	bool begin;
	bool accept;
	vector<Connection> connections;
};

// Eliminates the given state, this alters the nodes
void eliminateState (Node* state);

// Returns a string that is the regex of the given acceptState
string createRegex (Node* acceptState);

#endif