//
//  Player.cpp
//  socketexample
//
//  Created by Bob Polis on 23-11-15.
//  Copyright © 2015 Avans Hogeschool, 's-Hertogenbosch. All rights reserved.
//
#include "Player.h"
#include "BaseRole.h"

using namespace std;

bool Player::hasRole(Role role) {
	for (auto r : roles) {
		if (r->getRole() == role) {
			return true;
		}
	}
	return false;
}

bool Player::canPlay(Role role) {
	for (auto r : roles) {
		if (r->getRole() == role) {
			return r->alive;
		}
	}
	return false;
}

int Player::getPoints() {
	int points = 0;
	if (firstToEight) {
		points += 4;
	}
	else if (buildingsTable.size() >= 8) {
		points += 2;
	}
	if (hasBuilidingWithColor(B_BLUE) &&
		hasBuilidingWithColor(B_RED) &&
		hasBuilidingWithColor(B_ORANGE) &&
		hasBuilidingWithColor(B_GREEN) &&
		hasBuilidingWithColor(B_PURPLE)
		) {
		points += 3;
	}
	for (auto b : buildingsTable) {
		points += b->getCosts();
	}
	return points;
}

bool Player::hasBuilidingWithColor(BuildingColor c) {
	for (auto b : buildingsTable) {
		if (b->getColor() == c) {
			return true;
		}
	}
	return false;
}

void Player::getCoinsForBuildings(BuildingColor c) {
	int count = 0;
	for (auto b : buildingsTable) {
		if (b->getColor() == c) {
			count++;
		}
	}
	client->write("Je hebt " + to_string(count) + " goudstukken ontvangen voor je gebouwen \n\r");
	addCoins(count);
}