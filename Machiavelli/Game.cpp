#include "Game.h"
#include "BaseRole.h"
#include "Murderer.h"
#include "Thief.h"
#include "King.h"
#include "Builder.h"
#include "Condottiere.h"
#include "Priest.h"
#include "Magician.h"
#include "Trader.h"
#include <thread>

Game::Game()
{
	
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
	writeToAll(p->get_name() + " heeft het spel verlaten. Er zijn niet genoeg spelers om verder te spelen \r\n");
}

shared_ptr<Building> Game::getLastBuilding() {
	return buildings.GetLast();
}

void Game::start() {
	readCSV("../Resources/bouwkaarten.csv", T_BUILDINGS);
	readCSV("../Resources/karakterkaarten.csv", T_ROLES);
	buildings.ShuffleDeck();
	writeToAll("Het spel is gestart \r\n");
	for (auto player : players) {
		player->addCoins(2);
		player->addHandBuilding(buildings.GetLast());
		player->addHandBuilding(buildings.GetLast());
		player->addHandBuilding(buildings.GetLast());
		player->addHandBuilding(buildings.GetLast());
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
		r->reset();
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

	writeToAll("Er is een nieuwe ronden gestart, "+kingsName + " is nu aan de beurt om een rol te kiezen \n\r");
	chooseRoles();
	writeToAll("alle rollen zijn verdeeld, we kunnen starten");
	
	playRound();

	checkVictory();
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
		count++;
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

void Game::checkVictory() {
	bool vic = false;
	for (auto p : players) {
		if (p->firstToEight) {
			vic = true;
		}
	}
	if (vic) {
		string	name = "";
		int		points = 0;

		for (auto p : players) {
			if (points < p->getPoints()) {
				points = p->getPoints();
				name = p->get_name();
			}
			p->getClient()->write("Je hebt:" + std::to_string(p->getPoints()) + " punten!  \r\n");
		}

		writeToAll(name + " heeft gewonnen met " + std::to_string(points) + " punten!  \r\n");
	}
	else {
		round();
	}
}

BuildingColor Game::parseColor(string t) {
	if (t == "geel") 
		return B_ORANGE;
	if (t == "blauw")
		return B_BLUE;
	if (t == "rood")
		return B_RED;
	if (t == "groen")
		return B_GREEN;
	if (t == "lila")
		return B_PURPLE;
}

shared_ptr<BaseRole> Game::parseRole(string t) {

	if(t == "Moordenaar")
		return make_shared<Murderer>(Murderer());
	if(t == "Dief")
		return make_shared<Thief>(Thief());
	if( t== "Magiër")
		return make_shared<Magician>(Magician());
	if( t == "Koning")
		return make_shared<King>(King());
	if (t == "Prediker")
		return make_shared<Priest>(Priest());
	if (t == "Koopman")
		return make_shared<Trader>(Trader());
	if (t == "Bouwmeester")
		return make_shared<Builder>(Builder());
	if (t == "Condottiere")
		return make_shared<Condottiere>(Condottiere());

}



void Game::readCSV(string path, FileType type) {

	ifstream infile(path);

	while (infile)
	{
		string s;
		if (!getline(infile, s)) break;

		istringstream ss(s);

		while (ss)
		{
			string s;
			if (!getline(ss, s, ',')) break;


			vector <string> record;
			stringstream ss(s);
			string item;
			while (std::getline(ss, item, ';')) {
				record.push_back(item);
			}

			if (type == T_BUILDINGS) {
				buildings.AddCard(make_shared<Building>(Building(record.at(0), atoi(record.at(1).c_str()), parseColor(record.at(2)))));
			}
			else {
				roles.push_back(parseRole(record.at(1)));
			}
		}
	}
	if (!infile.eof())
	{
		cerr << "Fooey!\n";
	}
}