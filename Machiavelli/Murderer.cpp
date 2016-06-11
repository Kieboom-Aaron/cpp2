#include "Murderer.h"
#include "Game.h"


Murderer::Murderer():BaseRole("Moordenaar", R_MURDERER)
{
}


Murderer::~Murderer()
{
}


bool Murderer::handleKilling(Game g) {
	string message = "Kies iemand om tevermoorden: \n\r [0] annuleer \n\r";
	int c = 1;
	for (auto r : g.getAllRoles()) {
		if (r->getRole() != R_MURDERER) {
			message += "[" + to_string(c) + "] " + r->getName() + "\n\r";
			c++;
		}
	}
	g.getCurrentPlayer()->getClient()->write(message);
	int input = getResponse(g, g.getAllRoles().size() - 1);
	c = 1;
	if (input == 0) {
		return false;
	}
	for (auto r : g.getAllRoles()) {
		if (r->getRole() != R_MURDERER) {
			if (c == input) {
				g.writeToAll("De moordenaar heeft de " + r->getName() + " vermoord \n\r");
				r->alive = false;
				return true;
			}
			c++;
		}
	}

}


void Murderer::execute(Game g) {
	bool done = false;
	BaseRole::startTurn(g);
	BaseRole::print(g);
	g.getCurrentPlayer()->getClient()->write("[5] Bouw een gebouw \r\n");
	g.getCurrentPlayer()->getClient()->write("[6] Vermoord iemand \r\n");
	int input = BaseRole::getResponse(g, 6);
	while (input != 0) {
		if (!BaseRole::handleUserInput(g, input)) {
			if (!BaseRole::handleBuild(g, input)) {
				if (done) {
					g.getCurrentPlayer()->getClient()->write("Je hebt deze beurt al iemand vermoord \r\n");
				}
				else {
					done = handleKilling(g);
				}
			}
		}
		BaseRole::print(g);
		g.getCurrentPlayer()->getClient()->write("[5] Bouw een gebouw \r\n");
		g.getCurrentPlayer()->getClient()->write("[6] vermoord iemand \r\n");
		input = BaseRole::getResponse(g, 6);
	}
}