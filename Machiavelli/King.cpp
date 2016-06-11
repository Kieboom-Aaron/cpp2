#include "King.h"
#include "Game.h"

King::King():BaseRole("Koning", R_KING)
{
}


King::~King()
{
}

void King::execute(Game g) {
	g.getCurrentPlayer()->isKing = true;
	BaseRole::startTurn(g);
	g.getCurrentPlayer()->getCoinsForBuildings(B_ORANGE);
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