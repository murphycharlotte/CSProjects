/*
*	File: room.h
*	Authors: Charlotte Murphy, Lauren Reitz, and Yongshi Ye
*	Course: CS 467-400
*	Assignment: Capstone Project
*	Due: March 16, 2018
*	Desc: Room class header file. Defines Room objects. Rooms contain connections to other room
*		objects, room features, and room contents. Managed by Game, accessed by player.
*	Usage: For use with main.cpp and room.cpp
*
*/

#ifndef ROOM
#define ROOM

#include "feature.h"
#include "item.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <array>
#include <string>
using namespace std;
const int ITEM_ARR_SIZE = 16;

class Room
{
private:
    //Members
    int id;
    string name;
    string longDescription;
    string shortDescription;
    int roomContents[ITEM_ARR_SIZE];
    Item* roomItemPtrs[ITEM_ARR_SIZE];
    Room* connections[4]; // [North,East,South,West];
    int visited;
    
    int numFeatures;
    Feature* roomFeaturePtrs[2];
    
public:
    //Members
    
    //Methods
    Room();
    Room(int i);
    
    void setID(int id);
    void setName(string name);
    void setLongDesc(string longDesc);
    void setShortDesc(string shortDesc);
    void setRoomContents(int itemId, int isInRoom, Item* itemPtr);
	void setVisited(int visited);
	void setConnection(int direction, Room* roomPtr);
	void setFeature(int ptrIndex, Feature * featurePtr);

	int getRoomItemId(int itemId);
	string getRoomItemName(int itemId);
    int getId();
    string getName();
    string getLongDesc();
    string getShortDesc();
    Room* getConnection(int direction);
	int getConnectionId(int doorIdx);
    int getVisited();
	Feature* getFeature(int i);
	Feature* getRoomFeature(int ftrIndex);

	void displayName();
	int isItemInRoom(int itemId);
	bool isAccessible(string);
    void displayRoomContents();
	void displayRoomFeatureNames();
    void examineRoomFeature(int featureNum);

};

#endif

