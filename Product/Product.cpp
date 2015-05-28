#include "Automaton.h"
#include "PA.h"
#include <string>

void Product(const char* infile1, const char* infile2, const char* outfile) {
	Automaton at1;
	at1.parseAutomaton(infile1);
	
	Automaton at2;
	at2.parseAutomaton(infile2);
	
	Automaton product = productAutomaton(at1, at2);

	product.writeToFile(outfile);
}