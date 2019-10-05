#pragma once

#include <vector> 
#include <list>
#include <string>
#include <iostream>
#include "Map.h"
#include "BidingFacility.h"

using namespace std;

class Army;
class City;
class Player;
class BidingFacility;

//class Army
//{
//public:
//	Army(Player*, int);
//	~Army();
//
//	int* id;
//	Player* player;
//	Country* occupiedCountry;
//
//	void setOccupiedCountry(Country*);
//};
//
//class City
//{
//public:
//	City(Player*, int);
//	~City();
//
//	int* id;
//	Player* player;
//	Country* occupiedCountry;
//
//	void setOccupiedCountry(Country*);
//};

class Player
{
public:
	Player(string, int, int);
	//Player(string, int, int, string);
	~Player();

	string* name;
	int* age;
	int* numCoins;
	BidingFacility* bidFacObj;

	//string* color;
	//vector<Army*>* armies;
	//vector<City*>* cities;
	//vector<Country*>* ownedCountries;	// Part 1
	// vector<Card*>* hand;				// Part 4
	// vector<Good*>* goods				// Part 4
	//BidingFacility* bidingFacility;	// Part 5

	//void createArmies();
	//void createCities();

	//void payCoin(int);
	//void placeNewArmies();
	//void moveArmies();
	//void moveOverLand();
	//void buildCity(Country*);
	//void destroyArmy(Country*);

	operator std::string() const { return "Player " + *name; }

};