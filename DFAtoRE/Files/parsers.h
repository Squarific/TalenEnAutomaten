#ifndef PARSERS_H
#define PARSERS_H

#include "nodes.h"
using namespace std;

// Function that returns the nodes with their connection
// as specified in an xml file.

// Example format:
// <AUTOMATON>
// 	<states>
// 		<state begin="true" accept="false">a</state>
// 		<state begin="false" accept="false">b</state>
// 		<state begin="false" accept="false">c</state>
// 		<state begin="false" accept="true">d</state>
// 	</states>
// 	<connections>
// 		<connection from="a" to="b">k</connection>
// 		<connection from="b" to="c">l</connection>
// 		<connection from="c" to="d">m</connection>
// 		<connection from="c" to="c">i</connection>
// 		<connection from="a" to="b">o</connection>
// 		<connection from="c" to="d">p</connection>
// 	</connections>
// </AUTOMATON>

// vector<Node*> nodesFromXMLFile (const char* filename);

// Function that returns the nodes with their connection
// as specified in a dot file.

vector<Node*> nodesFromDotFile (const char* filename);

#endif