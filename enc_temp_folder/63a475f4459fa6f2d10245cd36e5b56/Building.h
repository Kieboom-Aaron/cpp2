#pragma once
#include <string>

using namespace std;

class Building
{
public:
	Building(string name, int costs);
	~Building();
	string getName() { return name; }
	int getCosts() { return costs; }
private:
	string name;
	int costs;
};

