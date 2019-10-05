#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "Player.h"

using namespace std;

class BidingFacility;
class Player;


class BidingFacility
{
public:
	// Biding facility is run once
	// Everyone has a bidingFacility

	static int* supply;// Supply is shared among all players
	static Player* winner;// Winner pays the amount to the supply
	static vector<Player*>* players;
	static vector<int>* allBids;
	
	int* amountBid;

	BidingFacility();
	~BidingFacility();

	Player* revealWinner(); //return the winner?
	Player* determineYoungest(Player*,Player*);//Called only if multiple highest bid
	
	void startBidProcess();//Start the bidding process, This method should be the only one used in driver
	void reveal();//Shows everyone's bid
	void giveToSupply(Player*);//Player gives its coins to the supply
	void bidCoins(Player*);// Player bids
	void showDetails();//Shows all players and their attributes for testing purposes
	
};