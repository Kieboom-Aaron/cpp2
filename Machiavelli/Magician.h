#pragma once
#include "BaseRole.h"
class Game;
class Magician :
	public BaseRole
{
public:
	Magician();
	~Magician();
	void execute(Game g) override;
private:
	void handleRemove(Game g, int amount);
};

