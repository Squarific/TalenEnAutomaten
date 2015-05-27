#include <vector>
#include <iostream>
#include "Board.h"
#include "Dfa.h"

using namespace std;

void Board::print() {
	cout << " ";
	for (int counter = 0; counter < size; counter++) {
		cout << counter;
	}
	cout << '\n';
	for (int y = 0; y < size; y++) {
		cout << y;
		for (int x = 0; x < size; x++) {
			cout << height[y][x];
			
		}
		cout << "\n";
	}
}

void Board::first_step (vector <int> places) {
	int x, y, other, place, test;
	for (place = 0; place < places.size(); place++) {
		for (x = 0; x < size; x++) {
			for (other = 0; other < places.size(); other++) {
				if (x == places[other]) {
					test++;
				}
			}
			if (test == 0) {
				y = places[place];
				height[x][y] = 'X';
			}
			test = 0;
		}	
		
		for (y = 0; y < size; y++) {
			for (other = 0; other < places.size(); other++) {
				if (y == places[other]) {
					test++;
				}
			}
			if (test == 0) {
				x = places[place];
				height[x][y] = 'X';
			}
			test = 0;
		}
	}
}

void Board::second_step (Dfa dfa) {
	int coord_x;
	int coord_y;
	for (int x = 0; x < height.size(); x++) {
		for (int y = 0; y < height[x].size(); y++) {
			if (height[x][y] == 'O') {
				int test = 0;
				vector <vector <int> > second;
				vector <int> first;
				first.push_back(x);
				first.push_back(y);
				second = dfa.get_pointers(first);
				for (int pairs = 0; pairs < second.size(); pairs++) {
					if (second[pairs][0] > second[pairs][1]) {
					coord_x = second[pairs][0];
					coord_y = second[pairs][1];
					}
					else {
					coord_x = second[pairs][1];
					coord_y = second[pairs][0];
					}
					if (height[coord_x][coord_y] == 'X') {
						test++;
						
					}
				}
				if (test > 0) {
					height[x][y] = 'X';
					
				}
			}
		}	
	}
	
}

vector<vector<int> > Board::third_step(Dfa dfa) {
	vector<vector<int> > states;
	vector<int> test;
	for (int y = 0; y < size; y++) {
		int tester = 0;
		vector<int> temp;
		temp.push_back(y);
		for (int x = y + 1; x < size; x++) {
			if (height[x][y] == 'O') {
				temp.push_back(x);
				test.push_back(x);
			} 
		}
		for (int test_int = 0; test_int < test.size(); test_int++){
			if (test[test_int] == y) {
				tester++;
				
			}
			
		}
		if (tester == 0) {
			states.push_back(temp);					
			
		}
	}
	return states;
}

bool Board::check_board(Board test) {
	for (int x = 0; x < height.size(); x++) {
		for (int y = 0; y < height[x].size(); y++) {
			if (height[x][y] != test.height[x][y]) {
				return false;
				
			}
		}
	}
	return true;
}