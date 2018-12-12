/*
*	File: item.cpp
*	Authors: Charlotte Murphy, Lauren Reitz, and Yongshi Ye
*	Course: CS 467-400
*	Assignment: Capstone Project
*	Due: March 16, 2018
*	Desc: Item class source file. Defines Item object for game.cpp. Item id, name, and description.
*		Managed by Game, accessed by Room, Feature, Player
*	Usage: For use with main.cpp and item.h
*
*/
#include "item.h"

// constructors
Item::Item() {
	id = -1;
}

Item::Item(int itemId) {
	id = itemId;
}

Item::Item(int itemId, string itemName) {
	id = itemId;
	name = itemName;
}

// set item id number
void Item::setId(int itemId) {
	id = itemId;
}

// set item name
void Item::setName(string itemName) {
	name = itemName;
}

// set item description
void Item::setDescription(string itemDesc) {
	description = itemDesc;
}

// return item id number
int Item::getId() {
    return id;
}

// return item name
string Item::getName() {
    return name;
}

// return item description
string Item::getDescription() {
	return description;
}


