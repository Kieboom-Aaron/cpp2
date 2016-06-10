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


bool BaseRole::checkValidInput(int amountOfOwnOptions, int response) {
	return (response >= 0 && response <= (4 + amountOfOwnOptions));
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
			client->write(b->getName() + " costs : "+to_string(b->getCosts())+"\r\n");
		}
		return true;
	}
	case 3: {
		shared_ptr<Socket> client = g.getCurrentPlayer()->getClient();
		for (auto b : g.getCurrentPlayer()->getTableBuildings()) {
			client->write(b->getName() + " costs : " + to_string(b->getCosts()) + "\r\n");
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
					message += b->getName() + " costs : " + to_string(b->getCosts()) + "\r\n";
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
void BaseRole::startTurn(Game g) {
	print(g);
	g.getCurrentPlayer()->getClient()->write("[5] Pak 2 goudstukken \r\n");
	g.getCurrentPlayer()->getClient()->write("[6] Pak 1 bouwkaart \r\n");
	g.getCurrentPlayer()->getClient()->write("machiavelli>");
	int playerInput = getResponse(g, 2);
	if (!handleUserInput(g, playerInput)) {
		switch (playerInput) {
		case 5: {
			g.getCurrentPlayer()->addCoins(2);
			return;
		}
		case 6: {
			return;
		}
		}
	}
}

bool BaseRole::handleBuild(Game g, int playerInput) {
	if(playerInput == 5){
		//TODO handle build command
		return true;
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