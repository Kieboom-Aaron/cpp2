#pragma once
#include "BaseRole.h"
class Condottiere :
	public BaseRole
{
public:
	Condottiere();
	~Condottiere();
	void execute(Game g) override;
};

