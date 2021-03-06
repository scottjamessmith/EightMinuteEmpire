#include "Actions.h"
#include <string>
#include "global.h"
#include <iostream>
#include "GameObservers.h"

using namespace std;

ActionOb* actOb;

void actionProcess(const string&, const int&, Player* player, Map* map, vector<Player*>*);
void actionPrint(const string&, const int&);
void computer_process(const string&, const int&, Player&);

void Actions::processAction(Player* player, Cards *card, Map *map, vector<Player*>* playerVector) const
{

	/*

	format of actions:

	string = "action" + # + ("AND" or "OR") + "action2" + #
	- # and "AND/OR" are optional, but if there is "AND/OR" there must be an "action2"
	- at least one action is required

	*/

	//process action(s)
	//Attach ActionOb

	actOb = new ActionOb(player, card);

	std::cout << endl;
	
	//if there is more than one action
	if (card->actions->size() > 2)
	{
		
		int selection = 0;

		//if it is a 'and' card, then choose between both actions or ignoring
		if ("AND" == *card->actions->at(2))
		{
			std::cout << "\tSelect one: " << endl;
			std::cout << "\t\t1: ";  actionPrint(*card->actions->at(0), stoi(*card->actions->at(1)));
			std::cout << " and ";  actionPrint(*card->actions->at(3), stoi(*card->actions->at(4))); std::cout << endl;
			std::cout << "\t\tor" << endl;
			std::cout << "\t\t2: take no action " << endl;

			//keep asking till valid input
			while (selection != 1 && selection != 2)
			{
				std::cout << "\tPlease enter a valid choice: ";
				cin >> selection;
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}

			//do both actions
			if (selection == 1)
			{
				
				actOb->setAction(card->actions->at(0));
				int amount;
				amount = stoi(*card->actions->at(1));
				actOb->setAmount(&amount);
				actOb->setAction(card->actions->at(3));
				int amount2;
				amount2 = stoi(*card->actions->at(4));
				actOb->setAmount(&amount2);
				card->Notify();
				delete actOb;
				actOb = NULL;
				actionProcess(*card->actions->at(0), stoi(*card->actions->at(1)), player, map, playerVector);
				actionProcess(*card->actions->at(3), stoi(*card->actions->at(4)), player, map, playerVector);

			} 
			//or ignore
			else if (selection == 2)
			{
				string* answer = new string("Ignore");
				actOb->setAction(answer);
				player->ignore(card);
				delete actOb;
				actOb = NULL;
				return;
			}
		}

		// if it is an 'or' card
		// same as last block
		if ( "OR" == *card->actions->at(2))
		{
			std::cout << "\tSelect one: " << endl;
			std::cout << "\t\t1: ";  actionPrint(*card->actions->at(0), stoi(*card->actions->at(1))); std::cout << endl;
			std::cout << "\t\tor" << endl;
			std::cout << "\t\t2: ";  actionPrint(*card->actions->at(3), stoi(*card->actions->at(4))); std::cout << endl;
			std::cout << "\t\tor" << endl;
			std::cout << "\t\t3: take no action " << endl;

			while (selection != 1 && selection != 2 && selection != 3)
			{
				std::cout << "\tPlease enter a valid choice: ";
				cin >> selection;
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}

			if (selection == 1)
			{
				actOb->setAction(card->actions->at(0));
				int amount;
				amount = stoi(*card->actions->at(1));
				actOb->setAmount(&amount);
				card->Notify();
				delete actOb;
				actOb = NULL;
				actionProcess(*card->actions->at(0), stoi(*card->actions->at(1)), player, map, playerVector);

			}
			else if (selection == 2)
			{
				actOb->setAction(card->actions->at(3));
				int amount;
				amount = stoi(*card->actions->at(4));
				actOb->setAmount(&amount);
				card->Notify();
				delete actOb;
				actOb = NULL;
				actionProcess(*card->actions->at(3), stoi(*card->actions->at(4)), player, map, playerVector);
			}
			else if (selection == 3)
			{
				string* answer= new string("Ignore");
				actOb->setAction(answer);
				answer = nullptr;
				player->ignore(card);
				delete actOb;
				actOb = NULL;
				return;
			}

		}

	}
	else //if only one action, then choose between action and ignore
	{
		int selection = 0;

		std::cout << "\tSelect one: " << endl;
		std::cout << "\t\t1: ";  actionPrint(*card->actions->at(0), stoi(*card->actions->at(1))); std::cout << endl;
		std::cout << "\t\tor" << endl;
		std::cout << "\t\t2: take no action " << endl;

		while (selection != 1 && selection != 2)
		{
			std::cout << "\tPlease enter a valid choice: ";
			cin >> selection;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

		if (selection == 1)
		{
			actOb->setAction(card->actions->at(0));
			int amount;
			amount = stoi(*card->actions->at(1));
			actOb->setAmount(&amount);
			card->Notify();
			delete actOb;
			actOb = NULL;
			actionProcess(*card->actions->at(0), stoi(*card->actions->at(1)), player, map, playerVector);

		}
		else if (selection == 2)
		{
			string* answer = new string("Ignore");
			actOb->setAction(answer);
			answer = nullptr;
			player->ignore(card);
			delete actOb;
			actOb = NULL;
			return;
		}
		
	}
	std::cout << endl;
}


void actionProcess(const string& action, const int& amount, Player *player, Map* map, vector<Player*>* playersVector)
{
	ProcessActOb* proOb;


	//if it is a place armies card
	if ("placeArmies" == action)
	{
		string countryName;
		for (int i = 0; i < amount; i++) //for every army to be placed
		{
			if (player->availableArmies() > 0) //if the player has any available armies
			{
				Army* newArmy = player->getAvailableArmy(); //get an army from the player
				
				Country* country = nullptr;
				bool isValid = false;

				while (country == nullptr || !isValid) //until is valid country name
				{
					std::cout << "\t" << i + 1 << ": Please give a valid country\'s name for the army to be placed in (-1 to exit): ";
					std::cin >> countryName;
					if (countryName == "-1")
					{
						return;
					}
					//std::cout << *country->name << endl;
					country = map->getCountry(countryName); //get pointer to specified country

					if (country != nullptr) //if the getCountry method doesnt return nullptr
					{
						//if the country is the starting country
						if (*country->isStartingCountry)
						{
							isValid = true;
						}
						else
						{
							//if the player has a city in that country
							for (auto j : *country->cities)
							{
								if (j->player == player)
								{
									isValid = true;
									break;
								}
							}
							if (!isValid)
								std::cout << "\t" << *country->name << " is either not the starting country or the player does not own a city there. " << endl;
						}
					}
				}
				proOb = new ProcessActOb(global::action, player,country, 1);
				global::action->Notify();
				player->placeNewArmies(country, 1);
				delete proOb;
				proOb = nullptr;
			}
			else
			{
				std::cout << "\tYou have no more armies to place. " << endl;
				break;
			}
		}
		return;
	}
	if ("createCity" == action) //virtually the same as the last block but for city placement
	{
		string countryName;
		for (int i = 0; i < amount; i++) //for all place city count (i think it's only ever 1)
		{
			if (player->availableCities() > 0)
			{
				City* newCity = player->getAvailableCity();

				Country* country = nullptr;

				bool isValid = false;
				while (country == nullptr || !isValid)
				{
					std::cout << "\tPlease give a valid country\'s name for the city to be placed in (-1 to exit): ";
					cin >> countryName;
					if (countryName == "-1")
					{
						return;
					}
					//std::cout << *country->name << endl;
					country = map->getCountry(countryName);

					if (country != nullptr)
					{
						//if the player has amn army in that country
						for (auto k : *country->occupyingArmies)
						{
							if (k->player == player)
							{
								isValid = true;
								break;
							}
						}
						if(!isValid)
							std::cout << "\t" <<*player->name << " does not have an army in " << *country->name << endl;
					}
				}
				proOb = new ProcessActOb(global::action, player, country);
				global::action->Notify();
				player->buildCity(country);
				delete proOb;
				proOb = nullptr;
			}
			else
			{
				std::cout << "\tYou have no more cities to place. " << endl;
				break;
			}
		}
		return;
	}
	if ("move" == action || "waterMove" == action)
	{

		vector<int> armiesAlreadyMoved; //vector for the armies that have already been moved this turn

		for (int j = 0; j < amount; j++)
		{

			if (player->availableArmies() == 14) // check if any armies have been placed
			{
				std::cout << "\tNo armies have been placed." << endl;
				return;
			}

			//if the card's amount of armies to move is greater than the player's placed armies
			if (armiesAlreadyMoved.size() >= 14 - player->availableArmies())
			{
				return;
			}

			Army* army = nullptr;
			int armyID;


			player->printPlayer();

			bool newArmy = false;
			while (army == nullptr || newArmy == false)
			{
				std::cout << "\tPlease give a valid placed army number (-1 to exit): ";
				cin >> armyID;
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

				if (armyID == -1)
				{
					return;
				}

				armyID--;

				newArmy = true;
				for (auto i : armiesAlreadyMoved)
				{
					if (armyID+1 == i)
						newArmy = false;
				}

				if (armyID < 14 && armyID > -1)
				{
					if (player->armies->at(armyID)->occupiedCountry != nullptr)
						army = player->armies->at(armyID);
				}
			}


			if ("waterMove" == action)
			{
				if (player->armies->at(armyID)->occupiedCountry != nullptr)
				{
					Country* country = nullptr;
					bool isValid = false;
					bool goBack = false;
					while (!isValid)
					{
						string countryName;
						std::cout << "\tPlease give a valid name for a country to move army " << armyID + 1 << " across land or water to; ";
						std::cout << "the possible adjacent countries are (-1 to change army selection): " << endl;
						for (auto k : *player->armies->at(armyID)->occupiedCountry->adjCountries)
						{
							std::cout << "\t" << *k->name << endl;
						}
						cin >> countryName;
						if (countryName == "-1")
						{
							goBack = true;
							break;
						}
						country = map->getCountry(countryName);
						if (country != nullptr)
						{
							for (auto k : *country->adjCountries)
							{
								//if the army's country is adjacent to the selected country
								if (*army->occupiedCountry->name == *k->name)
									isValid = true;
							}
						}
					}

					if (goBack)
					{
						j--;
						continue;
					}
					armiesAlreadyMoved.push_back(armyID + 1);
					proOb = new ProcessActOb(global::action, player, army->occupiedCountry, country);
					global::action->Notify();
					player->moveArmies(army->occupiedCountry, country);
					delete proOb;
					proOb = nullptr;
				}
			}
			else
			{
				if (player->armies->at(armyID)->occupiedCountry != nullptr)
				{
					Country* country = nullptr;
					bool isValid = false;
					bool isIsland = false;
					bool goBack = false;
					while (!isValid)
					{
						string countryName;
						int countryCheck = 0;
						for (auto k : *player->armies->at(armyID)->occupiedCountry->adjCountries)
						{
							//if the army's country is adjacent to the selected country and if they are in the same continent
							if (army->occupiedCountry->parentContinent == k->parentContinent)
								countryCheck++;
						}
						if (countryCheck == 0)
						{
							goBack = true;
							break;
						}

						std::cout << "\tPlease give a valid name for a country in " << *army->occupiedCountry->parentContinent->name << " to move army " << armyID + 1 << " to; ";
						std::cout << "the possible adjacent countries are (-1 to change army selection): " << endl;
						for (auto k : *player->armies->at(armyID)->occupiedCountry->adjCountries)
						{
							//if the army's country is adjacent to the selected country and if they are in the same continent
							if (army->occupiedCountry->parentContinent == k->parentContinent)
								std::cout << "\t" << *k->name << endl;
						}
						cin >> countryName;
						if (countryName == "-1")
						{
							isIsland = true;
							break;
						}
						country = map->getCountry(countryName);
						if (country != nullptr)
						{
							for (auto k : *country->adjCountries)
							{
								//if the army's country is adjacent to the selected country and if they are in the same continent
								if (army->occupiedCountry == k && army->occupiedCountry->parentContinent == country->parentContinent)
									isValid = true;
							}
						}
					}
					if (isIsland)
					{
						std::cout << "\tInvalid: this country is an island" << endl;
						j--;
						continue;
					}
					if (goBack)
					{
						j--;
						continue;
					}
					proOb = new ProcessActOb(global::action, player, army->occupiedCountry, country);
					armiesAlreadyMoved.push_back(armyID + 1);
					global::action->Notify();
					player->moveOverLand(army->occupiedCountry, country);
					delete proOb;
					proOb = nullptr;
					
				}
			}
		}
	}
	if ("destroyArmies" == action)
	{
		Country* country = nullptr;
		Player* enemyPlayer = nullptr;
		string enemyName;

		while (enemyPlayer == nullptr)
		{
			std::cout << "\tGive a valid player name (-1 to exit): ";
			cin >> enemyName;
			if (enemyName == "-1") 
			{
				return;
			}
			for (auto i : *playersVector)
			{
				if (enemyName == *i->name)
					enemyPlayer = i;
			}
		}

		string countryName;
		while (country == nullptr)
		{
			std::cout << "\tGive a valid country name (-1 to exit): ";
			cin >> countryName;
			if (countryName == "-1")
			{
				return;
			}
			country = map->getCountry(countryName);
			if (country != nullptr)
			{
				for (auto i : *country->occupyingArmies)
				{
					if (enemyName == *i->player->name)
					{
						proOb = new ProcessActOb(global::action, player, country, enemyPlayer );
						country = i->occupiedCountry;
						global::action->Notify();
						player->destroyArmy(country, enemyPlayer);
						delete proOb;
						proOb = nullptr;
						return;
					}
				}
				std::cout << "\t" << enemyName << " does not have an army in " << countryName << endl;
				country = nullptr;
			}
		}
	}

}


void actionPrint(const string& action, const int& amount)
{
	if ("placeArmies" == action)
	{
		if (amount > 1)
		{
			std::cout << "Place " << amount << " armies";
			return;
		}
		std::cout << "Place " << amount << " army";
	}
	if ("move" == action)
	{
		if (amount > 1)
		{
			std::cout << "Move " << amount << " armies";
			return;
		}
		std::cout << "Move " << amount << " army";
	}
	if ("createCity" == action)
	{
		if (amount > 1)
		{
			std::cout << "Create " << amount << " cities";
			return;
		}
		std::cout << "Create " << amount << " city";
	}
	if ("waterMove" == action)
	{
		if (amount > 1)
		{
			std::cout << "Move " << amount << " armies across water/land";
			return;
		}
		std::cout << "Move " << amount << " army across water/land";
	}
	if ("destroyArmies"== action)
	{
		if (amount > 1)
		{
			std::cout << "Destroy " << amount << " armies";
			return;
		}
		std::cout << "Destroy " << amount << " army";
	}
}


void Actions::computer_action(Player& p, Cards& c) const
{

	actOb = new ActionOb(&p, &c);

	cout << endl;

	//if there is more than one action
	if (c.actions->size() > 2)
	{

		int selection = 0;

		//if it is a 'and' card, then choose between both actions or ignoring
		if ("AND" == *c.actions->at(2))
		{
			actOb->setAction(c.actions->at(0));
			int amount;
			amount = stoi(*c.actions->at(1));
			actOb->setAmount(&amount);
			actOb->setAction(c.actions->at(3));
			int amount2;
			amount2 = stoi(*c.actions->at(4));
			actOb->setAmount(&amount2);
			c.Notify();
			delete actOb;
			actOb = NULL;
			computer_process(*c.actions->at(0), stoi(*c.actions->at(1)), p);
			computer_process(*c.actions->at(3), stoi(*c.actions->at(4)), p);
		}

		// if it is an 'or' card
		// same as last block
		if ("OR" == *c.actions->at(2))
		{
			if (*(p.get_strategy()->type) == "greedy")
			{
				auto action_index = 0;
				//go through all actions of face-up cards
				for (auto j : *c.actions)
				{
					//do action if any card has them
					if (*j == "destroyArmies")
					{
						actOb->setAction(j);
						int amount;
						amount = stoi(*c.actions->at(action_index + 1));
						actOb->setAmount(&amount);
						c.Notify();
						delete actOb;
						actOb = NULL;
						computer_process(*c.actions->at(action_index), stoi(*c.actions->at(action_index +1)), p);
						return;
					}
					if (*j == "createCity")
					{
						actOb->setAction(j);
						int amount;
						amount = stoi(*c.actions->at(action_index + 1));
						actOb->setAmount(&amount);
						c.Notify();
						delete actOb;
						actOb = NULL;
						computer_process(*c.actions->at(action_index), stoi(*c.actions->at(action_index +1)), p);
						return;
					}
					action_index++;
				}
				actOb->setAction(c.actions->at(0));
				int	amount = stoi(*c.actions->at(1));
				actOb->setAmount(&amount);
				c.Notify();
				delete actOb;
				actOb = NULL;
				computer_process(*c.actions->at(0), stoi(*c.actions->at(1)), p);
				return;
			}
			else if (*(p.get_strategy()->type) == "moderate")
			{
				auto action_index = 0;
				//go through all actions of face-up cards
				for (auto j : *c.actions)
				{
					//do action if any card has them
					if (*j == "placeArmies")
					{
						actOb->setAction(j);
						int amount;
						amount = stoi(*c.actions->at(action_index + 1));
						actOb->setAmount(&amount);
						c.Notify();
						delete actOb;
						actOb = NULL;
						computer_process(*c.actions->at(action_index), stoi(*c.actions->at(action_index + 1)), p);
						return;
					}
					action_index++;
				}
				
				actOb->setAction(c.actions->at(0));
				int amount = stoi(*c.actions->at(1));
				actOb->setAmount(&amount);
				c.Notify();
				delete actOb;
				actOb = NULL;
				computer_process(*c.actions->at(0), stoi(*c.actions->at(1)), p);
				return;
				
			}
		}

	}
	else 
	{
		actOb->setAction(c.actions->at(0));
		int amount = stoi(*c.actions->at(1));
		actOb->setAmount(&amount);
		c.Notify();
		delete actOb;
		actOb = NULL;
		delete actOb;
		actOb = NULL;
		computer_process(*c.actions->at(0), stoi(*c.actions->at(1)), p);
	}
	cout << endl;
}


int random_prime();

void computer_process(const string& action, const int& amount, Player& p)
{

	ProcessActOb* proOb;

		//if it is a place armies card
		if ("placeArmies" == action)
		{
			
			for (int i = 0; i < amount; i++) //for every army to be placed
			{
				bool placed = false;

				if (p.availableArmies() > 0) //if the player has any available armies
				{
					//computer will get a country that is the starting country or a country it has a city in but does not own
					for (auto i : *p.cities)
					{
						if (i->occupiedCountry != nullptr && i->occupiedCountry->owningPlayer != &p)
						{
							proOb = new ProcessActOb(global::action, &p, i->occupiedCountry, 1);
							global::action->Notify();
							p.placeNewArmies(i->occupiedCountry, 1);
							delete proOb;
							proOb = nullptr;
							placed = true;
							break;
						}
					}

					if (Map::getInstance()->startingCountry->owningPlayer != &p && !placed)
					{
						proOb = new ProcessActOb(global::action, &p, Map::getInstance()->startingCountry, 1);
						global::action->Notify();
						p.placeNewArmies(Map::getInstance()->startingCountry, 1);
						delete proOb;
						proOb = nullptr;
						placed = true;
						continue;
					}

					if(!placed)
					{
						proOb = new ProcessActOb(global::action, &p, Map::getInstance()->startingCountry, 1);
						global::action->Notify();
						p.placeNewArmies(Map::getInstance()->startingCountry, 1);
						delete proOb;
						proOb = nullptr;
						placed = true;
					}
					
				}
				else
				{
					cout << "\tComputer has no more armies to place. " << endl;
					break;
				}
			}
		}
		if ("createCity" == action) //virtually the same as the last block but for city placement
		{

			if (p.availableCities() > 0)
			{
				for (auto i : *p.armies)
				{
					if (i->occupiedCountry != nullptr)
					{
						for (auto j : *i->occupiedCountry->cities)
						{
							if (j->player != &p)
							{
								proOb = new ProcessActOb(global::action, &p, i->occupiedCountry);
								global::action->Notify();
								p.buildCity(i->occupiedCountry);
								delete proOb;
								proOb = nullptr;
								return;
							}
						}
					}
				}
				for (auto i : *p.armies)
				{
					if (i->occupiedCountry != nullptr)
					{
						proOb = new ProcessActOb(global::action, &p, i->occupiedCountry);
						global::action->Notify();
						p.buildCity(i->occupiedCountry);
						delete proOb;
						proOb = nullptr;
						return;
					}
				}
			}
			else
			{
				cout << "\tComputer has no more cities to place. " << endl;
			}
			return;
		}
		if ("move" == action || "waterMove" == action)
		{

			if (p.availableArmies() == 14) // check if any armies have been placed
			{
				cout << "\tThe computer player has not placed and armies." << endl;
				return;
			}

			Army* army = nullptr;
			
			int placements = amount;

			bool been_placed = false;

			int t = global::random_range_int(0, p.armies->size() - 1);
			
			for (auto k = 0; k < p.armies->size(); k++, t++)
			{
				if (placements < 0)
					break;

				if (t > p.armies->size() - 1)
					t = 0;

				army = p.armies->at(k);

				if (army->occupiedCountry == nullptr)
				{
					continue;
				}
								
				int count = 0;
				int i = global::random_range_int(0, army->occupiedCountry->adjCountries->size()-1);
				
				while (count < army->occupiedCountry->adjCountries->size())
				{
					if (i > army->occupiedCountry->adjCountries->size()-1)
						i = 0;
				
					Country* country = army->occupiedCountry->adjCountries->at(i);
					
					if ("waterMove" == action)
					{
						proOb = new ProcessActOb(global::action, &p, army->occupiedCountry, country);
						global::action->Notify();
						delete proOb;
						proOb = nullptr;
						p.moveArmies(army->occupiedCountry, country);
						placements--;
						been_placed = true;
						break;
					}
					if ("move" == action)
					{
						if (army->occupiedCountry->parentContinent == country->parentContinent)
						{
							proOb = new ProcessActOb(global::action, &p, army->occupiedCountry, country);
							global::action->Notify();
							delete proOb;
							proOb = nullptr;
							p.moveArmies(army->occupiedCountry, country);
							placements--;
							been_placed = true;
							break;
						}
					}
					i++;
					count++;
				}

			}

			
			if (!been_placed)
				cout << "The computer could not move any armies." << endl;
		}
		if ("destroyArmies" == action)
		{

			if (global::players == nullptr)
			{
				cout << "Error: destroy armies was called but players are not set in global." << endl;
				return;
			}

			if (global::players->size() < 2)
			{
				cout << "Computer was trying to destroy an army but it is the only one playing." << endl;
				return;
			}

			Player* enemy = nullptr;

			//choose random ish player order 
			const auto prime = random_prime();
			const int size = global::players->size();
			auto q = prime % size;
			auto t = q;

			for (auto k = 0; k < size; k++, q = (q + prime) % size)
			{
				enemy = global::players->at(q);

				if (enemy == &p)
				{
					continue;
				}

				for (auto j : *enemy->armies)
				{
					if (j->occupiedCountry != nullptr)
					{
						proOb = new ProcessActOb(global::action, &p, j->occupiedCountry, enemy);
						global::action->Notify();
						delete proOb;
						proOb = nullptr;
						p.destroyArmy(j->occupiedCountry, enemy);
						return;
					}
				}
			}

			cout << "\tComputer had no armies it could destroy." << endl;
			return;

		}
	
}

#include <random> 

int random_prime()
{
	//need a random prime larger than player count
	const int arr[] =
	{
		 31,    37,    41,    43,    47,    53,    59,    61,    67,    71,
		 73,    79,    83,    89,    97,   101,   103,   107,   109,   113,
		127,   131,   137,   139,   149,   151,   157,   163,   167,   173,
		179,   181,   191,   193,   197,   199,   211,   223,   227,   229,
	};

	return arr[global::random_range_int(0, 39)];
}