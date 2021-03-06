#pragma once
#include <string>
#include <vector>
#include "cards.h"
#include "Player.h"
#include "Map.h"
#include "MapLoader.h"
#include "Actions.h"
#include "computeScore.h"
#include "GameStrategy.h"


using namespace std;

class StrategyG;

class GameEngine 
{
	enum class Mode { single, tournament };
private:
	Mode* mode;
	Map* map;
	Deck* deck;
	int* supply;
	vector<Player*>* players;
	Country* startingCountry;
	int* NUM_PLAYERS;
	int* NUM_COINS_PER_PLAYER;
	StrategyG* strategy;//strategy is the mode of the game

public:
	GameEngine();
	GameEngine(string);
	~GameEngine();
	void setMode(string);
	int getMode();
	void setStartingCountry(Country*);
	Country* getStartingCountry();
	void chooseMap();
	void choosePlayers();
	void setNumOfPlayers();
	int* getNumOfPlayers();
	Deck* getDeck();
	void setSupply();
	int* getSupply();
	void setNumCoinsPerPlayer(const int);
	int* getNumCoinsPerPlayer();
	void createPlayers();
	void set_strategy(StrategyG* strat);
	void execute_strategy();
	StrategyG* get_strategy();
};