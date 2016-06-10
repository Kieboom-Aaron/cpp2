#pragma once
#include "BaseRole.h"

class Priest :
	public BaseRole
{
public:
	Priest();
	~Priest();
	void execute(Game g) override;
};

