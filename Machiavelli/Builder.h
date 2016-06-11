#pragma once
#include "BaseRole.h"
class Builder :
	public BaseRole
{
public:
	Builder();
	~Builder();
	void execute(Game g) override;
	void reset() override;
};

