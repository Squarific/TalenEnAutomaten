#ifndef RE_TO_NFA_H
#define RE_TO_NFA_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <deque>
#include "classes.h"

using namespace std;

typedef deque<state> nfa;

typedef stack<nfa> operandstack;

typedef stack<char> operatorstack;

unsigned int precedence(char c);

nfa processinputchar(char &input);

nfa mergenfa(nfa &n1, nfa &n2);

nfa plusop(nfa &n1, nfa &n2);

nfa dot(nfa &n1, nfa &n2);

nfa star(nfa &n1);

string readregexfile(string path);

nfa processinput(operandstack operands, operatorstack operators, const char* regex);

void writeToDotFile(nfa& uitkomst, string s);

void REtoNFA(const char* regex, string s);

#endif