#pragma once
#include "BaseRole.h"
class Thief:public BaseRole
{
public:
	Thief();
	~Thief();
	void execute(Game g) override;
};

