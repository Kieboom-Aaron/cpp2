#include "BaseRole.h"
#include "Socket.h"
#include "Game.h"

BaseRole::BaseRole(string n, Role r)
{
	name = n;
	role = r;
}


BaseRole::~BaseRole()
{
}


bool BaseRole::checkValidInput(int amountOfOptions, int response) {
	return (response >= 0 && response <= amountOfOptions);
}

bool BaseRole::handleUserInput(Game g, int userInput) {
	switch (userInput) {
	case 1: {
		shared_ptr<Socket> client = g.getCurrentPlayer()->getClient();
		for (auto r : g.getCurrentPlayer()->getRoles()) {
			client->write(r->getName() + "\r\n");
		}
		return true;
	}
	case 2: {
		shared_ptr<Socket> client = g.getCurrentPlayer()->getClient();
		for (auto b : g.getCurrentPlayer()->getHandBuildings()) {
			client->write(b->getName() + " costs : "+to_string(b->getCosts()) + " kleur : " + Building::mapColor(b->getColor()) +"\r\n");
		}
		return true;
	}
	case 3: {
		shared_ptr<Socket> client = g.getCurrentPlayer()->getClient();
		for (auto b : g.getCurrentPlayer()->getTableBuildings()) {
			client->write(b->getName() + " costs : " + to_string(b->getCosts()) + " kleur : " + Building::mapColor(b->getColor()) + "\r\n");
		}
		return true;
	}
	case 4: {
		shared_ptr<Socket> client = g.getCurrentPlayer()->getClient();
		for (auto p : g.getAllPlayers()) {
			if (p->getId() != g.getCurrentPlayer()->getId()) {
				string message = "speler : " + p->get_name() + "\r\n";
				message += "goudstukken : " + to_string(p->getCoins()) + "\r\n";
				message += "gebouwen op tafel : \r\n";
				for (auto b : p->getTableBuildings()) {
					message += b->getName() + " costs : " + to_string(b->getCosts()) + " kleur : " + Building::mapColor(b->getColor()) + "\r\n";
				}
				message += "aantal gebouwen in de hand : " + to_string(p->getHandBuildings().size()) + "\r\n";

				client->write(message);

			}
		}
		return true;
	}
	default: {
		return false;
	}
	}
}

void BaseRole::print(Game g) {
	string message = "";
	message += "Je hebt " + to_string(g.getCurrentPlayer()->getCoins()) + " goudstukken. \r\n ";
	message += "Wat wil je doen? \r\n";
	message += "[0] Ik ben klaar. \r\n";
	message += "[1] Kijken welke rollen je hebt. \r\n";
	message += "[2] Kijken welke gebouwen je in je hand hebt \r\n";
	message += "[3] Kijken welke gebouwen je gebouwd hebt \r\n";
	message += "[4] Kijken hoe andere ervoor staan \r\n";
	g.getCurrentPlayer()->getClient()->write(message);
}


void BaseRole::getCards(Game g) {
	vector<shared_ptr<Building>>opts;
	opts.push_back(g.getLastBuilding());
	opts.push_back(g.getLastBuilding());
	string message = "Kies een van de 2 kaarten: \n\r";
	message += "[0]" + opts.at(0)->getName() + " costs : " + to_string(opts.at(0)->getCosts()) + " kleur : " + Building::mapColor(opts.at(0)->getColor()) + "\n\r";
	message += "[1]" + opts.at(1)->getName() + " costs : " + to_string(opts.at(1)->getCosts()) + " kleur : " + Building::mapColor(opts.at(0)->getColor()) + "\n\r machiavelli>";
	g.getCurrentPlayer()->getClient()->write(message);
	int input = getResponse(g, 1);
	g.getCurrentPlayer()->addHandBuilding(opts.at(input));
}

void BaseRole::startTurn(Game g) {
	if (stolen) {
		int c = g.getCurrentPlayer()->getCoins();
		g.getCurrentPlayer()->removeCoins(c);
		for (auto p : g.getAllPlayers()) {
			if (p->hasRole(R_THIEF)) {
				p->addCoins(c);
				g.writeToAll("De dief heeft " + to_string(c) + " goudstukkern van de " + name + " gestolen \n\r");
			}
		}
	}
	print(g);
	g.getCurrentPlayer()->getClient()->write("[5] Pak 2 goudstukken \r\n");
	g.getCurrentPlayer()->getClient()->write("[6] Pak 1 bouwkaart \r\n");
	g.getCurrentPlayer()->getClient()->write("machiavelli>");
	int playerInput = getResponse(g, 6);
	if (!handleUserInput(g, playerInput)) {
		switch (playerInput) {
		case 5: {
			g.getCurrentPlayer()->addCoins(2);
			return;
		}
		case 6: {
			getCards(g);
			return;
		}
		}
	}
	else {
		startTurn(g);
	}
}

bool BaseRole::handleBuild(Game g, int playerInput) {
	if(playerInput == 5){
		if (g.getCurrentPlayer()->getHandBuildings().size() == 0) {
			g.getCurrentPlayer()->getClient()->write("Je hebt geen gebouwen\n\r");
			return true;
		}
		if (buildCount == 0) {
			g.getCurrentPlayer()->getClient()->write("Je hebt al gebouwt deze ronden\n\r");
			return true;
		}
		if (g.getCurrentPlayer()->getTableBuildings().size() >= buildingLimit) {
			g.getCurrentPlayer()->getClient()->write("Je hebt het maximale aantal gebouwen bereikt\n\r");
			return true;
		}
		string message = "Kies een gebouw om te bouwen \n\r";
		message += "[0] Annuleer \n\r";
		int c = 1;
		for (auto b : g.getCurrentPlayer()->getHandBuildings()) {
			message += "["+to_string(c)+"]" + b->getName() + " costs : " + to_string(b->getCosts()) + "\r\n";
			c++;
		}
		message += "machiavelli>";
		g.getCurrentPlayer()->getClient()->write(message);
		int userInput = getResponse(g, c - 1);
		int invalidInput = true;
		while (invalidInput) {
			if (userInput == 0) {
				return true;
			}
			else {
				shared_ptr<Building> b = g.getCurrentPlayer()->getHandBuildings().at(userInput - 1);
				if (b->getCosts() > g.getCurrentPlayer()->getCoins()) {
					g.getCurrentPlayer()->getClient()->write("Je hebt niet genoeg goudstukken om dit gebouw te bouwen, kies een andere optie\n\r machiavelli>");
					userInput = getResponse(g, c - 1);
				}
				else {
					g.getCurrentPlayer()->removeCoins(b->getCosts());
					g.getCurrentPlayer()->removeHandBuilding(userInput - 1);
					g.getCurrentPlayer()->addTableBuilding(b);
					invalidInput = false;
					if (g.getCurrentPlayer()->getTableBuildings().size() >= 8) {
						bool first = true;
						for (auto p : g.getAllPlayers()) {
							if (p->getTableBuildings().size() >= 8) {
								first = false;
							}
						}
						g.getCurrentPlayer()->firstToEight = first;
					}
					g.getCurrentPlayer()->getClient()->write("Je hebt het gebouw gebouwt\n\r");
					buildCount--;
					return true;
				}
			}
		}
	}
	return false;
}

int BaseRole::getResponse(Game g, int extraOptions) {
	g.getCurrentPlayer()->getClient()->write("machiavelli>");
	int playerInput = atoi(g.getCurrentPlayer()->getResponse().c_str());
	while (!checkValidInput(extraOptions, playerInput)) {
		g.getCurrentPlayer()->getClient()->write("machiavelli>");
		playerInput = atoi(g.getCurrentPlayer()->getResponse().c_str());
	}
	return playerInput;
}

void BaseRole::reset() {
	stolen = false;
	alive = true;
	buildCount = 1;
}