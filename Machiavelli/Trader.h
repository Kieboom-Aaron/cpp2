#pragma once
#include "BaseRole.h"
class Trader :
	public BaseRole
{
public:
	Trader();
	~Trader();
	void execute(Game g) override;
};

