#include "./REtoNFA/REtoNFA.h"
#include "./mssc/mssc.h"

int main (int argc, char *argv[]) {
	if (argc != 2) {
		std::cerr << "ERROR: Must supply a regex. Example:" << std::endl << "simplify a+b" << std::endl;

		return 1;
	}

	REtoNFA(argv[1]);

	ExecuteMssc("step1.gv");

	return 0;
}