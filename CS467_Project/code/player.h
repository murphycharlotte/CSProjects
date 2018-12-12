/*
*	File: player.h
*	Authors: Charlotte Murphy, Lauren Reitz, and Yongshi Ye
*	Course: CS 467-400
*	Assignment: Capstone Project
*	Due: March 16, 2018
*	Desc: Player class header file. Defines Player object for game.cpp. Tracks players location,
*		inventory, hints, and cheat status. Managed by Game. Interacts with Room
*	Usage: For use with main.cpp and player.cpp
*
*/

#ifndef PLAYER
#define PLAYER

#include "room.h"
#include <string>
#include <iostream>
using namespace std;

class Player
{
  private:
    //Members
    string name;
	Room* location;
	int cheatStatus;
	int bag[16];
	Item* bagItemPtrs[16];

  public:
    //Members
  	int availableHints;
    //Methods
    Player();
	Player(string name);

	void setName(string name);
	void setLocation(Room* nextRoom);
	void setCheatStatus();
	void setItem(int itemId, int isInBag, Item* itemPtr);

	string getName();
	Room* getLocation();
	bool getCheatStatus();
	Item* getBagItem(int itemId);
	int getNumGemsInBag();
	int isInBag(int itemId);
	
	void displayLocation();
	void listBag();
};

#endif
