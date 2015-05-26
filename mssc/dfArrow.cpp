#include "dfArrow.h"
#include "dfState.h"

dfArrow::~dfArrow(){}

dfArrow::dfArrow(char i, dfState &ns){
	input=i;
	nextstate=ns;
}