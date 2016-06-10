#pragma once
#include <string>
#include <memory>

enum Role {R_MURDERER, R_THIEF, R_MAGIC, R_KING, R_PRIEST, R_TRADER, R_BUILDER, R_CONDOTTIERE };

class Game;
using namespace std;
class BaseRole
{
public:
	BaseRole(string n, Role r);
	~BaseRole();
	virtual void execute(Game g) =0;
	bool checkValidInput(int amountOfOwnOptions, int response);
	bool handleUserInput(Game g, int userInput);
	void startTurn(Game g);
	string getName() { return name; };
	bool stolen;
	bool alive;
	Role getRole() { return role; }
	int getResponse(Game g, int extraOptions);
	bool handleBuild(Game g, int userInput);
protected:
	void print(Game g);
private:
	string name;
	Role role;
};

