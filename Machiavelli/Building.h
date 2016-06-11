#pragma once
#include <string>
enum BuildingColor {B_ORANGE, B_BLUE, B_RED, B_GREEN, B_PURPLE};
using namespace std;

class Building
{
public:
	Building(string name, int costs, BuildingColor b);
	~Building();
	string getName() { return name; }
	int getCosts() { return costs; }
	BuildingColor getColor() { return color; }
	static string mapColor(BuildingColor c) {
		switch (c) {
		case B_ORANGE:
			return "Oranje";
		case B_BLUE:
			return "Blauw";
		case B_RED:
			return "Rood";
		case B_GREEN:
			return "Groen";
		case B_PURPLE:
			return "Lila";
	}
	}
private:
	string name;
	int costs;
	BuildingColor color;
};

