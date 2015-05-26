#ifndef STATE_H_
#define STATE_H_

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <deque>

using namespace std;

class arrow;

class state{

	public:
		
		static unsigned int idcounter;
		unsigned int id;
		bool startstate;
		bool acceptstate;
		vector<arrow> arrows;
		state(){id=idcounter;idcounter++;startstate=false;acceptstate=false;}
		void addarrow(char &in, state &ns);
		void setstartstate();
		void setacceptstate();
		void resetstartstate();
		void resetacceptstate();

};

class arrow {
	public:
		string input;
		state nextstate;
		arrow(char i, state &ns) {
			input=i;
			nextstate=ns;
		}
};

#endif /* STATE_H_ */
