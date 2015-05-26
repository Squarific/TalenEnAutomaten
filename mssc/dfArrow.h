#ifndef DFARROW_H_
#define DFARROW_H_
#include "../REtoNFA/classes.h"
#include "dfState.h"
#include <iostream>
#include <string>

using namespace std
;
class dfArrow{

	public:
		
		string input;
		dfState nextstate;
		dfArrow(char i, dfState &ns);
		~dfArrow();
};

#endif /* DFARROW_H_ */
