//#include <iostream>
//#include <limits>
//#include <vector>
//#include "Player.h"
//#include "MapLoader.h"
//#include "Actions.h"
//#include "computeScore.h"
//#include "global.h"
//
//using namespace std;
//
////Exception struct of Wrong input type
//struct InputException : public exception
//{
//	const char* what() const throw()
//	{
//		return "Your input is invalid.";
//	}
//};
//
////Exception struct if insufficient Coins
//struct InsufficientCoinsException : public exception
//{
//	const char* what() const throw()
//	{
//		return "Player does not have enough coins.";
//	}
//};
//
////Retrieve numbers of players for the game
//int getNumOfPlayers()
//{
//	int playerInput;
//	while (true)
//	{
//		cout << "Enter the number of players [2-5] who will be playing this game: ";
//		cin >> playerInput;
//		if (cin.fail() || playerInput < 2 || playerInput > 5)
//		{
//			cout << "You've entered a number outside of the correct range. Try again." << endl;
//			cin.clear();
//			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // See [1]
//		}
//		else
//		{
//			cout << endl;
//			break;
//		}
//	}
//	return playerInput;
//}
//
////Determine the amount of cards per player to reach to end the game
//int determineMaxCards(int num_player)
//{
//	const int MAX_CARDS2 = 13;
//	const int MAX_CARDS3 = 10;
//	const int MAX_CARDS4 = 8;
//	const int MAX_CARDS5 = 7;
//
//	switch (num_player)
//	{
//	case 2:
//		return MAX_CARDS2;
//	case 3:
//		return MAX_CARDS3;
//	case 4:
//		return MAX_CARDS4;
//	default:
//		return MAX_CARDS5;
//
//	}
//}
//
////Determine the numbers of coins per player at the beginning of the game
//int getNumCoinsPerPlayer(const int numPlayers)
//{
//	int numCoinsPerPlayer = 0;
//	switch (numPlayers)
//	{
//	case 2:
//		numCoinsPerPlayer = 14;
//		break;
//	case 3:
//		numCoinsPerPlayer = 11;
//		break;
//	case 4:
//		numCoinsPerPlayer = 9;
//		break;
//	case 5:
//		numCoinsPerPlayer = 8;
//		break;
//	default:
//		break;
//	}
//	return numCoinsPerPlayer;
//}
//
////getMapFileName returns the name of the file
//string getMapFileName()
//{
//	string mapFileName = "";
//	int fileSelectInput;
//
//	cout << "The following map choices exist for the game:" << endl;
//	cout << "\t[1] map1.map" << endl;
//	cout << "\t[2] invalidMap.map" << endl;
//	cout << "\t[3] nonexistantMap.map" << endl;
//	while (true)
//	{
//		cout << "Enter the number of the map to use for the game: ";
//		cin >> fileSelectInput;
//		if (fileSelectInput < 1 || fileSelectInput > 3)
//		{
//			cout << "You've entered a number outside of the correct range. Try again." << endl;
//			cin.clear();
//			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // See [1]
//		}
//		else
//		{
//			cout << endl;
//			break;
//		}
//	}
//	switch (fileSelectInput)
//	{
//	case 1:
//		mapFileName = "map1.map";
//		break;
//	case 2:
//		mapFileName = "invalidMap.map";
//		break;
//	case 3:
//		mapFileName = "nonexistantMap.map";
//		break;
//	default:
//		break;
//	}
//	return mapFileName;
//}
//
////Iterate and create players for the game
//void createPlayers(const int numPlayers, const int numCoinsPerPlayer, vector<Player*>* players, Deck* deck, const string colors[])
//{
//	for (int i = 0; i < numPlayers; i++)
//	{
//		string name;
//		int age;
//		cout << "New player, enter your name: ";
//		cin >> name;
//		cout << "Enter your age: ";
//		cin >> age;
//		Player* player = new Player(name, age, numCoinsPerPlayer, colors[i], deck);
//		players->push_back(player);
//		player->printPlayer();
//	}
//}
//
//void createHuman(const int numPlayers, const int numCoinsPerPlayer, vector<Player*>* players, Deck* deck, const string colors[])
//{
//	for (int i = 0; i < numPlayers; i++)
//	{
//		string name;
//		int age;
//		cout << "New player, enter your name: ";
//		cin >> name;
//		cout << "Enter your age: ";
//		cin >> age;
//		Player* player = new Player(name, age, numCoinsPerPlayer, colors[i], deck);
//		player->set_strategy(new Human());
//		players->push_back(player);
//		player->printPlayer();
//	}
//}
//
//void createModerate(const int numPlayers, const int numCoinsPerPlayer, vector<Player*>* players, Deck* deck, const string colors[])
//{
//	for (int i = 0; i < numPlayers; i++)
//	{
//		string name;
//		int age;	
//		cout << "New moderate computer, enter name: ";
//		cin >> name;
//		cout << "Enter your age: ";
//		cin >> age;
//		Player* player = new Player(name, age, numCoinsPerPlayer, colors[i], deck);
//		player->set_strategy(new Moderate());
//		players->push_back(player);
//		player->printPlayer();
//	}
//}
//
//void createGreedy(const int numPlayers, const int numCoinsPerPlayer, vector<Player*>* players, Deck* deck, const string colors[])
//{
//	for (int i = 0; i < numPlayers; i++)
//	{
//		string name;
//		int age;
//		cout << "New greedy computer, enter name: ";
//		cin >> name;
//		cout << "Enter your age: ";
//		cin >> age;
//		Player* player = new Player(name, age, numCoinsPerPlayer, colors[i], deck);
//		player->set_strategy(new Greedy());
//		players->push_back(player);
//		player->printPlayer();
//	}
//}
//
////Placing initial player's armies
//void placeInitialPlayerArmies(vector<Player*>* players, Country* startingCountry)
//{
//	cout << "Each player must now place 3 of their army cubes on the starting country." << endl;
//	for (int i = 0; i < players->size(); i++)
//	{
//		players->at(i)->placeNewArmies(startingCountry, 3);
//	}
//	cout << *(startingCountry->name) << ", the starting country, holds the following:" << endl;
//	for (int i = 0; i < startingCountry->occupyingArmies->size(); i++)
//	{
//		cout << "A " << *(startingCountry->occupyingArmies->at(i)->player->color) << " army cube belonging to ";
//		cout << *(startingCountry->occupyingArmies->at(i)->player->name) << endl;
//	}
//	cout << endl;
//}
//
////First Player bids
//void bidFirstPlayer(vector<Player*>* players, const int numCoinsPerPlayer, int* supply)
//{
//	cout << "Beginning the bid for first player." << endl;
//	for (int i = 0; i < players->size(); i++)
//	{
//		int bidAmount;
//		while (true)
//		{
//			cout << *(players->at(i)->name) << ", please enter a bid amount between 0 and " << numCoinsPerPlayer << ": ";
//			cin >> bidAmount;
//			if (bidAmount < 0 || bidAmount > numCoinsPerPlayer)
//			{
//				cout << "You've entered a number outside of the correct range. Try again." << endl;
//				cin.clear();
//				cin.ignore(numeric_limits<streamsize>::max(), '\n'); // See [1]
//			}
//			else
//			{
//				players->at(i)->bidFacObj->bidCoins(bidAmount);
//				break;
//			}
//		}
//	}
//	players->at(0)->bidFacObj->startBidProcess(supply);
//}
//
//int main()
//{
//	// ######################################################################
//	//				Part 1: Game Start
//	// ######################################################################
//
//	cout << "########################################" << endl;
//	cout << "\tEight Minute Empire" << endl;
//	cout << "########################################\n" << endl;
//
//	// Select map from list of files.
//	MapLoader* mapLoader = nullptr;
//	while (true)
//	{
//		string mapFileName = getMapFileName();
//		try
//		{
//			mapLoader = new MapLoader(mapFileName, Map::getInstance());
//			break;
//		}
//		catch (const MapLoaderException& e)
//		{
//			cout << e.message << endl;
//			cout << "Try again." << endl;
//			cout << endl;
//			delete mapLoader;
//		}
//	}
//	cout << "You've selected the following map: " << endl;
//	Map::getInstance()->print();
//	cout << endl;
//	Country* startingCountry = Map::getInstance()->startingCountry; //Starting country is loaded from map
//
//	// Create deck with 42 cards. Shuffle method is done when creating a deck
//	global::main_deck = new Deck();
//	cout << "All " << global::main_deck->stackofCards->size() << " cards are shuffled and then putted into a deck and assigned to the game.\n" << endl;
//
//	// Select number of players.
//	const int NUM_PLAYERS = 3;
//	const int NUM_COINS_PER_PLAYER = getNumCoinsPerPlayer(NUM_PLAYERS);
//
//	// Create correct number of players.
//	global::players = new vector<Player*>();
//	const string COLORS[5] = { "Red", "Blue", "Green", "Yellow", "White" };
//	
//	createHuman(1, NUM_COINS_PER_PLAYER, global::players, global::main_deck, COLORS);
//	createModerate(1, NUM_COINS_PER_PLAYER, global::players, global::main_deck, COLORS);
//	createGreedy(1, NUM_COINS_PER_PLAYER, global::players, global::main_deck, COLORS);
//	
//	//Other details for setting up the game
//	bool endGame = false;
//	const int MAX_CARDS = determineMaxCards(NUM_PLAYERS);
//	global::action = new Actions();
//
//	// #######################################################################
//	//				Part 2: Startup Phase
//	// #######################################################################
//
//		// Create the supply of coins (value after each player has taken the appropriate amount of coins for themselves).
//	global::supply = new int(44 - (NUM_COINS_PER_PLAYER * NUM_PLAYERS));
//	cout << "Total supply of coins was 44, but since each player took " << NUM_COINS_PER_PLAYER << " coins, supply is now at " << global::supply << " coins.\n" << endl;
//
//	// Place 3 armies of each player on the starting country.
//	placeInitialPlayerArmies(global::players, startingCountry);
//
//	//draw and fill the cards space with faceups cards
//	global::main_deck->initialDraw();
//
//	// Bidding process
//	Player* currentPlayer;
//	int* currentPlayerIndex = new int(0);
//
//	bidFirstPlayer(global::players, NUM_COINS_PER_PLAYER, global::supply);
//	currentPlayer = global::players->at(0)->bidFacObj->winner;
//	auto it = find(global::players->begin(), global::players->end(), currentPlayer);
//	if (it != global::players->end())
//	{
//		*currentPlayerIndex = static_cast<int>(distance(global::players->begin(), it));
//	}
//	global::players->at(0)->bidFacObj->clearBidingFacility();
//
//	//Keep track of the last player of each round
//	Player* lastPlayer;
//	int lastIndex = 2;
//	lastPlayer = global::players->at(lastIndex);
//
//	//Attach PlayerOb on each player with their index
//	PlayerObserver* observeP;
//	int* track = new int(*currentPlayerIndex);
//	int* turn = new int(1);
//
//	do
//	{
//		observeP = new PlayerObserver(global::players->at(*track), turn);
//		track = new int((*track + 1 + NUM_PLAYERS) % NUM_PLAYERS);
//		turn = new int(*turn + 1);
//		observeP = NULL;
//
//	} while (*track != *currentPlayerIndex);
//
//	delete track;
//	track = NULL;
//
//	cout << "Let the game begin!\n" << endl;
//	currentPlayer->Notify();
//
//	// #################################################
//	//				Part 3: Main Game Loop
//	// #################################################
//
//	int newIndex;
//	do
//	{
//		currentPlayer->execute_strategy();
//		
//		cout << *(currentPlayer->name) << " now has "
//			<< currentPlayer->hand->faceupcards->size()
//			<< " cards" << endl;
//
//		// #################################################
//		//				Part 5: After Action
//		// #################################################
//
//
//
//		//Loop to the next player
//		newIndex = *currentPlayerIndex + 1;//Increasing the index position is clockwise 
//		*currentPlayerIndex = newIndex % NUM_PLAYERS;
//		currentPlayer = global::players->at(*currentPlayerIndex);//update current player
//
//		cout << endl;
//		cout << "************************************************************" << endl;
//		cout << endl;
//		currentPlayer->Notify();
//	
//		// #################################################
//		//			Part 6: Game End, Compute Score
//		// #################################################
//
//		if (lastPlayer->hand->faceupcards->size() >= MAX_CARDS)
//		{
//			cout << "Every Players has ";
//			cout << currentPlayer->hand->faceupcards->size() << " cards. " << endl;
//			cout << "Max cards each player can own has been reach. " << endl;
//			endGame = true;
//		}
//
//	} while (!endGame);
//	cout << endl;
//	cout << "****************END OF GAME****************" << endl;
//
//	//Compute Scores
//	computeScore score = computeScore();
//	Player* winner = score.determineWinner(global::players);
//
//	cout << "The winner is " << *winner->name << endl;//Print the winner of the game
//	
//	for (int i = 0; i < global::players->size(); i++)//Prints all the players
//	{
//		global::players->at(i)->printPlayer();
//	}
//
//	system("PAUSE");
//	// #################################################
//	//						Cleanup
//	// #################################################
//
//	for (int i = 0; i < NUM_PLAYERS; i++)
//	{
//		delete global::players->at(i);
//	}
//	delete global::players;
//	global::players = NULL;
//	delete global::main_deck;
//	global::main_deck = NULL;
//	delete mapLoader;
//	mapLoader = NULL;
//	delete Map::getInstance();
//	startingCountry = NULL;
//	global::supply = NULL;
//	currentPlayer = NULL;
//	delete currentPlayerIndex;
//	currentPlayerIndex = NULL;
//	lastPlayer = NULL;
//	delete global::action;
//	global::action = NULL;
//	winner = NULL;
//	delete observeP;
//	observeP = NULL;
//	delete turn;
//	turn = NULL;
//}
//
////Run program: Ctrl + F5 or Debug > Start Without Debugging menu
////Debug program: F5 or Debug > Start Debugging menu
//
////Tips for Getting Started: 
////  1. Use the Solution Explorer window to add/manage files
////  2. Use the Team Explorer window to connect to source control
////  3. Use the Output window to see build output and other messages
////  4. Use the Error List window to view errors
////  5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
////  6. In the future, to open this project again, go to File > Open > Project and select the .sln file
//
//// References
//// [1] https://www.hackerearth.com/practice/notes/validating-user-input-in-c/
