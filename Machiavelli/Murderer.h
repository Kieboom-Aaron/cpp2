#pragma once
#include "BaseRole.h"
class Game;
class Murderer:public BaseRole
{
public:
	Murderer();
	~Murderer();
	void execute(Game g) override;
	bool handleKilling(Game g);

};

