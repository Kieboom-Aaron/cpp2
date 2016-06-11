#include "Condottiere.h"
#include "Game.h"


Condottiere::Condottiere():BaseRole("Condottiere", R_CONDOTTIERE)
{
}


Condottiere::~Condottiere()
{
}

bool Condottiere::handleDestruction(Game g, int amountOfBuildings) {
	int input = getResponse(g, amountOfBuildings);
	bool invalidInput = true;
	while (invalidInput) {
		if (input == 0) {
			return false;
		}
		int c = 1;
		for (auto p : g.getAllPlayers()) {
			if (p->getId() != g.getCurrentPlayer()->getId() && !p->hasRole(R_PRIEST)) {
				int count = 0;
				for (auto b : p->getTableBuildings()) {
					if (c == input) {
						if (b->getCosts() - 1 > g.getCurrentPlayer()->getCoins()) {
							g.getCurrentPlayer()->getClient()->write("Je hebt nie genoeg goudstukken om dit gebouw te slopen, kies een ander gebouw\n\r machiavelli>");
							input = getResponse(g, amountOfBuildings);
						}
						else {
							g.getCurrentPlayer()->removeCoins(b->getCosts() - 1);
							p->removeTableBuilding(count);
							g.getCurrentPlayer()->getClient()->write("Je hebt het gebouw gesloopt \n\r");
							invalidInput = false;
							return true;
						}
					}
					c++;
					count++;
				}
			}
		}
		input = getResponse(g, amountOfBuildings);

	}
}



void Condottiere::execute(Game g) {
	bool done = false;
	BaseRole::startTurn(g);
	g.getCurrentPlayer()->getCoinsForBuildings(B_RED);
	BaseRole::print(g);
	g.getCurrentPlayer()->getClient()->write("[5] Bouw een gebouw \r\n");
	g.getCurrentPlayer()->getClient()->write("[6] Sloop het gebouw van een ander \r\n");
	int input = BaseRole::getResponse(g, 6);
	while (input != 0) {
		if (!BaseRole::handleUserInput(g, input)) {
			if (!BaseRole::handleBuild(g, input)) {
				if (done) {
					g.getCurrentPlayer()->getClient()->write("Je hebt deze ronden al een gebouw gesloopt\r\n");
				}
				else {
					g.getCurrentPlayer()->getClient()->write("Welk gebouw wil je slopen\r\n");
					g.getCurrentPlayer()->getClient()->write("[0] Annuleer\n\r");
					int c = 1;
					for (auto p : g.getAllPlayers()) {
						if (p->getId() != g.getCurrentPlayer()->getId() && !p->hasRole(R_PRIEST)) {
							string message = "speler : " + p->get_name() + "\r\n";
							for (auto b : p->getTableBuildings()) {
								message += "["+to_string(c)+"]" + b->getName() + " costs : " + to_string(b->getCosts()-1) + "\r\n";
								c++;
							}
							
							g.getCurrentPlayer()->getClient()->write(message);
						}
					}
					g.getCurrentPlayer()->getClient()->write("machiavelli");
					done = handleDestruction(g, c);
				}
			}
		}
		BaseRole::print(g);
		g.getCurrentPlayer()->getClient()->write("[5] Bouw een gebouw \r\n");
		g.getCurrentPlayer()->getClient()->write("[6] Sloop het gebouw van een ander \r\n");
		input = BaseRole::getResponse(g, 6);
	}
}