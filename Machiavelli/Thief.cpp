#include "Thief.h"
#include "Game.h"


Thief::Thief():BaseRole("Dief", R_THIEF)
{
}


Thief::~Thief()
{
}


bool Thief::handleRobery(Game g) {
	string message = "Kies iemand om te beroven: \n\r [0] annuleer \n\r";
	int c = 1;
	for (auto r : g.getAllRoles()) {
		if (r->getRole() != R_THIEF && r->getRole() != R_MURDERER) {
			message += "[" + to_string(c) + "] " + r->getName() + "\n\r";
			c++;
		}
	}
	g.getCurrentPlayer()->getClient()->write(message);
	int input = getResponse(g, g.getAllRoles().size() - 1);
	if (input == 0) {
		return false;
	}
	c = 1;
	for (auto r : g.getAllRoles()) {
		if (r->getRole() != R_THIEF && r->getRole() != R_MURDERER) {
			if (c == input) {
				g.writeToAll("De dief beroofd de " + r->getName() + " \n\r");
				r->stolen = true;
				return true;
			}
			c++;
		}
	}

}

void Thief::execute(Game g) {
	bool done = false;
	BaseRole::startTurn(g);
	BaseRole::print(g);
	g.getCurrentPlayer()->getClient()->write("[5] Bouw een gebouw \r\n");
	g.getCurrentPlayer()->getClient()->write("[6] Beroof iemand \r\n");
	int input = BaseRole::getResponse(g, 6);
	while (input != 0) {
		if (!BaseRole::handleUserInput(g, input)) {
			if (!BaseRole::handleBuild(g, input)) {
				if (done) {
					g.getCurrentPlayer()->getClient()->write("Je hebt deze beurt al iemand vermoord \r\n");
				}
				else {
					done = handleRobery(g);
				}
			}
		}
		BaseRole::print(g);
		g.getCurrentPlayer()->getClient()->write("[5] Bouw een gebouw \r\n");
		g.getCurrentPlayer()->getClient()->write("[6] Beroof iemand \r\n");
		input = BaseRole::getResponse(g, 6);
	}
}