#include "Condottiere.h"
#include "Game.h"


Condottiere::Condottiere():BaseRole("Condottiere", R_CONDOTTIERE)
{
}


Condottiere::~Condottiere()
{
}

void Condottiere::execute(Game g) {
	BaseRole::startTurn(g);
	BaseRole::print(g);
	g.getCurrentPlayer()->getClient()->write("[5] Bouw een gebouw \r\n");
	int input = BaseRole::getResponse(g, 1);
	if (!BaseRole::handleUserInput(g, input)) {
		if (!BaseRole::handleBuild(g, input)) {
			//role behaviour
		}
	}
	else {
		execute(g);
	}
}