/*
*	File: player.cpp
*	Authors: Charlotte Murphy, Lauren Reitz, and Yongshi Ye
*	Course: CS 467-400
*	Assignment: Capstone Project
*	Due: March 16, 2018
*	Desc: Player class source file. Defines Player object for game.cpp. Tracks players location,
*		inventory, hints, and cheat status. Managed by Game. Interacts with Room
*	Usage: For use with main.cpp and player.h
*
*/

#include "player.h"

// default constructor - used with saved game
Player::Player() {
	//
}

// constructor used with new game
Player::Player(string playerName) {
	name = playerName;
	cheatStatus = false;
	availableHints = 1;
}

void Player::setName(string playerName) {
	name = playerName;
}

void Player::setLocation(Room* nextRoom) {
	location = nextRoom;
	//location->setVisited(1);
}

void Player::setCheatStatus() {
	if (bag[15] == 1) {
		cheatStatus = true;
	}
	else {
		cheatStatus = false;
	}
}

void Player::setItem(int itemId, int isInBag, Item* itemPtr) {
	bag[itemId] = isInBag;
	bagItemPtrs[itemId] = itemPtr;
}

string Player::getName() {
	return name;
}

Room* Player::getLocation() {
	return location;
}

int Player::getNumGemsInBag() {
	int numGems = 0;
	// count blue gem through white gem
	for (int i = 9; i < 15; i++) {
		if (bag[i] == 1) {
			numGems++;
		}
	}
	return numGems;
}

bool Player::getCheatStatus() {
	return cheatStatus;
}

Item* Player::getBagItem(int itemId) {
	if (bag[itemId] != 1) {
		return nullptr;
	}
	else {
		return bagItemPtrs[itemId];
	}
}

int Player::isInBag(int itemId) {
	if (itemId < 0 || 15 < itemId) {
		cout << "Error: isInBag() invalid item id" << endl;
		return 0;
	}
	if (bag[itemId] == 1) {
		return 1;
	}
	else {
		return 0;
	}
}

void Player::displayLocation() {
	location->displayName();
}

void Player::listBag() {
	cout << "You have these items in your bag: " << endl << endl;
	for (int i = 0; i < 16; i++) {
		if (bag[i] > 0) {
			cout << "     " << bagItemPtrs[i]->getName() << endl;
		}
	}
}