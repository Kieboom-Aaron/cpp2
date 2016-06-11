#include "Builder.h"
#include "Game.h"


Builder::Builder():BaseRole("Bouwmeester", R_BUILDER)
{
	buildingLimit = 999;
}


Builder::~Builder()
{
}

void Builder::execute(Game g) {
	BaseRole::startTurn(g);
	auto b = g.getLastBuilding();
	auto b1 = g.getLastBuilding();
	g.getCurrentPlayer()->addHandBuilding(b);
	g.getCurrentPlayer()->addHandBuilding(b1);
	g.getCurrentPlayer()->getClient()->write("Je hebt 2 bouwkaarten afgepakt omdat je bouwmeester bent : " + b->getName() + ", " + b1->getName()+"\n\r");
	BaseRole::print(g);
	g.getCurrentPlayer()->getClient()->write("[5] Bouw een gebouw \r\n");
	int input = BaseRole::getResponse(g, 5);
	while (input != 0) {
		if (!BaseRole::handleUserInput(g, input)) {
			if (!BaseRole::handleBuild(g, input)) {
				//role behaviour
			}
		}
		BaseRole::print(g);
		g.getCurrentPlayer()->getClient()->write("[5] Bouw een gebouw \r\n");
		input = BaseRole::getResponse(g, 5);
	}
}

void Builder::reset() {
	stolen = false;
	alive = true;
	buildCount = 3;
}