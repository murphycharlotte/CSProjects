/*
*	File: room.cpp
*	Authors: Charlotte Murphy, Lauren Reitz, and Yongshi Ye
*	Course: CS 467-400
*	Assignment: Capstone Project
*	Due: March 16, 2018
*	Desc: Room class source file. Defines Room objects. Rooms contain connections to other room
*		objects, room features, and room contents. Managed by Game, accessed by player.
*	Usage: For use with main.cpp and room.h
*
*/

#include "room.h"

// Default constructor
Room::Room() {
	id = -1;
	numFeatures = 0;
	visited = 0;
	roomFeaturePtrs[0] = nullptr;
	roomFeaturePtrs[1] = nullptr;
}

Room::Room(int i) {
	id = i;
	numFeatures = 0;
	visited = 0;
	roomFeaturePtrs[0] = nullptr;
	roomFeaturePtrs[1] = nullptr;
}

// set functions
void Room::setID(int id1){
	id = id1;
}

void Room::setName(string name1){
	name = name1;
}

void Room::setLongDesc(string longDesc){
	longDescription = longDesc;
}

void Room::setShortDesc(string shortDesc){
	shortDescription = shortDesc;
}

void Room::setRoomContents(int itemId, int isInRoom, Item* itemPtr) {
	roomContents[itemId] = isInRoom;
	roomItemPtrs[itemId] = itemPtr;
}

// Function: setVisited()
// sets value for int Room::visited
void Room::setVisited(int visited1) {
	visited = visited1;
}

// Function: setConnection()
// Initializes connections Room pointer stored at index Direction
// int 0, 1, 2, 3 representing N, E, S, W
// INPUT: int direction and a Room object pointer
// Pre: A Room object exists
// Post: If the direction idex is valid, the connection at connections[direction]
// gets the value of roomPtr
void Room::setConnection(int direction, Room* roomPtr) {
	if (direction >= 0 && direction < 4) {
		connections[direction] = roomPtr;
	}
	else {
		cout << "Invalid direction index " << endl;
	}
}

// stores a feature pointer at roomFeaturePtrs[ptrIndex]
void Room::setFeature(int ptrIndex, Feature * featurePtr) {
	if (ptrIndex >= 0 && ptrIndex < 2) {
		roomFeaturePtrs[ptrIndex] = featurePtr;
	}
}


// get functions
// returns room feature ptr relative to i
Feature* Room::getFeature(int i){
	if(i<0 || i >1) return nullptr;
	return roomFeaturePtrs[i];
}

// Function: getId()
// returns int Room::id
int Room::getId(){
	return id;
}

// Function: getName()
// returns string Room::name
string Room::getName(){
	return name;
}

// Function: getLongDesc()
// returns Room::longDescription string
string Room::getLongDesc(){
	return longDescription;
}

// Function: getShortDesc()
// returns Room::shortDescription string
string Room::getShortDesc(){
	return shortDescription;
}

// Function: getConnection()
// INPUT: int 0, 1, 2, 3 representing N, E, S, W
// Returns room* stored in Room::connection at the direction index
Room* Room::getConnection(int direction) {
	if (direction >= 0 && direction < 4) {
		return connections[direction];
	}
	else {
		cout << "invalid direction index" << endl;
		return nullptr;
	}
}

// Function: isItemInRoom()
// determines if a specific item is in the Room
// INPUT: Int item id
// Returns 1 if item is found in the room, 0 if not
int Room::isItemInRoom(int itemId) {
	if (0 <= itemId && itemId < ITEM_ARR_SIZE) {
		return roomContents[itemId];
	}
	else {
		return -1;
	}
}

// duplicate function - same as "isItemInRoom()
int Room::getRoomItemId(int itemId) {
	return roomContents[itemId];
}

// Returns the name of an Item object in the room
string Room::getRoomItemName(int itemId) {
	// if the item is present in the room, return name
	if (roomContents[itemId] >= 0) {
		return roomItemPtrs[itemId]->getName();
	}
	// else return an empty string
	else {
		return "";
	}
}

// Function: getConnectionId()
// helper function for saveConnectionsData()
// Returns the Room:id for valid room connections
// INPUT: Int 0, 1, 2, 3 corresponding to N, E, S, W
// return a room id if connected 
// return -1 if not
int Room::getConnectionId(int doorIdx) {
	if (connections[doorIdx] && doorIdx >= 0 && doorIdx < 4) {
		return connections[doorIdx]->getId();
	}
	else {
		return -1;
	}
}

// Function: getVisited()
// returns int Room::visited
int Room::getVisited(){
	return visited;
}


// FunctionL displayName()
// Prints Room name to screen
void Room::displayName() {
	cout << name << endl;
}

// Function: displayRoomContents()
// Iterates through roomContents array, where each index position corresponds to a Game Item id
// if the value at the index position is one, the item is in the room, and the item name is displayed;
// INPUT: None
// Pre: Room object initialized successfully
// Post: The names of items corresponding to roomContents[] index positions with the value 1 are displayed
void Room::displayRoomContents() {
	cout << this->getName() << " Room Contents: " << endl << endl;
	
	displayRoomFeatureNames();
	cout << endl;

	int numItems = 0;
	for (int i = 0; i < 15; i++) {
		// If the item is in the room, 
		if (roomContents[i] == 1) {
			numItems++;
			if (numItems == 1) {
				cout << "  Items available: " << endl;
			}
			// Display item name
			cout << "     " << roomItemPtrs[i]->getName() << endl;
			numItems++;
		}
	}
	if (numItems == 0) {
		cout << "  There aren't any items available in " << this->getName() << " at this time. " << endl;
	}
}

// Function displayRoomFeatures()
// displays the feature name for each of the room's features
// INPUT: None
// Pre: Room object initialized successfully
// Post: Feature name for each feature in the room is displayed
void Room::displayRoomFeatureNames() {
	cout << "  Features available: " << endl;
	for (int i = 0; i < 2; i++) {
		if (roomFeaturePtrs[i] != nullptr) {
			cout << "     " << roomFeaturePtrs[i]->getName() << endl;
		}
	}
}

// determine if a room connection is valid,
// returns a bool value, respectively
bool Room::isAccessible(string direction) {

	if (direction.compare("NORTH")==0) {
		if (connections[0]) {
			return true;
		}
		else
			return false;
	}
	if (direction.compare("EAST") == 0) {
		if (connections[1]) {
			return true;
		}
		else
			return false;
	}
	if (direction.compare("SOUTH") == 0) {
		if (connections[2]) {
			return true;
		}
		else
			return false;
	}
	if (direction.compare("WEST") == 0) {
		if (connections[3]) {
			return true;
		}
		else
			return false;
	}
	return false;
}


// Return a pointer to a room feature
Feature* Room::getRoomFeature(int ftrIndex) {
	if (ftrIndex == 0 || ftrIndex == 1) {
		return roomFeaturePtrs[ftrIndex];
	}
	else {
		return nullptr;
	}
}

// Function: examineRoomFeature()
// INPUT: int featureNum ( either 1 or 2)
// Displayes feature description
// If the feature reveals an item, the item is added to the room
//		contents and the revealItemFlag is changed to 0
void Room::examineRoomFeature(int featureNum) {
	Feature* tempFeaturePtr;

	if ((featureNum == 0 || featureNum == 1) && roomFeaturePtrs[featureNum] != nullptr) {
		tempFeaturePtr = roomFeaturePtrs[featureNum];
	}
	else {
		cout << "Invalid feature number " << endl;
		return;
	}

	tempFeaturePtr->displayDescription();

	// check feature item reveal flag
	if (tempFeaturePtr->getRevealItemFlag() == 1) {
		// if feature reveals an item, get revealed item
		Item* tempItemPtr = tempFeaturePtr->getRevealedItem();
		// add item to room
		cout << "     Look! You discovered an item: " << tempItemPtr->getName() << endl;
		setRoomContents(tempItemPtr->getId(), 1, tempItemPtr);
		// update item reveal flag to 0 to reflect item added to room
		tempFeaturePtr->setRevealItem(nullptr);
	}

}
