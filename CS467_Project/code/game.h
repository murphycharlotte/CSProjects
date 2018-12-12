/*
*	File: game.h
*	Authors: Charlotte Murphy, Lauren Reitz, and Yongshi Ye
*	Course: CS 467-400
*	Assignment: Capstone Project
*	Due: March 16, 2018
*	Desc: Header file for game.cpp Contains declarations for Game class members and methods. The primary
*		members and methods used to play Darkwood castle. The Game class manages game play by managins
*		the subclasses: Room, Feature, Item, Player, and Puzzle. Game interacts with Parser to interpret
*		the user's input.
*	Usage: For use with main.cpp and game.cpp
*
*/

#ifndef GAME
#define GAME

#include "item.h"
#include "room.h"
#include "feature.h"
#include "player.h"
#include "parser.h"
#include "puzzle.h"
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <vector>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include <unistd.h>
#include <algorithm>

using namespace std;

const string GAME_END = "GAMEEND";

class Game
{
	private:
		//Members
		int numRooms;
		int numItems;
		int numFeatures;
		string directions[4] = { "NORTH","EAST","SOUTH","WEST" };

		string instructionsObjectives;
		string instructionsFeatures;
		string instructionItems;
		string instructionsNavigation;
		string instructionsTips;

		ParsedInput p;
		string userInput;

		// Game objects
		Player curPlayer;
		Room roomMap[15];
		Item gameItems[16];        
		Feature gameFeatures[30];
		Puzzle puzzleArr[2];

	public:
		// Initialize game function
		Game(string playerName, string gameDir);
		//Game(string savedGameDir);
		void initGame(string gameDir, string playerName);
		void initPuzzles();

		// Load game data functions
		int loadInstructions();
		int loadItems(string gameDir);
		int loadRooms(string gameDir);
		int loadConnections(string gameDir);
		int loadFeatures(string gameDir);
		int loadPlayer(string gameDir);

		// Gameplay functions
		void startGame(string type);
		void exitGame();
		bool winGame();

		// Help functions
		void viewInstructions();
		void viewCommands(); 
		void viewMap();

		// Player functions
		void displayPlayerBagItems();
		int getPlayerItemId(string itemName);
		void move(string direction);

		// Room functions
		void displayRoomDescription();
		Room* getRoom(int roomId);
		int getRoomIdByName(string roomName);

		// Item functions
		void lookAt(string objName); // also used for Features
		int takeItemInRoom(string itemName);
		void takeRainbowGem();
		void dropItem(string itemName);
		void dropRainbowGem(string rainbowGem);
		int getItemInRoom(string itemName);
		int getItemIdFromName(string itemName);

		// Feature functions
		void examineCurRoomFeature(int i);
		bool isExamineCurrentRoomFeature(string featureName);
		int getCurRoomFeatureIndex(string featureName);
		void freeDragon();
		void unlockShackles();
		void moveWindlass();

		// Save game data functions
		void saveGameData(string gameDir);
		void saveItemData(string dataDir);
		void saveRoomData(string dataDir);
		void saveConnectionsData(string dataDir);
		void savePlayerData(string dataDir);
		void saveFeatureData(string gameDir);

		// Utility and test functions
		void displayText(string description);
		int compareStringIgnoreCase(string a, string b);
		int containsStringIgnoreCase(string a, string b);
		void displayFeatureData(Feature* fPtr);
		vector<string> explode(string s);

};

#endif

