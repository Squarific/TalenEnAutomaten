#include "./REtoNFA/REtoNFA.h"
#include "./mssc/mssc.h"
#include "./TFA/Tfamain.h"
#include "./DFAtoRE/convert.h"
#include "./Product/Product.h"
#include "./Beautify/Beautify.h"

int main (int argc, char *argv[]) {
	// Simplify
	if (argc == 2) {
		REtoNFA(argv[1], "step1.gv");
		executeMssc("step1.gv", "step2.gv");
		executeTFA("step2.gv", "step3.gv");
		convertToRegex("step3.gv");
	}

	// Product + Simplify
	else if (argc == 3) {
		REtoNFA(argv[1], "step1a.gv");
		REtoNFA(argv[2], "step1b.gv");
		executeMssc("step1a.gv", "step2a.gv");
		executeMssc("step1b.gv", "step2b.gv");
		Product("step2a.gv", "step2b.gv", "step3.gv");
		executeTFA("step3.gv", "step4.gv");
		convertToRegex("step4.gv");		
	}

	// Error
	else {
		std::cerr << "ERROR: Must supply a regex. Example:" << std::endl << "simplify a+b" << std::endl;
		return 1;
	}

	return 0;
}