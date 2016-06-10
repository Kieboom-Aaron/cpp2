#pragma once
#include "BaseRole.h"
class King:public BaseRole
{
public:
	King();
	~King();
	void execute(Game g) override;
};

