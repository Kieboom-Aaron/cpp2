#include "Magician.h"
#include "Game.h"


Magician::Magician():BaseRole("Magier", R_MAGIC)
{
}


void Magician::handleRemove(Game g, int amount) {
	for (int i = 0; i < amount; i++) {
		string message = "Kies een gebouw om te wisselen \n\r";
		int c = 0;
		for (auto b : g.getCurrentPlayer()->getHandBuildings()) {
			message += "[" + to_string(c) + "]" + b->getName() + " costs : " + to_string(b->getCosts()) + "\r\n";
			c++;
		}
		message += "machiavelli>";
		g.getCurrentPlayer()->getClient()->write(message);
		int userInput = getResponse(g, c - 1);
		g.getCurrentPlayer()->removeHandBuilding(userInput);
	}
	for (int i = 0; i < amount; i++) {
		g.getCurrentPlayer()->addHandBuilding(g.getLastBuilding());
	}
	g.getCurrentPlayer()->getClient()->write("De gebouwen zijn gewisseld \n\r");
}

Magician::~Magician()
{
}

void Magician::execute(Game g) {
	bool done = false;
	BaseRole::startTurn(g);
	BaseRole::print(g);
	g.getCurrentPlayer()->getClient()->write("[5] Bouw een gebouw \r\n");
	g.getCurrentPlayer()->getClient()->write("[6] Wissel je kaarten met je tegenstander \r\n");
	g.getCurrentPlayer()->getClient()->write("[7] Wissel je kaarten met de stock \r\n");
	int input = BaseRole::getResponse(g, 7);
	while (input != 0) {
		if (!BaseRole::handleUserInput(g, input)) {
			if (!BaseRole::handleBuild(g, input)) {
				if (done) {
					g.getCurrentPlayer()->getClient()->write("Je hebt deze beurt je kaarten al gewisseld \r\n");
				}
				else if(input ==6){
					for (auto p : g.getAllPlayers()) {
						if (p->getId() != g.getCurrentPlayer()->getId()) {
							auto temp = g.getCurrentPlayer()->getHandBuildings();
							g.getCurrentPlayer()->setHandBuildings(p->getHandBuildings());
							p->setHandBuildings(temp);
							g.getCurrentPlayer()->getClient()->write("Je hebt de kaarten gewisseld \r\n");
						}
					}
						done = true;

				}
				else if (input == 7) {
					g.getCurrentPlayer()->getClient()->write("hoeveel kaarten wil je wisselen max "+to_string(g.getCurrentPlayer()->getHandBuildings().size())+" \n\r machiavelli>");
					int amount = getResponse(g, g.getCurrentPlayer()->getHandBuildings().size());
					if (amount > 0) {
						done = true;
					}
					handleRemove(g, amount);
				}
			}
		}
		BaseRole::print(g);
		g.getCurrentPlayer()->getClient()->write("[5] Bouw een gebouw \r\n");
		g.getCurrentPlayer()->getClient()->write("[6] wissel je kaarten met iemand \r\n");
		g.getCurrentPlayer()->getClient()->write("[7] Wissel je kaarten met de stock \r\n");
		input = BaseRole::getResponse(g, 7);
	}
}