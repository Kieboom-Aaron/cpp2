#include "Game.h"
#include "BaseRole.h"
#include "Murderer.h"
#include "Thief.h"
#include "King.h"
#include <thread>

Game::Game()
{
	roles.push_back(make_shared<Murderer>(Murderer()));
	roles.push_back(make_shared<Thief>(Thief()));
	roles.push_back(make_shared<King>(King()));
	buildings.push_back(shared_ptr<Building>(new Building{ "Buildin1", 1 }));
	buildings.push_back(make_shared<Building>(Building("Buildin2", 2)));
	buildings.push_back(make_shared<Building>(Building("Buildin3", 3)));
	buildings.push_back(make_shared<Building>(Building("Buildin4", 4)));
}


Game::~Game()
{
}


void Game::handleCommand(shared_ptr<Player> p, string cmd) {
	if (!isFull()) {
		p->getClient()->write("Het spel is nog niet begonnen");
		return;
	}
	if (p->getId() == currentPlayer->getId()) {
		currentPlayer->setResponse(cmd);
	}
	else {
		p->getClient()->write("Je bent nog niet aan de beurt");
	}
}

void Game::addPlayer(std::shared_ptr<Player> p) {
	players.push_back(p);
	if (isFull()) {
		thread g{ &Game::start, this };
		g.detach();
	}
}

void Game::removePlayer(std::shared_ptr<Player> p) {
	for (size_t i = 0; i< players.size(); i++) {
		if (p->getId() == players[i]->getId()) {
			players.erase(players.begin() + i);
		}
	}
}

void Game::start() {
	writeToAll("Het spel is gestart \r\n");
	for (auto player : players) {
		player->addCoins(2);
		player->addHandBuilding(buildings.at(0));
		buildings.erase(buildings.begin());
	}

	//TODO add buildings
	currentPlayer = players.at(0);
	
	currentPlayer->isKing = true;
	round();
}

void Game::resetDeck() {
	if (rolesDeck.GetDeckSize() > 0) {
		rolesDeck.ClearDeck();
	}

	for (auto r : roles) {
		r->stolen = false;
		r->alive = true;
		rolesDeck.AddCard(r);
	}
	rolesDeck.ShuffleDeck();

	int i = 0;
	if (rolesDeck.GetDeck().at(i)->getRole() == R_KING) {
		i++;
	}
	currentPlayer->getClient()->write("De " + rolesDeck.GetDeck().at(i)->getName() + " is uit de stapel gehaald\r\n");
	rolesDeck.RemoveCardIndex(i);
}

void Game::round() {
	//set new king + reset player roles
	string kingsName = "";
	for (auto player : players) {
		player->removeRoles();
		if (player->isKing) {
			kingsName = player->get_name();
			currentPlayer = player;
			currentPlayer->wasKing = false;
		}
	}
	if (kingsName == "") {
		for (auto player : players) {
			if (player->wasKing) {
				player->isKing = true;
				player->wasKing = false;
			}
		}
	}
	currentPlayer->isKing = false;

	resetDeck();

	writeToAll("Er is een nieuwe ronden gestart, "+kingsName + " is nu aan de beurt om een rol te kiezen");
	chooseRoles();
	writeToAll("alle rollen zijn verdeeld, we kunnen starten");
	
	playRound();


}

void Game::writeToAll(string text) {
	for (auto player : players) {
		player->getClient()->write(text);
	}
}

void Game::chooseRoles() {
	pickRole(false);
	nextPlayer();
	while (rolesDeck.GetDeckSize() > 0) {
		currentPlayer->getClient()->write("Het is jouw beurt om een kaart te kiezen");
		pickRole(true);
		nextPlayer();
	}
}

void Game::nextPlayer()
{
	int index = 0;
	int count = 0;
	for (const auto &p : players) {
		if (p->getId() == currentPlayer->getId() && count + 1 < players.size()) {
			index = count + 1;
			break;
		}
	}
	currentPlayer->getClient()->write("Je beurt is voorbij, je zut even moeten wachten\r");
	currentPlayer = players.at(index);
}

void Game::showAvailableRoles() {
	string text = "";
	int i = 1;
	for (const auto &r : rolesDeck.GetDeck()) {
		text += "[" + std::to_string(i) + "]" + r->getName()+"\n\r";
		i++;
	}
	currentPlayer->getClient()->write(text);
}

void Game::pickRole(bool remove)
{
	currentPlayer->getClient()->write("Er zijn " + std::to_string(rolesDeck.GetDeckSize()) + " rollen over.\r\nWelke wil je ?\n\r");
	showAvailableRoles();
	currentPlayer->getClient()->write("machiavelli>>");
	bool validInput = false;
	while (!validInput) {
		string response = currentPlayer->getResponse();
		int id = atoi(response.c_str());
		if (id >= 1 && id <= rolesDeck.GetDeck().size()) {
			currentPlayer->addRole(rolesDeck.GetDeck().at(id - 1));
			currentPlayer->getClient()->write("Je hebt de rol van " + rolesDeck.GetDeck().at(id - 1)->getName() + " gekozen.\r\n");
			rolesDeck.RemoveCardIndex(id - 1);
			validInput = true;
		}
		else {
			currentPlayer->getClient()->write("Deze rol bestaat niet. kies opnieuw\r\nmachiavelli> ");
		}
	}

	if (remove && rolesDeck.GetDeckSize() > 0) {
		currentPlayer->getClient()->write("Er zijn " + std::to_string(rolesDeck.GetDeckSize()) + " rollen over.\r\nWelke wil je ?\n\rmachiavelli>>");
		showAvailableRoles();
		bool validInput = false;
		while (!validInput) {
			string response = currentPlayer->getResponse();
			int id = atoi(response.c_str());
			if (id >= 1 && id <= rolesDeck.GetDeck().size()) {
				if (rolesDeck.GetDeck().at(id - 1)->getRole() == R_KING && rolesDeck.GetDeckSize() > 1) {
					currentPlayer->getClient()->write("Je kan de koning niet uit het spel halen. Probeer een andere kaart\r\nmachiavelli> ");
				}
				else {
					currentPlayer->getClient()->write("Je hebt de "+ rolesDeck.GetDeck().at(id - 1)->getName()+" uit het spel gehaald");
					rolesDeck.RemoveCardIndex(id - 1);
					validInput = true;
				}
			}
			else {
				currentPlayer->getClient()->write("Deze rol bestaat niet. kies opnieuw\r\nmachiavelli> ");
			}
		}
	}
}

bool Game::switchToRoleOwner(Role r) {
	for (auto p : players) {
		if (p->canPlay(r)) {
			p->getClient()->write("Jij hebt deze rol en bent nu dus aan de beurt\r\n");
			currentPlayer = p;
			return true;
		}
	}
	return false;
}


void Game::playRound()
{
	int currentRole = 0;

	while (currentRole < roles.size()) {
		int  playerBuild = 0;
		writeToAll("De " + roles.at(currentRole)->getName() + " is aan de beurt\r\n");
		
		if (switchToRoleOwner(roles.at(currentRole)->getRole())) {
			for (auto p : players) {
				if (p->getId() != currentPlayer->getId()) {
					p->getClient()->write(currentPlayer->get_name() + "is de " + roles.at(currentRole)->getName()+"\n\r");
				}
			}
			roles.at(currentRole)->execute(*this);
		}
		currentRole++;
	}
}