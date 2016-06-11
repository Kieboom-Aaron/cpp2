#include "Trader.h"
#include "Game.h"


Trader::Trader():BaseRole("Koopman", R_TRADER)
{
}


Trader::~Trader()
{
}

void Trader::execute(Game g) {
	BaseRole::startTurn(g);
	
	g.getCurrentPlayer()->addCoins(1);
	g.getCurrentPlayer()->getClient()->write("Je hebt 1 goudstuk ontvangen omdat je Koopman bent \r\n");
	g.getCurrentPlayer()->getCoinsForBuildings(B_GREEN);
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