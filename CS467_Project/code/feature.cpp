/*
*	File: feature.cpp
*	Authors: Charlotte Murphy, Lauren Reitz, and Yongshi Ye
*	Course: CS 467-400
*	Assignment: Capstone Project
*	Due: March 16, 2018
*	Desc:
*	Usage:
*
*/

#include "feature.h"

// Method definitions
Feature::Feature() {
	revealItemFlag = 0;
	requireTextFlag = 0;
	requireActionFlag = 0;
	for (int i = 0; i < 4; i++) {
		requiredItemIds[i] = -1;
	}
}

Feature::Feature(int featureId) {
	id = featureId;
	revealItemFlag = 0;
	requireTextFlag = 0;
	requireActionFlag = 0;
	for (int i = 0; i < 4; i++) {
		requiredItemIds[i] = -1;
	}
}


Feature::Feature(int featureId, string desc) {
	id = featureId;
	description = desc;
	revealItemFlag = 0;
	requireTextFlag = 0;
	requireActionFlag = 0;
	for (int i = 0; i < 4; i++) {
		requiredItemIds[i] = -1;
	}
}

// set feature id
void Feature::setId(int featureId) {
	id = featureId;
}

// Function to set Feature description string
void Feature::setDescription(string desc) {
	description = desc;
}

// set feature name
void Feature::setName(string featureName) {
	name = featureName;
}

// set unlocked status
void Feature::setUnlocked(int u) {
	unlocked = u;
}

// set reveal item flag
void Feature::setRevealItemFlag(int featureRevealItemFlag) {
	revealItemFlag = featureRevealItemFlag;
}

// set item for feature to reveal
void Feature::setRevealItem(Item* itemPtr) {
	revealItemPtr = itemPtr;
	if (revealItemPtr == nullptr) {
		revealItemFlag = 0;
	}
	else {
		revealItemFlag = 1;
	}
}

// set flag indicating a feature requires and 
void Feature::setRequireItemsFlag(int flag) {
	requireItemsFlag = flag;
}

// set the number of required items
void Feature::setNumRequiredItems(int num) {
	numRequiredItems = num;
}

// Function: setRequiredItem()
// Adds an item id, for valid item requirement, or -1, for no requirement, to requiredItemIds[]
// INPUT: Item id and an integer index value from 0 .. 3
// Pre: Feature object successfuly initialized by loadFeatures()
// Post: Item id at index requireIdx is updated. 
// Number of required items is updated based on number of valid Item ids in requiredItemIds[]
// requireItemsFlag is updated relative to number of required items
void Feature::setRequiredItemId(int itemId, int requireIdx) {
	// if requireIdx value is valid
	if (requireIdx >= 0 && requireIdx < 4) {
		// update value of pointer at position requireIdx
		requiredItemIds[requireIdx] = itemId;
	}
	else {
		cout << "Invalid requiredItems[] index value " << endl;
		return;
	}

	// count number of valid Item ids in requiredItemIds[]
	// numRequiredItems gets total
	int num = 0;
	for (int i = 0; i < 4; i++) {
		if (requiredItemIds[i] >= 0) {
			num++;
		}
	}

	// update requireItemsFlag 
	if (num > 0) {
		setRequireItemsFlag(1);
	}
	else {
		setRequireItemsFlag(0);
	}

	// update number of items required
	setNumRequiredItems(num);
}

// set flag indicating a feature requires text
void Feature::setRequiredTextFlag(int flag) {
	requireTextFlag = flag;
}

// set requried text to unlock feature
void Feature::setRequiredText(string text) {
	requiredText = text;

	// if requiredText string is not empty, requireTextFlag gets 1
	if (!requiredText.empty()) {
		requireTextFlag = 1;
	}
	// if requiredText string is an empty string, requireTextFlag gets 0
	else {
		requireTextFlag = 0;
	}
}

// set required action
void Feature::setRequiredAction(string action, int qty) {
	requiredAction = action;
	qtyRequiredAction = qty;

	// if action string is not empty and qty is valid, >0
	// requireActionFlag gets 1
	if (!requiredAction.empty() && qtyRequiredAction > 0) {
		requireActionFlag = 1;
	}
	// if action string is an empty string or invalid qty, <= 0
	// requireActionFlag gets 0
	else {
		requireActionFlag = 0;
	}
}

// set quantity of actions required
void Feature::setQtyRequiredAction(int qty) {
	qtyRequiredAction = qty;
	
	// if a valid qty, require action
	if (qtyRequiredAction > 0) {
		requireActionFlag = 1;
	}
	else {
		requireActionFlag = 0;
	}
}

// returns feature id
int Feature::getId() {
	return id;
}

// returns feature name
string Feature::getName() {
	return name;
}

// returns feature description
string Feature::getDescription() {
	return description;
}

// returns feature unlocked status
int Feature::getUnlocked() {
	return unlocked;
}

// returns reveal item flag
int Feature::getRevealItemFlag() {
	return revealItemFlag;
}

// returns pointer to revealed item
Item * Feature::getRevealedItem() {
	if (revealItemFlag == 1) {
		return revealItemPtr;
	}
	else {
		return nullptr;
	}
}

// returns id of revealed item
int Feature::getRevealedItemId() {
	if (revealItemPtr != nullptr) {
		return revealItemPtr->getId();
	}
	return -1;
}

// Function: getRequiredItem()
// Returns Item pointer from requiredItemIds[] stored at position requireIdx
// INPUT: Integer index value from 0 .. 3
// Pre: Feature object successfuly initialized by loadFeatures()
// Post: Returns an item pointer (null or valid)
int Feature::getRequiredItemId(int requireIdx) {
	// if index if valid, return pointer ad index pos
	if (requireIdx >= 0 && requireIdx < 4) {
		return requiredItemIds[requireIdx];
	}
	// else, print error, return nullptr
	else {
		cout << "Invalid requiredItems[] index value " << endl;
		return -1;
	}
}

// return number of required items
int Feature::getNumRequiredItems(){
	return numRequiredItems;
}

// returns requried text flag
int Feature::getRequireTextFlag() {
	return requireTextFlag;
}

// returns required text to unlock feature
string Feature::getRequiredText() {
	return requiredText;
}

// returns requried action flag
int Feature::getRequireActionFlag() {
	return requireActionFlag;
}

// returns qty required action
int Feature::getQtyRequiredAction() {
	return qtyRequiredAction;
}

//returns string representing required action
string Feature::getRequiredAction() {
	return requiredAction;
}

// returns 1 if an item is required by a feature, else 0
int Feature::isItemRequired(int itemId) {
	// if param itemId found in id array, return 1
	for (int i = 0; i < 4; i++) {
		if (requiredItemIds[i] == itemId) {
			return 1;
		}
	}
	// else, return 0;
	return 0;
}

// Function: fulfillItemRequired()
// Checks if param item id is required to unlock the feature
// Updates requiredItemIds[] and numItemsRequired, via subfuntion call to setRequiredId()
// INPUT: Item id
// Pre: Feature object successfuly initialized by loadFeatures()
// Post: If param item id was found/required: 
//	Value at requiredItemsIds[i] gets -1 and numRequriedItem reduced by 1
//
// ** How will this interact with game?
// Player enters command put item, parser triggers Game:: fulfillFeatureItemReq()
// that calls Feature::fulfillItemRequirement()
// if fulfillItemRequirement() returns id of matching required item, game removes item from Bag 
// and places it in usedGameItems
// else, displays message
int Feature::fulfillItemRequirement(int itemId) {
	// if an item is item required
	if (numRequiredItems > 0) {
		// if itemId is found in id array,
		for (int i = 0; i < 4; i++) {
			if (requiredItemIds[i] == itemId) {
				setRequiredItemId(-1, i);		// remove item id from id array & decrease numRequired items 
				updateRequireItemFlag();		// update requireItemsFlag if necessary
				return requiredItemIds[i];
			}
		}
	}
	return -1;
}

// Prints a feature description with appropriate line breaks
// to fit within the game window size
void Feature::displayDescription() {
	size_t startPos = 0;
	size_t substrLen;
	// starting at startPos, increment string index += 85, increment index until char at index i == ' '
	// set substring length to the difference between startPos and string index, i
	// From startPos, print substrLen number of chars, then print line break
	for (int i = 85; i < (int)description.length(); i++) {
		if (description[i] == ' ' || description[i] == '\0') {
			substrLen = (size_t)i + 1 - startPos;
			cout << description.substr(startPos, substrLen) << endl;
			startPos += substrLen;
			i += 84; // only 84, bc for-loop increments i when loop restarts
		}
	}
	// print remaining substring when i += 84 increments I past end of string
	// leaving <85 chars remaining in string 
	cout << description.substr(startPos) << endl;
	// cout << description << endl;
}

// Function: updateUnlocked()
// Checked require flags and qty
// if all are 0, feature is unlocked
// else, feature is locked, and item or text is not added to the room
// INPUT: none
// Pre: Feature object successfuly initialized by loadFeatures()
// Post: unlocked value set to 1 or 0 relative to requirement flags & qty
//
// Game calls after calling any fulfill functions
// adds item to 
int Feature::updateUnlocked() {
	if (requireItemsFlag < 1
		&& requireTextFlag < 1
		&& requireActionFlag < 1) {
		unlocked = 1;
	}
	else {
		unlocked = 0;
	}
	return unlocked;
}

int Feature::updateRequireItemFlag() {
	if (numRequiredItems < 1) {
		requireItemsFlag = 0;
	}
	else {
		requireItemsFlag = 1;
	}
	return requireItemsFlag;
}