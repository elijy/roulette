#ifndef USER_H
#define USER_H

//INCLUDES
#include "socket.h"
#include "Bet.h"
#include <iostream>

using namespace std;


//DEFINITION
class User {

private:
	Socket sSock;
	Socket iSock;
	int cashFlow;
	int id;
	int currentRoll;
	int currentBet;
	int currentAmount;
	string allBets;
	

public:
	User(int startFloat);
	~User();

	bool isBroke();
	Socket listen();

	//BETTING FUNCTIONS
	void placeBet();
	void PromptBet();
	void CheckBet();
	bool WasRed();
	bool WasBlack();
	bool WasOdd();
	bool WasEven();
	bool WasFirstColumn();
	bool WasSecondColumn();
	bool WasThirdColumn();
	bool WasFirstTwelve();
	bool WasSecondTwelve();
	bool WasThirdTwelve();
	bool WasFirstHalf();
	bool WasSecondHalf();
	
	// READING FROM TABLE
	void WaitForBet();
	string ReadFrom_sSocket();

	// WRITING TO TABLE
	void SendID();
	void SendBets();
	void SendMoney();
	void WriteTo_sSocket();

	// READING FROM SERVER
	void SetID();
	void WaitForRoll();
	void WaitForBets();
	string ReadFrom_iSocket();

	// WRITING TO SERVER
	void SendBet();



	int GetID() { return id; };

};


#endif
