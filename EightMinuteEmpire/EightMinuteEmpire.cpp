#include <iostream>
#include <limits>
#include <vector>
#include "cards.h"
#include "Player.h"
#include "Map.h"
#include "MapLoader.h"

using namespace std;

int getNumOfPlayers()
{
	int playerInput;
	while (true)
	{
		cout << "Enter the number of players [2-5] who will be playing this game: ";
		cin >> playerInput;
		if (cin.fail() || playerInput < 2 || playerInput > 5)
		{
			cout << "You've entered a number outside of the correct range. Try again." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // See [1]
		}
		else
		{
			cout << endl;
			break;
		}
	}
	return playerInput;
}

int getNumCoinsPerPlayer(const int numPlayers)
{
	int numCoinsPerPlayer = 0;
	switch (numPlayers)
	{
	case 2:
		numCoinsPerPlayer = 14;
		break;
	case 3:
		numCoinsPerPlayer = 11;
		break;
	case 4:
		numCoinsPerPlayer = 9;
		break;
	case 5:
		numCoinsPerPlayer = 8;
		break;
	default:
		break;
	}
	return numCoinsPerPlayer;
}

string getMapFileName()
{
	string mapFileName = "";
	int fileSelectInput;

	cout << "The following map choices exist for the game:" << endl;
	cout << "\t[1] map1.map" << endl;
	cout << "\t[2] invalidMap.map" << endl;
	cout << "\t[3] nonexistantMap.map" << endl;
	while (true)
	{
		cout << "Enter the number of the map to use for the game: ";
		cin >> fileSelectInput;
		if (fileSelectInput < 1 || fileSelectInput > 3)
		{
			cout << "You've entered a number outside of the correct range. Try again." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // See [1]
		}
		else
		{
			cout << endl;
			break;
		}
	}
	switch (fileSelectInput)
	{
	case 1:
		mapFileName = "map1.map";
		break;
	case 2:
		mapFileName = "invalidMap.map";
		break;
	case 3:
		mapFileName = "nonexistantMap.map";
		break;
	default:
		break;
	}
	return mapFileName;
}

void createPlayers(const int numPlayers, const int numCoinsPerPlayer, vector<Player*>* players, Deck* deck, const string colors[])
{
	for (int i = 0; i < numPlayers; i++)
	{
		string name;
		int age;
		cout << "New player, enter your name: ";
		cin >> name;
		cout << "Enter your age: ";
		cin >> age;
		Player* player = new Player(name, age, numCoinsPerPlayer, colors[i], deck);
		players->push_back(player);
		player->printPlayer();
	}
}

void placeInitialPlayerArmies(vector<Player*>* players, Country* startingCountry)
{
	cout << "Each player must now place 3 of their army cubes on the starting country." << endl;
	for (int i = 0; i < players->size(); i++)
	{
		players->at(i)->placeNewArmies(startingCountry, 3);
	}
	cout << *(startingCountry->name) << ", the starting country, holds the following:" << endl;
	for (int i = 0; i < startingCountry->occupyingArmies->size(); i++)
	{
		cout << "A " << *(startingCountry->occupyingArmies->at(i)->player->color) << " army cube belonging to ";
		cout << *(startingCountry->occupyingArmies->at(i)->player->name) << endl;
	}
	cout << endl;
}

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

int main()
{
	// #################################################
	//				Part 1: Game Start
	// #################################################

	cout << "########################################" << endl;
	cout << "\tEight Minute Empire" << endl;
	cout << "########################################\n" << endl;

	// Select map from list of files.
	Map* map = new Map();
	MapLoader* mapLoader = nullptr;
	while (true)
	{
		string mapFileName = getMapFileName();
		try
		{
			mapLoader = new MapLoader(mapFileName, map);
			break;
		}
		catch (const MapLoaderException& e)
		{
			cout << e.message << endl;
			cout << "Try again." << endl;
			cout << endl;
			delete mapLoader;
		}
	}
	cout << "You've selected the following map: " << endl;
	map->print();
	cout << endl;
	Country* startingCountry = map->mapCountries->at(0); // SHOULD WE DECIDE THIS ANOTHER WAY?

	// Create deck with 42 cards.
	Deck* deck = new Deck();
	cout << "A deck containing " << deck->stackofCards->size() << " cards is assigned to the game.\n" << endl;
		
	// Select number of players.
	const int NUM_PLAYERS = getNumOfPlayers();	
	const int NUM_COINS_PER_PLAYER = getNumCoinsPerPlayer(NUM_PLAYERS);	

	// Create correct number of players.
	vector<Player*>* players = new vector<Player*>();
	const string COLORS[5] = { "Red", "Blue", "Green", "Yellow", "White" };
	createPlayers(NUM_PLAYERS, NUM_COINS_PER_PLAYER, players, deck, COLORS);

	//Other details for setting up the game
	bool endGame = false;

	// #################################################
	//				Part 2: Startup Phase
	// #################################################

	// Create the supply of coins (value after each player has taken the appropriate amount of coins for themselves).
	int supplyVal = 44 - (NUM_COINS_PER_PLAYER * NUM_PLAYERS);
	int* supply = &supplyVal;
	cout << "Total supply of coins was 44, but since each player took " << NUM_COINS_PER_PLAYER << " coins, supply is now at " << supplyVal << " coins.\n" << endl;

	// Shuffle deck an draw 6 cards.

	// Place 3 armies of each player on the starting country.
	placeInitialPlayerArmies(players, startingCountry);

	// Bidding process
	Player* currentPlayer;
	int* currentPlayerIndex = new int(0);

	bidFirstPlayer(players, NUM_COINS_PER_PLAYER, supply);
	currentPlayer = players->at(0)->bidFacObj->winner;
	auto it = find(players->begin(), players->end(), currentPlayer);
	if (it != players->end())
	{ 
		*currentPlayerIndex = static_cast<int>(distance(players->begin(), it));
	}
	players->at(0)->bidFacObj->clearBidingFacility();

	cout << "Supply is now at " << *(supply) << " coins." << endl;
	cout << "First player is " << *(currentPlayer->name) << ", now with " << *(currentPlayer->numCoins) << " coins." << endl;
	cout << "First player index is " << *currentPlayerIndex << "\n" << endl;

	cout << "Let the game begin!\n" << endl;

	// #################################################
	//				Part 3: Main Game Loop
	// #################################################

	while (!endGame)
	{
		//Increasing the index position is clockwise 
		delete currentPlayerIndex;

		currentPlayerIndex = new int((*currentPlayerIndex ++)%(NUM_PLAYERS));

		currentPlayer = players->at(*currentPlayerIndex);//update current player

		cout << "Supply is now at " << *(supply) << " coins." << endl;
		cout << "Current player is " << *(currentPlayer->name) << ", now with " << *(currentPlayer->numCoins) << " coins." << endl;
		cout << "Current player index is " << *currentPlayerIndex << "\n" << endl;

		//Current user takes one face-up card & pay the cost of the card
		Cards* chosenCard;
		int cardPosition;
		cout << "Select one of the face-up cards" << endl;
		cin >> cardPosition;
		chosenCard = faceupcards->at(--cardPosition); 
		currentPlayer->pickUpCard(chosenCard, cardPosition, supply);

		//Display current player's action

	}

		// #################################################
		//				Part 4: Player Actions
		// #################################################

		// #################################################
		//				Part 5: After Action
		// #################################################

	// #################################################
	//			Part 6: Game End, Compute Score
	// #################################################

	// #################################################
	//						Cleanup
	// #################################################

	for (int i = 0; i < NUM_PLAYERS; i++)
	{
		delete players->at(i);
	}
	delete players;
	players = NULL;
	delete deck;
	deck = NULL;
	delete mapLoader;
	mapLoader = NULL;
	delete map;
	map = NULL;
	startingCountry = NULL;
	supply = NULL;
	currentPlayer = NULL;
	delete currentPlayerIndex;
	currentPlayerIndex = NULL;
}

 //Run program: Ctrl + F5 or Debug > Start Without Debugging menu
 //Debug program: F5 or Debug > Start Debugging menu

 //Tips for Getting Started: 
 //  1. Use the Solution Explorer window to add/manage files
 //  2. Use the Team Explorer window to connect to source control
 //  3. Use the Output window to see build output and other messages
 //  4. Use the Error List window to view errors
 //  5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
 //  6. In the future, to open this project again, go to File > Open > Project and select the .sln file

// References
// [1] https://www.hackerearth.com/practice/notes/validating-user-input-in-c/
