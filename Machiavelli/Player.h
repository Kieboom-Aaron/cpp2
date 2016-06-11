//
//  Player.hpp
//  socketexample
//
//  Created by Bob Polis on 23-11-15.
//  Copyright © 2015 Avans Hogeschool, 's-Hertogenbosch. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include "Building.h"
#include "Socket.h"
#include <string>
#include <vector>
#include <memory>
#include "BaseRole.h"

using namespace std;

class Game;
class BaseRole;
static int playerCount;

class Player {
public:
	Player() {}
	Player(const std::string& name, const shared_ptr<Socket> socket) : client{ socket }, name{ name }, id{playerCount} { playerCount++; }

	// Player information
	std::string get_name() const { return name; }
	int getId() { return id; }
	void set_name(const std::string& new_name) { name = new_name; }
	shared_ptr<Socket> getClient() { return client; }

	void removeRoles() { roles.clear(); };
	void addRole(shared_ptr<BaseRole> role) { roles.push_back(role); }
	bool isKing = false;
	bool wasKing = false;
	bool firstToEight = false;
	bool canPlay(Role r);
	void setResponse(string r) { response = r; }
	string getResponse() { while (response == "") { } string text = response; response = "";return text; }
	vector<shared_ptr<BaseRole>> getRoles() { return roles; }

	//buildings
	vector<shared_ptr<Building>> getHandBuildings() { return buildingsHand; }
	vector<shared_ptr<Building>> getTableBuildings() { return buildingsTable; }
	void addHandBuilding(shared_ptr<Building> b) { buildingsHand.push_back(b); }
	void addTableBuilding(shared_ptr<Building> b) { buildingsTable.push_back(b); }
	void removeHandBuilding(int index) { buildingsHand.erase(buildingsHand.begin() + index); }
	void removeTableBuilding(int index) { buildingsTable.erase(buildingsTable.begin() + index); }
	//coins
	void addCoins(int amount) { coins += amount; }
	void removeCoins(int amount) { coins -= amount; if (coins < 0) { coins = 0; } }
	int getCoins() { return coins; }
	int getPoints();
	bool hasRole(Role r);
	void getCoinsForBuildings(BuildingColor c);
	void setHandBuildings(vector<shared_ptr<Building>> b) { buildingsHand = b; }
private:
	// Player information
	std::string name;
	int id;
	int coins = 0;
	string response;
	const shared_ptr<Game> game;
	const shared_ptr<Socket> client;
	vector<shared_ptr<BaseRole>>roles;
	vector<shared_ptr<Building>>buildingsHand, buildingsTable;
	bool hasBuilidingWithColor(BuildingColor c);

};

#endif /* Player_hpp */
