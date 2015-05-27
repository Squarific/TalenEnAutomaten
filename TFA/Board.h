#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "Dfa.h"

using namespace std;

class Board {
	private:
		int size;
		vector <vector <char> > height;
	
	public:
		Board (int amount) {
			size = amount;
			for (int heightSize = 0; heightSize < size; heightSize++){
				vector <char> width;

				for (int widthSize = 0; widthSize < size; widthSize++){
					if (widthSize < heightSize) {
						width.push_back('O');
					}
					else if (widthSize == heightSize){
						width.push_back(' ');
					}
					else {
						width.push_back('X');
					}
				}

				height.push_back(width);
			}
		}
		
		void print();
		
		void first_step (vector <int> places);
		
		void second_step (Dfa dfa);
		
		vector<vector<int> > third_step(Dfa dfa);
		
		bool check_board(Board test);
};

#endif