#include "dfArrow.h"
#include "dfState.h"

vector<int> dfState::getIds(){
	return ids;
}

dfState::dfState(){

	startstate=false;
	acceptstate=false;
}

void dfState::addarrow(char &in, dfState &ns){
	
	dfArrow pijl(in, ns);
	arrows.push_back(pijl);
}

dfState::~dfState(){}