#include "classes.h"

unsigned int state::idcounter=0;

void state::addarrow(char &in, state &ns){
	arrow pijl(in, ns);
	arrows.push_back(pijl);
}

void state::setstartstate(){startstate=true;}

void state::setacceptstate(){acceptstate=true;}

void state::resetstartstate(){startstate=false;}

void state::resetacceptstate(){acceptstate=false;}