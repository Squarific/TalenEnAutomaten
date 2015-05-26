#include "Automaton.h"
#include "PA.h"

int main (int argc, char *argv[]) {
	std::string input;

	if (argc != 3) {
		std::cerr << "INPUT ERROR: Please input 2 .gv files." << std::endl;
		std::cerr << "Example: PA file1.gv file2.gv" << std::endl;
		return 1;
	}

	Automaton at1;
	at1.parseAutomaton(argv[1]);
	
	Automaton at2;
	at2.parseAutomaton(argv[2]);
	
	Automaton product = productAutomaton(at1, at2);

	product.writeToFile("Output.gv");
	
	return 0;
}