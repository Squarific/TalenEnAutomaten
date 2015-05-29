//assuming syntax correctness and overall validity of input regex
//met een expliciete '.' voor concatenation, en kleine 'e' voor epsilon transitie/lege string
//geinformeerd door:
//http://www.codeproject.com/Articles/5412/Writing-own-regular-expression-parser
//http://www.geeksforgeeks.org/expression-evaluation/
//order of precedence *.+ en eerst wat tussen de haakjes staat
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

unsigned int precedence(char c){
	if(c=='*'){return 5;}
	if(c=='.'){return 4;}
	if(c=='+'){return 3;}
}

nfa processinputchar(char &input){
	
	state begin;
	begin.setstartstate();
	state end;
	end.setacceptstate();
	begin.addarrow(input, end);
	nfa output;
	output.push_front(begin);
	output.push_back(end);
	return output;
}

nfa mergenfa(nfa &n1, nfa &n2){
	//n2 moet verplaatst worden in n1
	state savedback = n1.back();
	n1.pop_back();
	while(!n2.empty()){
		n1.push_back(n2.front());
		n2.pop_front(); 
	}
	n1.push_back(savedback);
	return n1;

}

//neem aan dat startstates vanvoorzitten en acceptstates achteraan
nfa plusop(nfa &n1, nfa &n2){

	char epsilon = 'e';

	nfa output;
	n1.front().resetstartstate();
	n2.front().resetstartstate();
	n1.back().resetacceptstate();
	n2.back().resetacceptstate();
	state newstart;
	newstart.setstartstate();
	state newaccept;
	newaccept.setacceptstate();
	newstart.addarrow(epsilon, n1.front());
	newstart.addarrow(epsilon, n2.front());
	n1.back().addarrow(epsilon, newaccept);
	n2.back().addarrow(epsilon, newaccept);
	output.push_front(newstart);
	output.push_back(newaccept);
	mergenfa(output, n1);
	mergenfa(output, n2);
	return output;
}

nfa dot(nfa &n1, nfa &n2){

	char epsilon = 'e';

	n1.front().setstartstate();
	n1.back().resetacceptstate();
	n2.front().resetstartstate();
	n2.back().setacceptstate();
	n1.back().addarrow(epsilon, n2.front());

	while(!n2.empty()){
		n1.push_back(n2.front());
		n2.pop_front(); 
	}

	return n1;
}

nfa star(nfa &n1){

	char epsilon = 'e';
	state newstart;
	state newend;
	newstart.setstartstate();
	newend.setacceptstate();
	n1.front().resetstartstate();
	n1.back().resetacceptstate();
	newstart.addarrow(epsilon, n1.front());
	newstart.addarrow(epsilon, newend);
	n1.back().addarrow(epsilon, n1.front());
	n1.back().addarrow(epsilon, newend);
	n1.push_front(newstart);
	n1.push_back(newend);
	return n1;
}
string readregexfile(string path){
	int l = 200;
	ifstream input;
	input.open(path.c_str());
	
    char * buffer = new char [l];
	input.getline(buffer,l);
	string output = buffer;
	input.close();
	return output;
}

nfa processinput(operandstack operands, operatorstack operators, const char* regex){
	char epsilon = 'e';
	string input(regex);

	if(input.empty()){
		state begin; 
		state end; 
		nfa total; 
		total.push_back(begin); 
		total.push_back(end); 
		return total;
	}
	if(input.at(0)=='e' and input.size()==1){
		state begin; 
		state end;
		begin.addarrow(epsilon, end);
		nfa total; 
		total.push_back(begin); 
		total.push_back(end); 
		return total;
	}
	for(int i = 0; i != input.length(); ++i) {

		if(input.at(i)=='('){
			operators.push(input.at(i));
			continue;
		}
		else if(input.at(i)==')'){
			while(operators.top()!='('){
				char operater = operators.top();
				operators.pop();
				if (operater=='*'){
					nfa operand=operands.top();
					operands.pop();
					operands.push(star(operand));
					continue;
				}
				else if (operater=='+'){
					nfa topoperand=operands.top();
					operands.pop();
					nfa bottomoperand=operands.top();
					operands.pop();
					operands.push(plusop(bottomoperand, topoperand));
					continue;				
				}
				else if (operater=='.'){
					nfa topoperand=operands.top();
					operands.pop();
					nfa bottomoperand=operands.top();
					operands.pop();
					operands.push(dot(bottomoperand, topoperand));
					continue;						
				}
			}
			operators.pop();//pop de linkerhaak
		}
		else if(input.at(i)=='*' or input.at(i)=='.' or input.at(i)=='+'){
			//kan hier net onder nog een haakje linkerhaakje tegenkome
			while((!operators.empty()) and (operators.top()!='(') and (precedence(operators.top())>=precedence(input.at(i)))){
				char operater = operators.top();
				operators.pop();
				if (operater=='*'){
					nfa operand=operands.top();
					operands.pop();
					operands.push(star(operand));
					continue;
				}
				else if (operater=='+'){
					nfa topoperand=operands.top();
					operands.pop();
					nfa bottomoperand=operands.top();
					operands.pop();
					operands.push(plusop(bottomoperand, topoperand));
					continue;				
				}
				else if (operater=='.'){
					nfa topoperand=operands.top();
					operands.pop();
					nfa bottomoperand=operands.top();
					operands.pop();
					operands.push(dot(bottomoperand, topoperand));
					continue;						
				}
			}
			operators.push(input.at(i));
		}
		
		else{
			operands.push(processinputchar(input.at(i)));
		}
	}
	while(!operators.empty()){
		char operater = operators.top();
		operators.pop();
		if (operater=='*'){
			nfa operand=operands.top();
			operands.pop();
			operands.push(star(operand));
			continue;
		}
		else if (operater=='+'){
			nfa topoperand=operands.top();
			operands.pop();
			nfa bottomoperand=operands.top();
			operands.pop();
			operands.push(plusop(bottomoperand, topoperand));
			continue;				
		}
		else if (operater=='.'){
			nfa topoperand=operands.top();
			operands.pop();
			nfa bottomoperand=operands.top();
			operands.pop();
			operands.push(dot(bottomoperand, topoperand));
			continue;						
		}
	}
   
	if (operands.size()==1) {
		return operands.top();//normaal maar 1 nfa over op het einde,
	} else {
		cout << "We ended up with more than one nfa at the end. The generated nfa is meaningless." << endl;
		nfa meaningless;
		return meaningless;
	}
}

void writeToDotFile(nfa& uitkomst, string s){
    ofstream outputfile;
    outputfile.open(s.c_str());
	bool writeToFile;
    if ( outputfile.good() ) {
    	writeToFile = true;
    } else {
    	writeToFile = false;
    }
    stringstream output;
    output << "digraph \"g\"{\n\trankdir=LR\n";
	for (deque<state>::iterator it = uitkomst.begin(); it!=uitkomst.end(); ++it){
		if(it->startstate==true){
			output<<"\t-1[style=\"invis\"];";
			output<<"\n\t-1->";
			output<<'"'<<'q'<<it->id<<'"';
			output<<";\n";
		}
		if(it->acceptstate==true){
			output<<"\t";
			output<<'"'<<'q'<<it->id<<'"';
			output<<"[shape=\"doublecircle\"];\n";
		}
		for(vector<arrow>::iterator it2 = it->arrows.begin(); it2!= it->arrows.end(); ++it2){
			
			output<<"\t";
			//output<<it->id;
			output<<'"'<<'q'<<it->id<<'"';
			output<<"->";
			//output<<it2->nextstate.id;
			output<<'"'<<'q'<<it2->nextstate.id<<'"';
			output<<"[label=\"";
			output<<it2->input;
			output<<"\"]";
			output<<";\n";

		}
	}

	output<<"}";

    if (writeToFile) {
    	outputfile << output.str();
    	outputfile.close();
    } else {
    	cerr << "ERROR: RE to NFA: Could not open file." << endl;
    }
}

void REtoNFA(const char* regex, string s) {
	operandstack OD;
	operatorstack OT;
	nfa uitkomst = processinput(OD, OT, regex);
	writeToDotFile(uitkomst, s);
}