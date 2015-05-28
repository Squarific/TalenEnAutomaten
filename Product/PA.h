#ifndef PA_H
#define PA_H

#include "Automaton.h"
#include <vector>

bool vectorContains(std::vector<std::string> &vec, std::string str);

Automaton productAutomaton(Automaton& automaton_1, Automaton& automaton_2);

#endif