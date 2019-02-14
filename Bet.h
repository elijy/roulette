#ifndef BET_H
#define BET_H

//INCLUDES
#include <iostream>
#include <list>
using namespace std;

//
// Types
// 
// 1 = Single 
// 2 = Split (too many)
// 3 = Street ( 1-3, 4-6, 7-9, 10-12, 13-15, 16-18, 19-21, 22-24, 25-27, 28-30, 31-33, 34-36 )
// 4 = Corner
// 5 = First Five 
// 6 = Six Line (
// 7 = Twelves ( 1st 12, 2nd 12, 3rd 12, Columns )
// 8 = Halves  (RED, BLACK, ODD, EVEN, HIGH, LOW)


class Bet {

private:
	int type;
	list<int> values;


public:
	//Bet(int bettype);
	void SetBet(int bettype);
	void PopulateValues();

};



#endif


