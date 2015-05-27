#include <fstream>
#include <iostream>

#include <string>
#include <vector>

#include "./TinyXML/tinyxml.h"
#include "nodes.h"

using namespace std;

// Returns a vector of addresses from nodes
// Nodes have to be deleted when no longer required
// vector<Node*> nodesFromXMLFile (const char* filename) {
// 	// Create xml dom
// 	TiXmlDocument doc(filename);
	
// 	// Load the document
// 	if (!doc.LoadFile()) {
// 		cerr << "File " << filename << " not found" << endl;
// 		return vector<Node*> {};
// 	}

// 	// Get root element
// 	TiXmlElement* root = doc.FirstChildElement();
// 	if (root == NULL) {
// 		cerr << "XML Error: No root element" << endl;
// 		return vector<Node*> {};
// 	}

// 	// Root element should be 'AUTOMATON'
// 	if (string(root->Value()) != "AUTOMATON") {
// 		cerr << "XML Error: Root element has to be called 'AUTOMATON' but was '" << root->Value() << "'" << endl;
// 		return vector<Node*> {};
// 	}

// 	TiXmlElement* states = root->FirstChildElement();
// 	TiXmlElement* connections = states->NextSiblingElement();

// 	vector<Node*> allStates;
	
// 	// Parse the states
// 	for (TiXmlElement* state = states->FirstChildElement(); state != NULL; state = state->NextSiblingElement()) {
// 		string begin = state->Attribute("begin");
// 		string accept = state->Attribute("accept");

// 		bool beginBool;
// 		bool acceptBool;

// 		if (begin == "true") {
// 			beginBool = true;
// 		} else {
// 			beginBool = false;
// 		}

// 		if (accept == "true") {
// 			acceptBool = true;
// 		} else {
// 			acceptBool = false;
// 		}

// 		TiXmlNode* TXnode = state->FirstChild();
// 		TiXmlText* text = TXnode->ToText();
// 		string statename = text->Value();

// 		allStates.push_back(new Node(beginBool, acceptBool, statename));
// 	}

// 	for (TiXmlElement* conEl = connections->FirstChildElement(); conEl != NULL; conEl = conEl->NextSiblingElement()) {
// 		string from = conEl->Attribute("from");
// 		string to = conEl->Attribute("to");

// 		TiXmlNode* node = conEl->FirstChild();
// 		TiXmlText* text = node->ToText();
// 		string label = text->Value();

// 		Node* fromState;
// 		Node* toState;

// 		for(auto &state : allStates) {
// 			if (state->name == from) {
// 				fromState = state;
// 			}
// 			if (state->name == to) {
// 				toState = state;
// 			}
// 		}

// 		Connection con = Connection(label, fromState, toState);

// 		for(auto &state : allStates) {
// 			if (state->name == from) {
// 				state->addConnection(con);
// 			} else if (state->name == to) {
// 				state->addConnection(con);
// 			}
// 		}
// 	}

// 	return allStates;
// }

vector<Node*> nodesFromDotFile (const char* filename) {
	std::string name;
	bool start_set = false;
	vector<Node*> allStates;

	std::ifstream input;
	input.open(filename);
	
	// Create all the states
	for(std::string line; getline(input, line); ) {
		if (line.find("->") != -1) {
			if (line.find("[label=\"") != -1) {

				// stateFrom
				string from = line;
				from.erase(0, from.find("\"") + 1);
				from.erase(from.find("\""), from.size());

				// stateTo
				string to = line;
				for (int j = 0; j < 3; j++) {
					to.erase(0, to.find("\"") + 1);
				}
				to.erase(to.find("\""), to.size());

				bool fromAlreadyAdded = false;
				bool toAlreadyAdded = false;

				for(auto &state : allStates) {
					if (state->name == from) {
						fromAlreadyAdded = true;
					}
					if (state->name == to) {
						toAlreadyAdded = true;
					}
				}

				if (!fromAlreadyAdded) {
					allStates.push_back(new Node(false, false, from));
					start_set = true;
				}

				if (!toAlreadyAdded) {
					allStates.push_back(new Node(false, false, to));
					start_set = true;
				}

			} else {
				if (line.find("-1->")) {
					// stateTo
					string to = line;
					to.erase(0, to.find("\"") + 1);
					to.erase(to.find("\""), to.size());

					for(auto &state : allStates) {
						if (state->name == to) {
							state->accept = true;
							continue;
						}
					}

					allStates.push_back(new Node(true, false, to));
				} else {
					std::cerr << "PARSING ERROR: incorrect syntax: " << std::endl << line << std::endl
					<< "No label found." << std::endl << std::endl;
				}
			}
		} else if (line.find("}") != -1) {
			break;
		} else if (line.find("[shape=\"doublecircle\"]") != -1) {
			string endstate = line;
			endstate.erase(0, endstate.find("\"") + 1);
			endstate.erase(endstate.find("\""), endstate.size());

			for(auto &state : allStates) {
				if (state->name == endstate) {
					state->accept = true;
				}
			}
		} else if (line.find("digraph") != -1) {
			continue;
		}
	}

	input.close();
	input.open(filename);

	// Create all the connections
	for(std::string line; getline(input, line); ) {
		if (line.find("->") != -1) {
			if (line.find("[label=\"") != -1) {
				// stateFrom
				string from = line;
				from.erase(0, from.find("\"") + 1);
				from.erase(from.find("\""), from.size());

				// stateTo
				string to = line;
				for (int j = 0; j < 3; j++) {
					to.erase(0, to.find("\"") + 1);
				}
				to.erase(to.find("\""), to.size());

				// label
				name = line;
				for (int j = 0; j < 5; j++) {
					name.erase(0, name.find("\"") + 1);
				}
				name.erase(name.find("\""), name.size());

				Node* fromState;
				Node* toState;

				for(auto &state : allStates) {
					if (state->name == from) {
						fromState = state;
					}
					if (state->name == to) {
						toState = state;
					}
				}

				Connection con = Connection(name, fromState, toState);

				for(auto &state : allStates) {
					if (state->name == from) {
						state->addConnection(con);
					} else if (state->name == to) {
						state->addConnection(con);
					}
				}
			}
			else {
				if (!line.find("-1")) {
					std::cerr << "PARSING ERROR: incorrect syntax: " << std::endl << line << std::endl
					<< "No label found so I ignored this line." << std::endl << std::endl;	
				}		
			}
		}
		else if (line.find("}") != -1) {
			break;
		}
		else if (line.find("[shape=\"doublecircle\"]")) {
			continue;
		}
	}

	input.close();

	return allStates;
}