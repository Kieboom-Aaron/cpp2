#include "Murderer.h"
#include "Game.h"


Murderer::Murderer():BaseRole("Moordenaar", R_MURDERER)
{
}


Murderer::~Murderer()
{
}

void Murderer::execute(Game g) {
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