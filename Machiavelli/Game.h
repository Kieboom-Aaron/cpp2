#pragma once
#include "BaseRole.h"
#include "Player.h"
#include <string>
#include "Deck.h"
#include <memory>
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
	void writeToAll(string text);
	bool switchToRoleOwner(Role r);
	void playRound();
	Deck<BaseRole>rolesDeck;
	vector<shared_ptr<Building>>buildings;
	vector<shared_ptr<BaseRole>>roles;
};

