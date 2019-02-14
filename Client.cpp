#include "socket.h"
#include "User.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

int main() {

	try {

		User* client = new User(1000);

		while (true)
		{
			client->WaitForBet();
			client->SendBet();
			client->WaitForBets();
			client->SendBets();
			client->WaitForRoll();
			client->CheckBet();
			if (client->isBroke())
			{
				std::cout<<"You Broke - Kicked Out\n";
				break;
			} else {
				client->SendMoney();
			}
		}

        cout << "Sleep now" << endl;
    }
    catch(string s)
    {
        cout << s << endl;
    }
    catch(...)
    {
        cout << "Caught unexpected exception" << endl;
    }

}
