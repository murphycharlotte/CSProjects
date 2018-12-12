/*
*	File: feature.cpp
*	Authors: Charlotte Murphy, Lauren Reitz, and Yongshi Ye
*	Course: CS 467-400
*	Assignment: Capstone Project
*	Due: March 16, 2018
*	Desc: Feature class header file. Defines Feature object for game.cpp to associate with a
*		Room object. Managed by Game via Room. Accessed by player
*	Usage: For use with main.cpp and feature.cpp*
*/
#ifndef FEATURE
#define FEATURE

#include <iostream>
#include <array>
#include <string>
#include "item.h"
using namespace std;


class Feature 
{
    private:
		// Members
		int id;
		string name;
		string description;
		int unlocked;
		int revealItemFlag;
		Item* revealItemPtr;
		int requireItemsFlag;
		int numRequiredItems;
		int requiredItemIds[4];
		int requireTextFlag;
		string requiredText;
		int requireActionFlag;
		int qtyRequiredAction;
		string requiredAction;
    
    public:
	    // Methods
		Feature();
		Feature(int featureId);

		Feature(int featureId, string desc);

		void setId(int featureId);
		void setDescription(string featureName);
		void setName(string featureName);
		void setUnlocked(int u);
		void setRevealItemFlag(int flag);
		void setRevealItem(Item * itemPtr);
		void setRequireItemsFlag(int flag);
		void setNumRequiredItems(int num);
		void setRequiredItemId(int itemId, int requireIdx);
		void setRequiredTextFlag(int flag);
		void setRequiredText(string text);
		void setRequiredAction(string action, int qty);
		void setQtyRequiredAction(int qty);

		int getId();
		string getName();
		string getDescription();
		int getUnlocked();
		int getRevealItemFlag();
		Item* getRevealedItem();
		int getRevealedItemId();
		int getRequiredItemId(int requireIdx);
		int getNumRequiredItems();
		int getRequireTextFlag();
		string getRequiredText();
		int getRequireActionFlag();
		int getQtyRequiredAction();
		string getRequiredAction();

		int isItemRequired(int itemId);
		int fulfillItemRequirement(int itemId);
		int updateRequireItemFlag();
		void displayDescription();
		int updateUnlocked();
};
#endif

