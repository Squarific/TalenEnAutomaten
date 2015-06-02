#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "../Product/PA.h"

void addNewName(std::vector<std::string> &vec) {
	std::stringstream ss;
	ss << vec.size();
	vec.push_back("q" + ss.str());
}

void readStates(const char* infile, std::vector<std::string> &stateNames, std::vector<std::string> &newNames) {
	std::string stateFrom, stateTo, newLine;
	
	std::ifstream input;
	input.open(infile);
	
	for (std::string line; getline(input, line); ) {
		// Name should be on the first line
		if (line.find("digraph ") != -1) {
			continue;
		}

		// Start state
		else if (line.find("-1->") != -1) {
			stateFrom = line;
			stateFrom.erase(0, stateFrom.find("\"") + 1);
			stateFrom.erase(stateFrom.find("\""), stateFrom.size());

			if (! vectorContains(stateNames, stateFrom)) {
				stateNames.push_back(stateFrom);
				addNewName(newNames);
			}
		}

		// Transitions
		else if (line.find("->") != -1) {
			if (line.find("[label=\"") != -1) {
				// stateFrom
				stateFrom = line;
				stateFrom.erase(0, stateFrom.find("\"") + 1);
				stateFrom.erase(stateFrom.find("\""), stateFrom.size());

				// stateTo
				stateTo = line;
				for (int j = 0; j < 3; j++) {
					stateTo.erase(0, stateTo.find("\"") + 1);
				}
				stateTo.erase(stateTo.find("\""), stateTo.size());

				if (! vectorContains(stateNames, stateFrom)) {
					stateNames.push_back(stateFrom);
					addNewName(newNames);
				}

				if (! vectorContains(stateNames, stateTo)) {
					stateNames.push_back(stateTo);
					addNewName(newNames);
				}
			}
			else {
				std::cerr << "PARSING ERROR: Line has incorrect syntax: " << std::endl << line << std::endl
				<< "No label found." << std::endl << std::endl;				
			}
		}
		else if (line.find("}") != -1) {
			break;
		}
		else if (line.find("rankdir") != -1) {
			continue;
		}
		else if (line.find("-1[style") != -1) {
			continue;
		}
		else if (line.find("doublecircle") != -1) {
			stateFrom = line;
			stateFrom.erase(0, stateFrom.find("\"") + 1);
			stateFrom.erase(stateFrom.find("\""), stateFrom.size());

			if (! vectorContains(stateNames, stateFrom)) {
				stateNames.push_back(stateFrom);
				addNewName(newNames);
			}
		}
		else {
			std::cerr << "PARSING ERROR: Line has incorrect syntax: " << std::endl << line << std::endl;
		}
	}

	input.close();
}

void writeStates(const char* infile, const char* outfile, std::vector<std::string> &stateNames, std::vector<std::string> &newNames) {
	std::string newLine;

	std::ifstream input;
	input.open(infile);

	std::ofstream output;
	output.open(outfile);
	
	for (std::string line; getline(input, line); ) {
		newLine = line;

		for (int i = 0; i < stateNames.size(); i++) {
			while (newLine.find(stateNames[i]) != -1) {
				newLine.replace(newLine.find(stateNames[i]), stateNames[i].length(), newNames[i]);
			}
		}

		output << newLine << std::endl;
	}
}

void beautify(const char* infile, const char* outfile) {
	std::vector<std::string> stateNames, newNames;

	readStates(infile, stateNames, newNames);
	writeStates(infile, outfile, stateNames, newNames);
}