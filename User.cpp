
//INCLUDES

#include "socket.h"
#include "socketserver.h"
#include "User.h"
#include <iostream>
#include <sstream>


//CONSTRUCTOR
User::User(int startFloat) : iSock(Socket("192.168.43.202", 3000)), sSock(listen()) {

	try {
		iSock.Open();
	} catch (int e) {
		std::cout<<"Error: "<<e<<std::endl;
		exit(0);
	}
	this->SetID();
	this->SendID();
	std::cout<<"Player "<<this->GetID()<<" is connected\n";
	cashFlow = startFloat;

}

Socket User::listen() {

	std::cout << "Listening on 5008 for GUI Connection\n";

	SocketServer server(5008);
	
	Socket sock = server.Accept();
	
	std::cout << "Received A Connection!\n";

	return sock;
}

void User::SendID() {

	stringstream ss;
	ss << id;
	string str = ss.str();
	ByteArray ba(str);

	int sz = sSock.Write(ba);

	if (sz <= 0) {
		std::cout<<"Error Writing to sSock\n";
		exit(0);
	} else {
		std::cout<<"Sent ID To sSock\n";
	}
}

void User::placeBet() {


}


void User::PromptBet() {
	std::cout <<"*******************************************************\n";
	std::cout <<"BETTING OPTION (Pays)\n";
	std::cout <<"---------------\n";
	std::cout <<"1. Single Bet (36)\n";
	std::cout <<"2. Split Bet (17)\n";
	std::cout <<"3. Street Bet (11)\n";
	std::cout <<"4. Corner Bet (8)\n";
	std::cout <<"5. First Five Bet (6)\n";
	std::cout <<"6. Six Line Bet (5)\n";
	std::cout <<"7. Twelves Bet (2)\n";
	std::cout <<"8. Halves Bet (1)\n";
	std::cout <<"Please Enter The Number of Type of Bet You would like to make: ";

}
//CHECK IF USER IS BROKE
bool User::isBroke() {

	if (cashFlow == 0) {
		//YOU BROKE
		return true;
	} 

	//NAH, YOU GOOD
	return false;

}

//IF USER IS GOOD WITH THEIR CURRENT BETS, ENTER THIS FUNCTION TO WAIT FOR THE NEXT ROLL
void User::WaitForRoll() {

	//LITERALLY JUST READ FROM THE SOCKET
	ByteArray ba;
	std::cout<<"Waiting For Roll \n";
	int sz = iSock.Read(ba);
	if (sz <= 0)
	{
		//DIDNT WORK
		std::cout<<"Bad Read\n";
		exit(0);
	} else {
		int roll;
		istringstream ( ba.ToString() ) >> roll;
		std::cout <<"Rolled Number: "<<roll<<std::endl;
		currentRoll = roll;
	}
}

//Wait For Bet From GUI
void User::WaitForBet() {

	std::cout<<"Waiting For Bet\n";
	string response = this->ReadFrom_sSocket();
	//FORM #,$
	string bet(response);
	string::size_type pos = bet.find(",");
	string value = bet.substr(0, pos);
	string amount = bet.substr(pos+1, bet.size());

	std::cout<<"Bet : "<<value<<" Amount : "<<amount<<std::endl;
	
	istringstream ( value ) >> currentBet;
	istringstream ( amount ) >> currentAmount;
	
}

void User::WaitForBets() {

	std::cout<<"Waiting For All Bets\n";
	allBets = this->ReadFrom_iSocket();
	std::cout<<"All Bets: "<<allBets<<std::endl;
	

}

string User::ReadFrom_iSocket() {

	ByteArray response;
	int sz = iSock.Read(response);

	if (sz <= 0) {
		std::cout<<"Bad Read!\n";
		exit(0);
	}

	return response.ToString();

}

void User::SendBet() {

	stringstream ss;
	ss << currentBet;
	string bet = ss.str();
	ss.str("");
	ss << currentAmount;
	string amount = ss.str();
	ByteArray ba(bet+","+amount);
	
	int sz = iSock.Write(ba);

	if (sz <= 0) {
		std::cout<<"Error Writing Bet to iSock\n";
		exit(0);
	} else {
		std::cout<<"Wrote "<<sz<<" bytes - sent bet\n";
	}
}


void User::SendMoney() {

	stringstream ss;
	ss << cashFlow;
	string cash = ss.str();
	ByteArray ba(cash);
	
	int sz = sSock.Write(ba);

	if (sz <= 0) {
		std::cout<<"Error Writing Money to iSock\n";
		exit(0);
	} else {
		std::cout<<"Wrote "<<sz<<" bytes - sent money\n";
	}
}

void User::SendBets() {

	ByteArray message(allBets);
	int sz = sSock.Write(message);

	if (sz <= 0) {
		std::cout<<"Error Writing Bets to sSock\n";
		exit(0);
	} else {
		std::cout<<"Wrote "<<sz<<" bytes - sent bet array\n";
	}

}

string User::ReadFrom_sSocket() {
	ByteArray response;
	int sz = sSock.Read(response);
	if (sz <= 0) {
		std::cout<<"Bad Read!\n";
		exit(0);
	}

	return response.ToString();
}

void User::WriteTo_sSocket() {
	ByteArray message("Hello");
	int sz = sSock.Write(message);

	std::cout <<"Wrote "<<sz<<" Bytes \n";
}

void User::SetID() {

	//STREAM INTO INTEGER ID
	istringstream ( (this->ReadFrom_iSocket()) ) >> id;

}

void User::CheckBet() {

	bool win = false;

	std::cout<<"About To Check Bet - Bet:"<<currentBet<<"\n";
	std::cout<<"About To Check Bet - Roll:"<<currentRoll<<"\n";
	std::cout<<"About To Check Bet - Amount:"<<currentAmount<<"\n";
	
	//CHECKING SINGLES
	if (currentBet >= 6 && currentBet <= 41)
	{
		int difference = (currentBet - currentRoll);
		if (difference == 5)
		{
			cashFlow += currentAmount*35;
			std::cout<<"Won - Now Have $"<<cashFlow<<"!\n";
			win = true;
		}
	}

	//CHECKING NON-SINGLES
	switch (currentBet) 
	{
		case 0:
			std::cout<<"Bet Was Red!\n";
			if (this->WasRed())
			{
				cashFlow += currentAmount;
				std::cout<<"Won - Now Have $"<<cashFlow<<"!\n";
				win = true;
			}
			break;

		case 1:
			std::cout<<"Bet Was Black!\n";
			if (this->WasBlack())
			{
				cashFlow += currentAmount;
				std::cout<<"Won - Now Have $"<<cashFlow<<"!\n";
				win = true;
			}
			break;

		case 2:
			std::cout<<"Bet Was Odd!\n";
			if (this->WasOdd())
			{
				cashFlow += currentAmount;
				std::cout<<"Won - Now Have $"<<cashFlow<<"!\n";
				win = true;
			}
			break;

		case 3:
			std::cout<<"Bet Was Even!\n";
			if (this->WasEven())
			{
				cashFlow += currentAmount;
				std::cout<<"Won - Now Have $"<<cashFlow<<"!\n";
				win = true;
			}
			break;
	
		case 4:
			std::cout<<"Bet Was Double Zero!\n";
			if (currentRoll == 37) 
			{
				cashFlow += currentAmount*35;
				std::cout<<"Won - Now Have $"<<cashFlow<<"!\n";
				win = true;
			}
			break;
	
		case 5:
			std::cout<<"Bet Was Zero!\n";
			if (currentRoll == 0) 
			{
				cashFlow += currentAmount*35;
				std::cout<<"Won - Now Have $"<<cashFlow<<"!\n";
				win = true;
			}
			break;
	
		case 42:
			std::cout<<"Bet Was First Column!\n";
			if (this->WasFirstColumn())
			{
				cashFlow += currentAmount*2;
				std::cout<<"Won - Now Have $"<<cashFlow<<"!\n";
				win = true;
			}
			break;

		case 43:
			std::cout<<"Bet Was Second Column!\n";
			if (this->WasSecondColumn())
			{
				cashFlow += currentAmount*2;
				std::cout<<"Won - Now Have $"<<cashFlow<<"!\n";
				win = true;
			}
			break;

		case 44:
			std::cout<<"Bet Was Third Column!\n";
			if (this->WasThirdColumn())
			{
				cashFlow += currentAmount*2;
				std::cout<<"Won - Now Have $"<<cashFlow<<"!\n";
				win = true;
			}
			break;

		case 45:
			std::cout<<"Bet Was First Twelve!\n";
			if (this->WasFirstTwelve())
			{
				cashFlow += currentAmount*2;
				std::cout<<"Won - Now Have $"<<cashFlow<<"!\n";
				win = true;
			}
			break;

		case 46:
			std::cout<<"Bet Was Second Twelve!\n";
			if (this->WasSecondTwelve())
			{
				cashFlow += currentAmount*2;
				std::cout<<"Won - Now Have $"<<cashFlow<<"!\n";
				win = true;
			}
			break;

		case 47:
			std::cout<<"Bet Was Third Twelve!\n";
			if (this->WasThirdTwelve())
			{
				cashFlow += currentAmount*2;
				std::cout<<"Won - Now Have $"<<cashFlow<<"!\n";
				win = true;
			}
			break;

		case 48:
			std::cout<<"Bet Was First Half!\n";
			if (this->WasFirstHalf())
			{
				cashFlow += currentAmount;
				std::cout<<"Won - Now Have $"<<cashFlow<<"!\n";
				win = true;
			}
			break;

		case 49:
			std::cout<<"Bet Was Second Half!\n";
			if (this->WasSecondHalf())
			{
				cashFlow += currentAmount;
				std::cout<<"Won - Now Have $"<<cashFlow<<"!\n";
				win = true;
			}
			break;

		default:
			std::cout<<"This is bad!\n";
			break;
	
	}
	std::cout<<"Checked Bet\n";	

	if (!win) {
		std::cout<<"Better Luck Next Time!\n";	
		cashFlow -= currentAmount;
		std::cout<<"Now Have $"<<cashFlow<<"!\n";	
	}
}


bool User::WasRed() {

	if (currentRoll == 1 || currentRoll == 3 || currentRoll == 5 || currentRoll == 7 || currentRoll == 9 || currentRoll == 12) 
		return true;

	if (currentRoll == 14 || currentRoll == 16 || currentRoll == 18 || currentRoll == 19 || currentRoll == 21 || currentRoll == 23)
		return true;

	if (currentRoll == 25 || currentRoll == 27 || currentRoll == 30 || currentRoll == 32 || currentRoll == 34 || currentRoll == 36)
		return true;

	return false;
}

bool User::WasBlack() {

	if (currentRoll == 2 || currentRoll == 4 || currentRoll == 6 || currentRoll == 8 || currentRoll == 10 || currentRoll == 11)
		return true;

	if (currentRoll == 13 || currentRoll == 15 || currentRoll == 17 || currentRoll == 20 || currentRoll == 22 || currentRoll == 24)
		return true;

	if (currentRoll == 26 || currentRoll == 28 || currentRoll == 29 || currentRoll == 31 || currentRoll == 33 || currentRoll == 35)
		return true;

	return false;
}

bool User::WasOdd() {

	if (currentRoll % 2)
		return true;

	return false;
}

bool User::WasEven() {

	if (!(currentRoll % 2))
		return true;

	return false;
}

bool User::WasFirstColumn() {

	for (int i=1; i <= 34; i+=3)
	{
		if (currentRoll == i)
			return true;
		else
			continue;
	}

	return false;
}

bool User::WasSecondColumn() 
{
	for (int i=2; i <= 35; i+=3)
	{
		if (currentRoll == i)
			return true;
		else
			continue;

	}
	
	return false;

}

bool User::WasThirdColumn() 
{
	for (int i=3; i <= 36; i+=3)
	{
		if (currentRoll == i)
			return true;
		else
			continue;

	}

	return false;
}

bool User::WasFirstTwelve() 
{
	if (currentRoll >= 1 && currentRoll <= 12)
		return true;

	return false;

}

bool User::WasSecondTwelve() 
{

	if (currentRoll >= 13 && currentRoll <= 24)
		return true;

	return false;

}

bool User::WasThirdTwelve() 
{

	if (currentRoll >= 25 && currentRoll <= 36)
		return true;

	return false;

}

bool User::WasFirstHalf() 
{
	if (currentRoll >= 1 && currentRoll <= 18)
		return true;

	return false;
}



bool User::WasSecondHalf() 
{
	if (currentRoll >= 19 && currentRoll <= 36)
		return true;

	return false;
}

User::~User() {
	iSock.Close();
	sSock.Close();
}

