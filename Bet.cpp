
//INCLUDES
#include <iostream>
#include <list>
#include "Bet.h"

using namespace std;


void Bet::SetBet(int btype) {

	type = btype;
	cout << "Bet has been set!\n";
	this->PopulateValues();

}


void Bet::PopulateValues() {

	if (type == 1) {
		//SINGLE VALUE BET
		int number = 0;
		while (true) {
			cout << "Please enter a number: ";
			cin >> number;
			if (number <= 0 || number >= 36) {
				cout <<"Invalid! ";
				continue;
			} else {
				cout << number << " Selected!\n";
				values.resize(1);
				values.push_front(number);
				break;
			}
		}
	} else if (type == 2) {
		//SPLIT BET

	} else if (type == 3) {
		//STREET BET

	} else if (type == 4) {
		//CORNER BET

	} else if (type == 5) {
		//FIRST FIVE BET

	} else if (type == 6) {
		//SIX LINE BET

	} else if (type == 7) {
		//TWELVES LINE BET

	} else if (type == 8) {
		//HALF BET

	}


}
