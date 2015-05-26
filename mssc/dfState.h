#ifndef DFSTATE_H_
#define DFSTATE_H_
#include "../REtoNFA/classes.h"
#include "dfArrow.h"
#include <iostream>
#include <string>

using namespace std;

class arrow;
class dfArrow;
class dfState: public state{

	public:
		
		vector<int> ids;
		vector<int> getIds();
		vector<dfArrow> arrows;
		void addarrow(char &in, dfState &ns);
		dfState();
		~dfState();
};


#endif /* DFSTATE_H_ */
