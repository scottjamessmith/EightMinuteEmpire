#pragma once
#include "GameStrategy.h"
#include "global.h"

using namespace std;
//Exception struct of Wrong input type
struct InputException : public exception
{
	const char* what() const throw()
	{
		return "Your input is invalid.";
	}
};

void bidFirstPlayer(vector<Player*>* players, const int numCoinsPerPlayer, int* supply)
{
	cout << "Beginning the bid for first player." << endl;
	for (int i = 0; i < players->size(); i++)
	{
		int bidAmount;
		while (true)
		{
			cout << *(players->at(i)->name) << ", please enter a bid amount between 0 and " << numCoinsPerPlayer << ": ";
			cin >> bidAmount;
			if (bidAmount < 0 || bidAmount > numCoinsPerPlayer)
			{
				cout << "You've entered a number outside of the correct range. Try again." << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); // See [1]
			}
			else
			{
				players->at(i)->bidFacObj->bidCoins(bidAmount);
				break;
			}
		}
	}
	players->at(0)->bidFacObj->startBidProcess(supply);
}

TournamentMode::TournamentMode()
{
	StrategyG::type = new string("tournament");
}

TournamentMode::~TournamentMode()
{
	delete StrategyG::type;
}

SingleMode::SingleMode()
{
	StrategyG::type = new string("single");
}

SingleMode::~SingleMode()
{
	delete StrategyG::type;
}

void TournamentMode::execute(GameEngine& game)
{
	game.choosePlayers();

	// Choose First player to go

	global::currentPlayerIndex = new int(0);

	cout << "Please choose the first player to start: " << endl;

	for (int i = 0; i < global::players->size(); i++)
	{
		cout << " " << (i + 1) << ")Player " << (i + 1) << " : ";
		cout << *global::players->at(i)->name << endl;
	}

	cout << endl;
	int answerP;
	cin >> answerP;

	global::currentPlayer = global::players->at(answerP - 1);
	auto it = find(global::players->begin(), global::players->end(), global::currentPlayer);
	if (it != global::players->end())
	{
		*global::currentPlayerIndex = static_cast<int>(distance(global::players->begin(), it));
	}
	game.setSupply();
}


void SingleMode::execute(GameEngine& game)
{
	game.setNumOfPlayers();
	game.setSupply();
	for (int i = 0; i < *game.getNumOfPlayers(); i++)
	{
		//TODO: Handle exception of inputs
		string name;
		int age;
		cout << "New player, enter your name: ";
		cin >> name;

		while (true)
		{
			cout << "Enter your age: ";
			try
			{
				cin >> age;
				if (cin.fail() || age > 100 || age < 1)
					throw InputException();
			}
			catch (InputException e)
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << e.what() << endl;
			}
			if (age > 0 && age < 100)
				break;
		}

		Player* player = new Player(name, age, *game.getNumCoinsPerPlayer(), global::COLORS[i], global::main_deck);
		player->set_strategy(new Human());
		global::players->push_back(player);
		player->printPlayer();
		player = NULL;//point to null and delete the pointer
		delete player;
	}

	//Bidding process to know who goes first
	bidFirstPlayer(global::players, *game.getNumCoinsPerPlayer(), global::supply);
	global::currentPlayer = global::players->at(0)->bidFacObj->winner;
	auto it = find(global::players->begin(), global::players->end(), global::currentPlayer);
	if (it != global::players->end())
	{
		*global::currentPlayerIndex = static_cast<int>(distance(global::players->begin(), it));
	}
	global::players->at(0)->bidFacObj->clearBidingFacility();

}