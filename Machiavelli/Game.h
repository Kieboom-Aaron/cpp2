#pragma once
#include "BaseRole.h"
#include "Player.h"
#include <string>
#include "Deck.h"
#include <memory>
enum FileType {T_BUILDINGS, T_ROLES};
class Game
{
public:
	Game();
	~Game();
	
	void handleCommand(shared_ptr<Player> p, string command);
	void addPlayer(shared_ptr<Player> p);
	void removePlayer(shared_ptr<Player> p);
	bool isFull() { return players.size() > 1; }
	shared_ptr<Player> getCurrentPlayer() { return currentPlayer; }
	vector<shared_ptr<Player>>getAllPlayers() { return players; }
	vector<shared_ptr<BaseRole>>getAllRoles() { return roles; }
	void writeToAll(string text);
	shared_ptr<Building>getLastBuilding();
	Deck<Building>buildings;
private:
	vector<shared_ptr<Player>> players;
	shared_ptr<Player> currentPlayer;
	void nextPlayer();
	void resetDeck();
	void start();
	void round();
	void chooseRoles();
	void pickRole(bool skipRemove);
	void showAvailableRoles();
	void checkVictory();
	
	bool switchToRoleOwner(Role r);
	void playRound();
	Deck<BaseRole>rolesDeck;
	vector<shared_ptr<BaseRole>>roles;
	shared_ptr<BaseRole> parseRole(string t);
	BuildingColor parseColor(string t);
	void readCSV(string path, FileType type);
};

