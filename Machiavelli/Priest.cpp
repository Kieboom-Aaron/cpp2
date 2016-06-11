#include "Priest.h"
#include "Game.h"


Priest::Priest():BaseRole("Priester", R_PRIEST)
{
}


Priest::~Priest()
{
}

void Priest::execute(Game g) {
	BaseRole::startTurn(g);
	g.getCurrentPlayer()->getCoinsForBuildings(B_BLUE);
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