#include "Player.h"
#include "cards.h"
#include "Map.h"
#include "BidingFacility.h"
#include "global.h"

struct InsufficientCoinsException : public exception
{
	const char* what() const throw()
	{
		return "Player does not have enough coins.";
	}
};

Army::Army()
	: player(nullptr), occupiedCountry(nullptr) {}

Army::Army(Player* newPlayer)
	: player(newPlayer), occupiedCountry(nullptr) {}

Army::~Army()
{
	player = NULL;
	occupiedCountry = NULL;
}

void Army::setOccupiedCountry(Country* country)
{
	Army::occupiedCountry = country;
}

City::City()
	: player(nullptr), occupiedCountry(nullptr) {}

City::City(Player* newPlayer)
	: player(newPlayer), occupiedCountry(nullptr) {}

City::~City()
{
	player = NULL;
	occupiedCountry = NULL;
}

void City::setOccupiedCountry(Country* country)
{
	City::occupiedCountry = country;
}

Player::Player()
{
	name = new string("");
	age = new int(0);
	numCoins = new int(0);
	color = new string("");
	armies = new vector<Army*>();
	cities = new vector<City*>();
	ownedCountries = new vector<Country*>();
	ownedContinents = new vector<Continent*>();
	hand = new Hand();
	bidFacObj = new BidingFacility(this);
	victoryPoint = new int(0);
	scores = new vector <Player*>();
	strategy = new Human();
}

Player::Player(string inputName, int inputAge, int coinAmount, string selectedColor, Deck* mainDeck)
{
	name = new string(inputName);
	age = new int(inputAge);
	numCoins = new int(coinAmount);
	color = new string(selectedColor);
	armies = new vector<Army*>();
	cities = new vector<City*>();
	ownedCountries = new vector<Country*>();
	ownedContinents = new vector<Continent*>();
	hand = new Hand(mainDeck);
	bidFacObj = new BidingFacility(this);
	victoryPoint = new int(0);
	scores = new vector <Player*>();

	this->createArmies();
	this->createCities();
}

Player::~Player()
{
	for (int i = 0; i < armies->size(); i++)
	{
		delete armies->at(i);
		armies->at(i) = NULL;
	}
	for (int i = 0; i < cities->size(); i++)
	{
		delete cities->at(i);
		cities->at(i) = NULL;
	}
	delete name, age, numCoins, color, armies, cities, ownedCountries, hand, bidFacObj;
	name = NULL;
	age = NULL;
	numCoins = NULL;
	color = NULL;
	armies = NULL;
	cities = NULL;
	ownedCountries = NULL;
	hand = NULL;
	bidFacObj = NULL;
	victoryPoint = NULL;
	scores = NULL;
}

int Player::availableCities()
{
	int numCities = 0;
	for (int i = 0; i < cities->size(); i++)
	{
		if (cities->at(i)->occupiedCountry == nullptr)
		{
			numCities++;
		}
	}
	return numCities;
}

City* Player::getAvailableCity()
{
	for (int j = 0; j < cities->size(); j++)
	{
		if (cities->at(j)->occupiedCountry == nullptr)
		{
			return cities->at(j);
		}
	}
	return nullptr;
}

int Player::availableArmies()
{
	int numArmies = 0;
	for (int i = 0; i < armies->size(); i++)
	{
		if (armies->at(i)->occupiedCountry == nullptr)
		{
			numArmies++;
		}
	}
	return numArmies;
}

Army* Player::getAvailableArmy()
{
	for (int j = 0; j < armies->size(); j++)
	{
		if (armies->at(j)->occupiedCountry == nullptr)
		{
			return armies->at(j);
		}
	}
	return nullptr;
}

void Player::createArmies()
{
	for (int i = 0; i < 14; i++) {
		Army* army = new Army(this);
		this->armies->push_back(army);
	}
}

void Player::createCities()
{
	for (int i = 0; i < 3; i++) {
		City* city = new City(this);
		this->cities->push_back(city);
	}
}

void Player::printPlayer()
{
	cout << "Player: " << *(this->name) << ", age " << *(this->age) << ", color " << *(this->color) << endl;
	cout << "\t" << *(this->name) << " holds " << *(this->numCoins) << " coins." << endl;
	cout << "\t" << *(this->name) << " has " << this->armies->size() << " " << *(this->color) << " army cubes and " << this->cities->size() << " " << *(this->color) << " city discs." << endl;
	for (int i = 0; i < armies->size(); i++)
	{
		if (armies->at(i)->occupiedCountry == nullptr)
		{
			cout << "\t\t\t" << "Army " << i+1 << " has not been placed." << endl;
		} 
		else
		{
			cout << "\t\t\t" << "Army " << i+1 << " is in " << *armies->at(i)->occupiedCountry->name << endl;
		}
	}
	cout << "\t\t" << this->cities->size() << " cities (discs)." << endl;
	for (int i = 0; i < cities->size(); i++)
	{
		if (cities->at(i)->occupiedCountry == nullptr)
		{
			cout << "\t\t\t" << "City " << i + 1 << " has not been placed." << endl;
		}
		else
		{
			cout << "\t\t\t" << "City " << i + 1 << " is in " << *cities->at(i)->occupiedCountry->name << endl;
		}

	}
	cout << "\t" << *(this->name) << " owns  " << this->ownedCountries->size() << " countries, and has " << *this->hand->goods << " goods collected." << endl;
	for (int i = 0; i < ownedCountries->size(); i++)
	{
		cout << "\t\t" << *ownedCountries->at(i)->name << endl;

	}

	if (ownedContinents->size() > 0)
	{
		cout << "\t" << *(this->name) << " owns the continents: " << this->ownedContinents->size() << endl;
		for (int i = 0; i < ownedContinents->size(); i++)
		{
			cout << "\t\t" << *ownedContinents->at(i)->name << endl;

		}
	}

	cout << "\t" << *(this->name) << " has " << this->hand->faceupcards->size() << " cards in hand." << endl;
	cout << "\t" << *(this->name) << " has their own bidding facility.\n" << endl;
	cout << "\t" << *name << " has " << *victoryPoint << " points." << endl;
}

void Player::buildCity(Country* country)
{
	City* availableCity = this->getAvailableCity();
	availableCity->occupiedCountry = country;
	country->addCity(availableCity);
}

void Player::destroyArmy(Country* country, Player* otherPlayer)
{
	Army* selectedArmy = country->getArmy(otherPlayer);
	selectedArmy->occupiedCountry = nullptr;
	country->removeArmy(selectedArmy);
}

void Player::moveArmies(Country* initCountry, Country* finalCountry)
{
	Army* selectedArmy = initCountry->getArmy(this);
	selectedArmy->occupiedCountry = finalCountry;
	initCountry->moveArmy(finalCountry, selectedArmy);
}

void Player::moveOverLand(Country* initCountry, Country* finalCountry)
{
	Army* selectedArmy = initCountry->getArmy(this);
	selectedArmy->occupiedCountry = finalCountry;
	initCountry->moveArmy(finalCountry, selectedArmy);
}

void Player::payCoin(int amount, int* supply)
{
	try
	{
		if ((*(this->numCoins) - amount) < 0)
			throw InsufficientCoinsException();
		else
		{
			*(this->numCoins) = *(this->numCoins) - amount;
			*supply = *supply + amount;
			computeScore();
		}
	}
	catch (InsufficientCoinsException e)
	{
		cout << e.what();
		return ;
	}

	Notify();

}

void Player::placeNewArmies(Country* country, int amount)
{
	for (amount; amount > 0; amount--)
	{
		Army* availableArmy = this->getAvailableArmy();
		availableArmy->occupiedCountry = country;
		country->addArmy(availableArmy);
	}
}

//Ignore action only prints out that they ignore it
void Player::ignore(Cards* card)
{
	card->Notify();
}

CurrentPOb* obCard;

//Depending on the position of the cards, the amount to pay is different
void Player::payCard(Cards* c, int* supply)
{
	int position = 0;

	for (auto i : *global::main_deck->cardsSpace->faceupcards)
	{
		if (i == c)
		{
			break;
		}
		position++;
	}

	obCard = new CurrentPOb(this, c, &position, supply);
	
	switch (position)
	{
	case 0:
		obCard->setCost(new int(0));
		this->payCoin(0,supply);
		delete obCard;
		obCard = NULL;
		break;
	case 1:
	case 2:
		obCard->setCost(new int(1));
		this->payCoin(1,supply);
		delete obCard;
		obCard = NULL;
		break;
	case 3:
	case 4:
		obCard->setCost(new int(2));
		this->payCoin(2,supply);
		delete obCard;
		obCard = NULL;
		break;
	case 5:
		obCard->setCost(new int(3));
		this->payCoin(3,supply);
		delete obCard;
		obCard = NULL;
		break;
	default:
		cout << "Invalid input" << endl;
		break;
	}
	
}

/*
1 country = 1 point
more armies than other player in a country, 
cities counted as armies, if the numer is the same, no point for everyone
*/

int getCoalPoint(Hand* hand)
{
	int coal[6] =
	{
		0, 0, 1, 2, 3, 5
	};

	int temp = 0;
	for (int i = 0; i < hand->faceupcards->size(); i++)
	{
		if (*(hand->faceupcards->at(i)->good) == "coal")
		{
			temp++;
		}

	}
	*hand->goods = *hand->goods + coal[temp];
	return coal[temp];
}

int getAnvilPoint(Hand* hand)
{
	int anvil[8] =
	{
		0, 0, 1, 1, 2, 2, 3, 5
	};

	int temp = 0;
	for (int i = 0; i < hand->faceupcards->size(); i++)
	{
		if (*(hand->faceupcards->at(i)->good) == "2anvil")
		{
			temp = temp + 2;
			continue;

		}
		if(*(hand->faceupcards->at(i)->good) == "anvil")
		{
			temp++;
		}
	}
	*hand->goods = *hand->goods + anvil[temp];
	return anvil[temp];
}

int getTreePoint(Hand* hand)
{

	int tree[7] =
	{
		0, 0, 1, 1, 2, 3, 5
	};

	int temp = 0;
	for (int i = 0; i < hand->faceupcards->size(); i++)
	{
		if (*(hand->faceupcards->at(i)->good) == "tree")
		{
			temp++;
		}
	}
	*hand->goods = *hand->goods + tree[temp];
	return tree[temp];
}

int getGemPoint(Hand* hand)
{
	int gem[5] =
	{
		0, 1, 2, 3, 5
	};

	int temp = 0;
	for (int i = 0; i < hand->faceupcards->size(); i++)
	{
		if (*(hand->faceupcards->at(i)->good) == "gem")
		{
			temp++;
		}
	
	}
	*hand->goods = *hand->goods + gem[temp];
	return gem[temp];
}

int getCarrotPoint(Hand* hand)
{
	int carrot[9] =
	{
		0, 0, 0, 1, 1, 2, 2, 3, 5
	};

	int temp = 0;
	for (int i = 0; i < hand->faceupcards->size(); i++)
	{
		if (*(hand->faceupcards->at(i)->good) == "2carrot")
		{
			temp= temp +2;
			continue;
		}
		if (*(hand->faceupcards->at(i)->good) == "carrot")
		{
			temp++;
		}
		
	}
	*hand->goods = *hand->goods + carrot[temp];
	return carrot[temp];
}

int goodPoints(Hand* hand)
{
	*hand->goods = 0;
	int newGoodPoints = getCoalPoint(hand) + getAnvilPoint(hand) + getTreePoint(hand) + getGemPoint(hand) + getCarrotPoint(hand);
	return newGoodPoints;
}

void Player::computeScore()
{	
	int points = 0;
	points = ownedCountries->size();
	points += ownedContinents->size();
	points += goodPoints(hand);
	*victoryPoint = points;
}

void Player::addOwnedCountry(Country* to_add)
{
	ownedCountries->push_back(to_add);
	computeScore();
}

void Player::addOwnedContinent(Continent* to_add)
{
	ownedContinents->push_back(to_add);
	computeScore();
}

bool Player::removeOwnedCountry(Country* to_remove)
{
	for(int i = 0; i < ownedCountries->size(); i++)
	{
		if (ownedCountries->at(i) == to_remove)
		{
			ownedCountries->erase(ownedCountries->begin() + i);
			computeScore();
			return true;
		}
	}
	return false;
}

bool Player::removeOwnedContinent(Continent* to_remove)
{
	for (int i = 0; i < ownedContinents->size(); i++)
	{
		if (ownedContinents->at(i) == to_remove)
		{
			ownedContinents->erase(ownedContinents->begin() + i);
			computeScore();
			return true;
		}
	}
	return false;
}

void Player::set_strategy(Strategy* strat)
{
	delete strategy;
	strategy = strat;
}

void Player::execute_strategy()
{
	strategy->execute(*this);
}

Strategy* Player::get_strategy()
{
	return strategy;
}



// References
// [1] https://thispointer.com/c-how-to-find-an-element-in-vector-and-get-its-index/
