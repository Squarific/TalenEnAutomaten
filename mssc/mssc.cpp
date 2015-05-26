#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <stdlib.h>
#include <algorithm> 
#include <stack>
#include <deque>
#include <vector>
#include "../REtoNFA/classes.h"
#include "dfArrow.h"
#include "dfState.h"

using namespace std;

char epsilon = 'e';
string epsilonstring = "e";

typedef deque<state> states;
typedef deque<dfState> dfStates;

void msscWriteToDotFile(dfStates &outputstates, vector<char>& alfabet){
	
	bool errorStateExists=false;
	string s = "step2.gv";
    ofstream outputfile;
    outputfile.open(s.c_str());
	bool writeToFile;
    if ( outputfile.good() ) {
    	writeToFile = true;
    } else {
    	writeToFile = false;
    }
    stringstream output;
    output << "digraph g{\n\trankdir=LR\n";
	for (dfStates::iterator it = outputstates.begin(); it!=outputstates.end(); ++it){
		if(it->startstate==true){
			output<<"\t-1[style=\"invis\"];";
			output<<"\n\t-1->";
			for(int i = 0; i!= it->ids.size(); i++){
				if(i==0){
				output<<'"';
				}
				output<<'q'<<it->ids.at(i);
				if(i==it->ids.size()-1){
					output<<'"'<<";\n";
				}
				else{
					output<<',';
				}
			}

		}
		if(it->acceptstate==true){
			output<<"\t";
			for(int i = 0; i!= it->ids.size(); i++){
				if(i==0){
				output<<'"';
				}				
				output<<'q'<<it->ids.at(i);
				if(i==it->ids.size()-1){
					output<<'"';
				}
				else{
					output<<',';
				}
			}
			output<<"[shape=\"doublecircle\"];\n";
		}
		/*for(vector<arrow>::iterator it2 = it->arrows.begin(); it2!= it->arrows.end(); ++it2){
			
			output<<"\t";
			//output<<it->id;
			output<<'"'<<'q'<<it->id<<'"';
			output<<"->";
			//output<<it2->nextstate.id;
			output<<'"'<<'q'<<it2->nextstate.id<<'"';
			output<<"[label=\"";
			output<<it2->input;
			output<<"\"]";
			output<<";\n";

		}*/

		for(int p = 0 ; p!=alfabet.size(); p++){
			output<<"\t";
			for(int i = 0; i!= it->ids.size(); i++){
				if(i==0){
				output<<'"';
				}
				output<<'q'<<it->ids.at(i);
				if(i==it->ids.size()-1){
					output<<'"';
				}
				else{
					output<<',';
				}
			}
			output<<"->";
			//output<<'"'<<'q'<<it2->nextstate.id<<'"';
			bool transitionExists=false;
			unsigned int arrowCounter = 0;
			for(vector<dfArrow>::iterator u= it->arrows.begin(); u != it->arrows.end(); u++){
				string alfabetElement;
				alfabetElement += alfabet.at(p);
				if(u->input==alfabetElement){
					transitionExists=true;
					break;
				}
				arrowCounter++;
			}
			if(transitionExists){
				for(int i = 0; i!= it->arrows.at(arrowCounter).nextstate.getIds().size(); i++){
					if(i==0){
					output<<'"';
					}	
					output<<'q'<<it->arrows.at(arrowCounter).nextstate.getIds().at(i);
					if(i==it->arrows.at(arrowCounter).nextstate.getIds().size()-1){
						output<<'"';
					}
					else{
						output<<',';
					}
				}
				
			}

			else{
				output<<'"'<<'X'<<'"';	
				errorStateExists=true;			
			}
			output<<"[label=\"";
			output<<alfabet.at(p);
			output<<"\"]";
			output<<";\n";
		}
	}
	if(errorStateExists){
		for(int p = 0 ; p!=alfabet.size(); p++){
			output<<"\t";
			output<<'"'<<'X'<<'"';
			output<<"->";	
			output<<'"'<<'X'<<'"';
			output<<"[label=\"";
			output<<alfabet.at(p);
			output<<"\"]";
			output<<";\n";
		}
	}
	output<<"}";

    if (writeToFile) {
    	outputfile << output.str();
    	outputfile.close();
    } else {
    	cout << "ERROR: MSSC: Could not open file." << endl;
    }
    cout << "MSSC: File saved." << endl;
}

bool stateexixsts(states &inputstates, int stateid){
	
	for(states::iterator i = inputstates.begin(); i!=inputstates.end(); i++){
		
		if(i->id == stateid){
		
			return true;
		}
	}
	return false;
}

state* findstate(states &inputstates, int stateid){
	
	for(states::iterator i = inputstates.begin(); i!=inputstates.end(); i++){
		
		if(i->id == stateid){
		
			return &(*i);
		}
	}
}

void readdotfile(string path, states &inputstates, vector<char> &alfabet){
	int l = 2000;
	ifstream input;
	input.open(path.c_str());	
    char * buffer = new char [l];
	int counter;
	bool startreading = false;
	while(true){
		

		input.getline(buffer,l);
		string output = buffer;

		if(output=="}"){
			break;
		}
		if(startreading){
			if(counter == 0){
			
				state startstate;
				startstate.setstartstate();
				size_t firstquote = output.find("\"");
				startstate.id = atoi(output.substr(firstquote+2, output.size()-2).c_str());
				//cout<<output<<endl;
				//cout<<startstate.id<<endl;
				//pushback
				inputstates.push_front(startstate);// startstate zit dus altijd vooraan.
			}
			if(output.find("doublecircle")!=string::npos){//doublecircle

				size_t firstquote = output.find("\"");
				size_t secondquote = output.find("\"", firstquote+1);
				int endstateid= atoi(output.substr(firstquote+2, secondquote).c_str());			
				state *endstate = findstate(inputstates, endstateid);
				endstate->setacceptstate();

			}
			if(output.find("label")!=string::npos){
				size_t firstquote = output.find("\"");
				size_t secondquote = output.find("\"", firstquote+1);
				int firststateid= atoi(output.substr(firstquote+2, secondquote).c_str());
				size_t thirdquote = output.find("\"", secondquote+1);
				size_t fourthquote = output.find("\"", thirdquote+1);
				int secondstateid= atoi(output.substr(thirdquote+2, fourthquote).c_str());
				size_t fifthquote = output.find("\"", fourthquote+1);
				size_t sixthquote = output.find("\"", fifthquote+1);
				string transitionsymbol = output.substr(fifthquote+1, fifthquote+2).c_str();
				char label = transitionsymbol.at(0);
				if(label != epsilon){
					bool in = false;
					for(vector<char>::iterator k = alfabet.begin(); k!= alfabet.end(); k++){
						if(*k == label){
							in = true;
						}
					}
					if(in==false){
						alfabet.push_back(label);
					}					
				}
				//if(counter == 2)
				//	cout<< firststateid <<' '<<secondstateid<<' '<<label<<endl;
				if(!stateexixsts(inputstates, secondstateid)){

					state secondstate;
					secondstate.id = secondstateid;
					inputstates.push_back(secondstate);
				}
				state *secondstate = findstate(inputstates, secondstateid);	

				if(!stateexixsts(inputstates, firststateid)){

					state firststate;
					firststate.id = firststateid;
					inputstates.push_back(firststate);
				}	

				state *firststate = findstate(inputstates, firststateid);	
				firststate->addarrow(label, *secondstate);
				
			}
			counter++;
		}
		if(output.substr(0, output.size())== "	-1[style=\"invis\"];"){
			startreading = true;
			counter = 0;
		}
	}
	input.close();
}
void printstates(states& inputstates){
	for(states::iterator k = inputstates.begin(); k!=inputstates.end(); k++){
		cout<<k->id<<':'<<endl;
		for(vector<arrow>::iterator j = k->arrows.begin(); j!= k->arrows.end(); j++ ){
			cout<<"		"<<j->input<<' '<<j->nextstate.id<<endl;
		}
	}
}
void printalfabet(vector<char> &alfabet){
	for(vector<char>::iterator k = alfabet.begin(); k!=alfabet.end(); k++){
		cout<<*k<<endl;
	}
}

void eclose(states& inputstates, int id, vector<int> &ecloseids){

	state *start = findstate(inputstates, id);
	ecloseids.push_back(id);

	bool in = false;
	for(vector<int>::iterator k = ecloseids.begin(); k!= ecloseids.end(); k++){
		if(*k == id){
			in = true;
		}
	}
	if(in==false){
		ecloseids.push_back(id);
	}

	for(vector<arrow>::iterator i= start->arrows.begin(); i != start->arrows.end(); i++){
		if (i->input==epsilonstring){
			eclose(inputstates, i->nextstate.id, ecloseids);
		}
	}
}

void removedoubles(vector<int> &ints){
//copied from:http://stackoverflow.com/questions/1041620/whats-the-most-efficient-way-to-erase-duplicates-and-sort-a-vector
	sort( ints.begin(), ints.end() );
	ints.erase( unique( ints.begin(), ints.end() ), ints.end() );
	/*for(vector<int>::iterator i = ints.begin(); i!= ints.end();i++){
		for(vector<int>::iterator j = i+1; j!=ints.end();i++){
			if(*i==*j){
				ints.

			}
		}
	}*/
}

void alleclose(states& inputstates, vector<vector<int> > &allecloseids){

	for(states::iterator i = inputstates.begin(); i!= inputstates.end(); i++){
		vector<int> ecloseids;
		eclose(inputstates, i->id, ecloseids);
		allecloseids.push_back(ecloseids);
	}
}

void printalleclose(vector<vector<int> > &allecloseids){
	
	for(vector<vector<int> >::iterator j = allecloseids.begin(); j!= allecloseids.end(); j++){
		for(vector<int>::iterator k = j->begin(); k!= j->end(); k++){
			cout<<*k<<',';
		}
		cout<<'.'<<endl;
	}
}
bool intVectorEqTest(vector<int> &ints, vector<int> &ints2){

	sort( ints.begin(), ints.end() );
	sort( ints2.begin(), ints2.end() );
	

	if(ints.size()!=ints2.size()){
		return false;
	}
	for(int i = 0; i!= ints.size(); i++){
		if(ints.at(i)!=ints2.at(i)){
			return false;
		}

	}
	return true;
}


void mssc(states& inputstates, dfStates& outputstates, vector<char>& alfabet){

//void eclose(states& inputstates, int id, vector<int> &ecloseids){

	state* start = &inputstates.front();
	vector<int> starteclose;
	eclose(inputstates, start->id, starteclose);
	dfState newstartstate;
	newstartstate.setstartstate();
	newstartstate.ids = starteclose;
	outputstates.push_front(newstartstate);
	int current = 0;// de staat op de welke de transitiefunctie zal worden opgeroepen;

	//while of recursief?
	while(current<outputstates.size()){
		for(int i = 0; i!=alfabet.size(); i++){
			vector<int> nextstateids;
			char trans = alfabet.at(i);
			for(vector<int>::iterator j = outputstates.at(current).ids.begin(); j!=outputstates.at(current).ids.end(); j++){//rec
				state* staat = findstate(inputstates, *j);
				for(vector<arrow>::iterator k = staat->arrows.begin(); k!=staat->arrows.end();k++){
					if(k->input.at(0)==trans){
						eclose(inputstates, k->nextstate.id, nextstateids);
					}
				}//end arrowloop		
			}//end idloop
			removedoubles(nextstateids);
			bool stateAlreadyExists = false;
			int stateLocation = 0;
			for(dfStates::iterator l = outputstates.begin(); l!=outputstates.end(); l++){
				if(intVectorEqTest(nextstateids, l->ids)){
					stateAlreadyExists = true;
					break;
				}
				stateLocation++;
			}
			if(nextstateids.empty()){
				//outputstates.at(0)->addarrow(*i, ouputstates.at(stateLocation));//rec
				//niks doen en bij writetodotfile ervoor zorgen dat voor elk char een transitie bestaat, zij het naar een errorstate
			}
			else if(stateAlreadyExists){
				outputstates.at(current).addarrow(trans, outputstates.at(stateLocation));//rec
			}
			else{
				dfState outputstate;
				outputstate.ids = nextstateids;
				outputstates.push_back(outputstate);
				outputstates.at(current).addarrow(trans, outputstates.at(stateLocation));//rec			
			}
		}// charloop	
		current++; //volgende state
	}
	for(states::iterator i=inputstates.begin(); i!=inputstates.end(); i++){
		if(i->acceptstate){
			int id = i->id;
			for(dfStates::iterator j = outputstates.begin(); j!=outputstates.end(); j++){
				for(int k = 0; k!= j->ids.size(); k++){
					if(j->ids.at(k)==id){						
						j->setacceptstate();
					}
				}
			}
		}
	}
}

void ExecuteMssc(string inputpath) {	
	states inputstates;
	vector<char> alfabet;
	readdotfile(inputpath, inputstates, alfabet);
	dfStates outputstates;
	mssc(inputstates,outputstates,alfabet);
	msscWriteToDotFile(outputstates, alfabet);
}
