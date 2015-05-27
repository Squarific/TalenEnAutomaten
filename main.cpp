#include "./REtoNFA/REtoNFA.h"
#include "./mssc/mssc.h"
#include "./TFA/Tfamain.h"
#include "./DFAtoRE/convert.h"

int main (int argc, char *argv[]) {
	if (argc != 2) {
		std::cerr << "ERROR: Must supply a regex. Example:" << std::endl << "simplify a+b" << std::endl;

		return 1;
	}

	REtoNFA(argv[1], "step1.gv");

	executeMssc("step1.gv", "step2.gv");

	executeTFA("step2.gv", "step3.gv");

	convertToRegex("step3.gv");

	return 0;
}