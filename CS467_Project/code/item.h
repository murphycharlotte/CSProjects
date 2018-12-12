/*
*	File: item.h
*	Authors: Charlotte Murphy, Lauren Reitz, and Yongshi Ye
*	Course: CS 467-400
*	Assignment: Capstone Project
*	Due: March 16, 2018
*	Desc: Item class header file. Defines Item object for game.cpp. Item id, name, and description.
*		Managed by Game, accessed by Room, Feature, Player
*	Usage: For use with main.cpp and item.cpp
*
*/

#ifndef ITEM
#define ITEM

#include <string>

using namespace std;

class Item {
	private:
		int id;
		string name;
		string description;

	public:
		Item();
		Item(int itemId);
		Item(int itemId, string itemName);

		void setId(int itemId);
		void setName(string itemName);
		void setDescription(string itemDesc);

		int getId();
		string getName();
		string getDescription();
};

#endif
