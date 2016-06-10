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

bool Player::canPlay(Role role) {
	for (auto r : roles) {
		if (r->getRole() == role) {
			return true;
		}
	}
	return false;
}