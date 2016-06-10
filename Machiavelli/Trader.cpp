#include "Trader.h"
#include "Game.h"


Trader::Trader():BaseRole("Koopman", R_TRADER)
{
}


Trader::~Trader()
{
}

void Trader::execute(Game g) {
	g.getCurrentPlayer()->addCoins(1);
	g.getCurrentPlayer()->getClient()->write("Je hebt 1 goudstuk ontvangen omdat je Koopman bent \r\n");
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