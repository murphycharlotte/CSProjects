/*
*	File: game.cpp
*	Authors: Charlotte Murphy, Lauren Reitz, and Yongshi Ye
*	Course: CS 467-400
*	Assignment: Capstone Project
*	Due: March 16, 2018
*	Desc: Source file for Game.h Contains definitions for Game class members and methods. The primary
*		members and methods used to play Darkwood castle. The Game class manages game play by managins
*		the subclasses: Room, Feature, Item, Player, and Puzzle. Game interacts with Parser to interpret
*		the user's input.
*	Usage: For use with main.cpp and game.h
*/
#include "game.h"

/*------------------------------------------- Initialize Game -------------------------------------------*/

//Contructor to Start New Game
Game::Game(string playerName, string gameDir) {
    numRooms = 15;
    numItems = 16;
    numFeatures = 30;
    // initialize default rooms and items
    initGame(gameDir, playerName);
    
    // initialize new game player
    //curPlayer = Player(playerName);
    // retrieve default player data
    //loadPlayer(gameDir);
    // set player name per parameter
}


// Function: initGame()
// Sub function of Game constructor
// Loads default instructions from instructions.txt
// inits default puzzles
// For each game object type: Item, Room, Room Connection, and Feature
//   Creates default object(s)
//   Loads object data from respective .txt file
//   Initializes objects per data in .txt file
// INPUT: game data directory as a string, player name as a string
// Pre: In main.cpp, user chooses new or saved game options and enters name or saved game name
// Post: All game objects and variables are initialized with default values of stored values
//    from a saved game.
void Game::initGame(string gameDir, string playerName) {
    // initialize instructions data    
	loadInstructions();

    // initialize default items
    for (int i = 0; i < numItems; i++) {
        gameItems[i] = Item(i);
    }

    // load item data
    loadItems(gameDir);

    // initialize default rooms
    for (int i = 0; i < numRooms; i++) {
        roomMap[i]=Room(i);
    }

    // load room data
    loadRooms(gameDir);

    // load room connection data
    loadConnections(gameDir);

    // initialize default features
    for (int i = 0; i < numFeatures; i++ ) {
        gameFeatures[i] = Feature(i);
    }

    // load feature data
    loadFeatures(gameDir);


	// initialize default player
	curPlayer = Player(playerName);
	
	// load player data
	loadPlayer(gameDir);

	// initialize puzzles
	initPuzzles();
}

// init puzzles
void Game::initPuzzles() {
	for (int i = 1; i <= 2; i++) {
		puzzleArr[i - 1] = Puzzle(i);
		string curPuzzleFile = "puzzle" + to_string(i) + ".txt";
		puzzleArr[i - 1].init(curPuzzleFile);
	}
}



/*------------------------------------------- Load Game Data Functions -------------------------------------------*/

// Function: loadInstructions()
// Sub function of initGame()
// Loads the instuction data. The instruction data does not change if the game
// is a saved game or a new game. The directory is hard coded relative the the current working
// directory to point to the data file. Therefore, only a LOAD instructions function is necessary,
// and a SAVE instructions functions is not needed
int Game::loadInstructions() {
	string gameDir = "../data/";
	string objectivesTxt, featuresTxt, itemsTxt, navigationTxt, tipsTxt;

	// build path and filename
	string fileName = gameDir + "instructions.txt";
	// open file & verify file pointer
	ifstream instructionsFile;
	instructionsFile.open(fileName);

	if (!instructionsFile.is_open()) {
		cout << "Could not open file \"" << fileName << "\"" << endl;
		return 0;
	}

	// objectivesTxt
	if (getline(instructionsFile, objectivesTxt)) {
		instructionsObjectives = objectivesTxt;
	}
	else { cout << "Error reading objectives line from " << fileName << endl; return 0; }


	// features
	if (getline(instructionsFile, featuresTxt)) {
		instructionsFeatures = featuresTxt;
	}
	else { cout << "Error reading features line from " << fileName << endl; return 0; }

	// items
	if (getline(instructionsFile, itemsTxt)) {
		instructionItems = itemsTxt;
	}
	else { cout << "Error reading items line from " << fileName << endl; return 0; }

	// navigation
	if (getline(instructionsFile, navigationTxt)) {
		instructionsNavigation = navigationTxt;
	}
	else { cout << "Error reading navigation line from " << fileName << endl; return 0; }

	// tips
	if (getline(instructionsFile, tipsTxt)) {
		instructionsTips = tipsTxt;
	}
	else { cout << "Error reading tips line from " << fileName << endl; return 0; }
	
	return 1;
}


// Functions: loadItems()
// Sub function of initGame()
// Load item data from gameDir/items.txt file:
// Initializes Game Item objects stored in Item gameItems[16]
// For 0 - 15, the order in which the Rooms were initialized in
// initGame() became each Item's id, which corresponds the specific file 
// lines that store each item's name and description
// Values read from file: Item name, Item description
int Game::loadItems(string gameDir) {
    // build path and filename
    string fileName = gameDir + "items.txt";
    // open file & verify file pointer
    ifstream itemsFile;
    itemsFile.open(fileName);
    
    if (!itemsFile.is_open()) {
        cout << "Could not open file \"" << fileName << "\"" << endl;
        return 0;
    }
    
    // Read lines one at a time and save strings read as item names
    // iterator i corresponds to gameItems[] index and respective item id
    string itemName;
	string itemDesc;
    int itemsLoaded = 0;
	// read lines from items.txt and initialize corresponding Item object stored at gameItems[i]
    for (int i = 0; i < numItems; i++) {
		// item name
        if (getline(itemsFile, itemName)) {
			gameItems[i].setName(itemName);	
			// item description
			if (getline(itemsFile, itemDesc)) {
				gameItems[i].setDescription(itemDesc);
				itemsLoaded++;
			}
			// if getline was unsuccessful and all of the items have not been retrieved
			else { cout << "Error reading item desc from item #" << i << "  fileName: " << fileName << endl; return 0; }
        }
        // if getline was unsuccessful and all of the items have not been retrieved
        else { cout << "Error reading item name from " << fileName << endl; return 0; }
    }
    
    // close file and verify the correct number of lines were successfully read
    itemsFile.close();
  
	if (itemsLoaded != numItems) { cout << "Error: number of items loaded != numItems\n"; return 0; }

	return 1;
}


// Functions: loadRooms()
// Sub function of initGame()
// Initializes Game Room objects stored in Room roomMap[15]
// For 0 - 14, the order in which the Rooms were initialized 
// in initGame() became each Room's id, which directly correspond
// to each Room's data file.
// For n = 0 - 14, Load room data from gameDir/n.txt file:
// Values read from file: Line 1 - Room name,  Line 2 -long description, 
// Line 3, short description, Lines 4-19 correspond to Items 0-15 and
// contain 1 or 0 if a specific item is available in the room or not,
// and line 20 - isVisited, a value the denotes if a player has visited
// the room or not.
int Game::loadRooms(string gameDir) {
    
    for (int i = 0; i < numRooms; i++) {
        //build file name: directory + value of i + '.txt'
        string line;
        string fileName = gameDir;
        fileName += to_string(i);
        fileName += ".txt";
        
        // open file, read only & verify file pointer
        ifstream roomFile;
        roomFile.open(fileName);
        if (!roomFile.is_open()) { cout << "Could not open " << fileName; return 0; }
        
        // Room id
        roomMap[i].setID(i);
        
        // Room name
        if (getline(roomFile, line)) {
            roomMap[i].setName(line);
        }
        else { cout << "getline error"; return 0; }
        
        // long description
        if (getline(roomFile, line)) {
            roomMap[i].setLongDesc(line);
        }
        else { cout << "getline error"; return 0; }
        
        // short description
        if (getline(roomFile, line)) {
            roomMap[i].setShortDesc(line);
        }
        else { cout << "getline error"; return 0; }
        
        // Room contents
        // j is the item index that also corresponds to game Items' Ids
        // values read should be either 0 for not in room/not available
        // or 1 for is in room/ is available
        int isInRoom;
        Item* tempItemPtr;
        for (int j = 0; j < numItems; j++) {
            if (roomFile >> isInRoom) {
                tempItemPtr = &(gameItems[j]);
                if (isInRoom != 1) {
                    isInRoom = 0;
                }
                roomMap[i].setRoomContents(j, isInRoom, tempItemPtr);
            }
            else { cout << "Failed to read room contents from " << fileName << endl; return 0; }
        }
        
        // Room visited
        int isVisited;
        if (roomFile >> isVisited) {
            roomMap[i].setVisited(isVisited);
        }
        else { cout << "Failed to read isVisited from " << fileName << endl; return 0; }
        
        // close file
        roomFile.close();
        
    } // end room files for-loop
    
    return 1;
}


// Functions: loadConnections()
// Sub function of initGame()
// Initializes each game Room's connections to other Rooms 
// respresented by Room pointers stored in Room::Room* connections[4]
//   Room:connections[4] stores respresents room connections in each direction,
//   North, East, South, West. If a connection is valid, it gets a Room pointer
//   If a connection is not valid, it gets a nulptr
// For i = 0 - 14, each of the i lines read, each begins with a roomid, i,
// an the remaining 4 numbers on the line are saved to Room::connection[0 - 3]
// and represent either another room id (valid connection) or -1 (no valid connections)
// Values read: [current room id] [connection value] [connection value] [connection value] [connection value]
int Game::loadConnections(string gameDir) {
    // Loop to read room connections
    //build file name: directory + value of i + '.txt'
    int roomId, roomConnection;
    string line;
    string fileName = gameDir + "connections.txt";
    
    // open file, read only & verify file pointer
    ifstream connectionFile;
    connectionFile.open(fileName);
    if (!connectionFile.is_open()) {
        cout << "Could not open connections file ";
        return 0;
    }
    
    // get one line of ints at a time
    while (getline(connectionFile, line)) {
        // convert string line to a stream of characters
        istringstream iss(line);
        // first int is the room id
        if (iss >> roomId) {
            // remaining 4 ints are connecting rooms ids
            for (int i = 0; i < 4; i++) {
                if (iss >> roomConnection) {
                    // if -1, connection is set to null
                    if (roomConnection < 0) {
                        roomMap[roomId].setConnection(i, nullptr);
                    }
                    // else connection points to room stored at roomMap[roomConnection]
                    else {
                        roomMap[roomId].setConnection(i, &roomMap[roomConnection]);
                    }
                }
                // if expected number cannot be read from iss
                else {
                    return 0;
                }
            }
        }
        // if expected number cannot be read from iss
        else {
            return 0;
        }
    }
    
    connectionFile.close();
    
    // return 1 for successful import of connection data
    return 1;
}


// Function: loadFeatures()
// Sub function of initGame()
// For 0 - 30, The order in which initGame initialized each Feature in Feature gameFeatures[30]
// became each Feature's id number
// Each Room stores Features pointers in Feature* roomFeatures[2]
// For each Room 0 - 14 in Game::roomMap[]
//   For each room Feature 0 - 1
//   one line is read for each of these values:
//   Feature name, feature description, unlocked status (0 or 1), 
//   4 ints for required item ids, required text flag, (if flag) required text,
//   required action flag, (if flag) required action
// **Note Actions were changed to commands 
int Game::loadFeatures(string gameDir) {
	string line;
	string fileName = gameDir + "features.txt";

	// open file, read only & verify file pointer
	ifstream featureFile;
	featureFile.open(fileName);
	if (!featureFile.is_open()) { cout << "Could not open connections file "; return 0; }

	Feature* featurePtr;
	int gameFeatureIds[2];	// holds feature id to reference gameFeatures[], 
							// gameFeature[] index positions correspond to feature ids
	
	// For each Room
	for (int i = 0; i < numRooms; i++) {
		// set gameFeatures ids relative to room id
		// each room gets two features - room[0] gets feature 0 and 1, room[1] gets feature 2 and 3, etc
		// index j references this array
		gameFeatureIds[0] = i * 2;
		gameFeatureIds[1] = i * 2 + 1;

		// Initialize 2 room features
		for (int j = 0; j < 2; j++) {
			// set first feature pointer to point to feature object stored in gameFeatures[]
			// feature gameFeatureIdx[j == 0] should == i *2 [0, 2, 4, 6, etc] and 
			// gameFeatureIdx[j == 1] should == i *2 + 1 [1, 3, 5, 7, etc]
			featurePtr = &gameFeatures[gameFeatureIds[j]];

			// name
			// read name line from file (string) and set feature name
			if (getline(featureFile, line)) {
				featurePtr->setName(line);
			}
			else { cout << "getline error " << endl; return 0; }

			// description
			// read line from file (string) and set feature description
			if (getline(featureFile, line)) {
				featurePtr->setDescription(line);
			}
			else { cout << "getline error " << endl; return 0; }

			// int unlocked
			// read line from file (int) and set feature unlocked value
			if (getline(featureFile, line)) {
				int unlocked;
				istringstream issUnlocked(line);
				if (issUnlocked >> unlocked) {
					featurePtr->setUnlocked(unlocked);
				}
				else { cout << "string stream error " << endl; return 0; }
			}
			else { cout << "getline error " << endl; return 0; }


			// Item* revealItem
			// Read line from file (int) and set revealItem
			if (getline(featureFile, line)) {
				int itemId;
				istringstream issRevItem(line);

				// item id used to initialize reveal item pointer
				// setRevealItem() updates revealItemFlag relative to Item pointer 
				// if itemPointer == nullptr, flag = 0, else, flag = 1
				if (issRevItem >> itemId) {
					// if invalid item id, set item pointer to null
					if (itemId < 0 || numItems <= itemId) {
						featurePtr->setRevealItem(nullptr);
					}
					// set item pointer to point to relative game item
					// revealItemFlag updated by setRevealItem()
					else {
						featurePtr->setRevealItem(&gameItems[itemId]);
					}
				}
				else { cout << "string stream error " << endl; return 0; }
			}
			else { cout << "getline error " << endl; return 0; }

			// requiredItemIds[]
			// Read line from file (int int int int) and set values for each value in requiredItemIds[0 .. 3]
			// numRequiredItems and requireItemsFlag updated within setRequiredItem()
			if (getline(featureFile, line)) {
				istringstream issReqItem(line);
				//cout << "Game::loadFeatures():";
				int tempItemId;
				for (int k = 0; k < 4; k++) {
					if (issReqItem >> tempItemId) {
						featurePtr->setRequiredItemId(tempItemId, k);
					}
					else { cout << "string stream error " << endl; return 0; }
				}
			}
			else { cout << "getline error " << endl; return 0; }

			// requireTextFlag and required text
			// read line from file (int string) and set requiredTextFlag
			// set requiredText if flag == 1
			if (getline(featureFile, line)) {
				int textFlag;
				istringstream issReqText(line);

				if (issReqText >> textFlag) {
				}
				else { cout << "string stream error " << endl; return 0; }

				// requireTextFlag is initialized to 0. Only need to change it if flag > 0
				// setRequiredText() updated the value of the flag relative to value of string
				// if stringrequiredText is empty or not
				if (textFlag > 0) {
					if (getline(featureFile, line)) {
						featurePtr->setRequiredTextFlag(textFlag);
						featurePtr->setRequiredText(line);
					}
					else { cout << "getline error " << endl; return 0; }
				}
			}
			else { cout << "getline error " << endl; return 0; }

			// qtyRequiredAction and requiredAction
			// read line from file (int string) and set qtyRequired and requiredAction
			if (getline(featureFile, line)) {
				string reqAction;
				int qtyReq;
				istringstream issReqAct(line);
				
				if (issReqAct >> qtyReq) {
					// qtyRequiredAction initialized to default 0 - only need to change if qty > 0
					// setRequiredAction updated flag values relative to action and qty
					if (qtyReq > 0) {
						if (getline(featureFile, line)) {
							featurePtr->setRequiredAction(line, qtyReq);
						}
						else { cout << "getline error " << endl; return 0; }
					}
				}
				else { cout << "string stream error " << endl; return 0; }

			}
			else { cout << "getline error " << endl; return 0; }
			
			roomMap[i].setFeature(j, featurePtr);			
		}	
		
		// Test LoadFeature()
		/*
		roomMap[i].displayName();
		displayFeatureData(&gameFeatures[gameFeatureIds[0]]);
		cout << endl;
		displayFeatureData(&gameFeatures[gameFeatureIds[1]]);
		cout << endl;
		*/
	}

	// close file
	featureFile.close();

	// return 1 for successful import of connection data
	return 1;
}


// Function: loadPlayer()
// Sub function of initGame()
// Reads  player gamedir/player.txt
// Values read: // Line 1 - name, Line 2 - location (where the player starts the game),
// lines 3 - 18 initialize the player's inventory - The values read are 1 or 0 whether 
// the item is in the player's bag or not. 
// These lines initialize Player::bag[] with 1 or 0 and Player::bagItemPtrs[] with 
// Item Ptrs or nulptr, respectively. 
int Game::loadPlayer(string gameDir) {
    string fileName = gameDir + "player.txt";
    
    // open file, read only & verify file pointer
    ifstream playerFile;
    playerFile.open(fileName);
    if (!playerFile.is_open()) { cout << "Could not open player file "; return 0; }
    
    // Player name
    string playerName;
	// read and consume first line - name is set based on user input
	// for name of new player or name of saved game
    if (getline(playerFile, playerName)) {
        // do nothing
    }
    else { cout << "Error reading player name from " << fileName << endl; return 0; }
    
    // Player location
    int roomId;
    if (playerFile >> roomId) {
        curPlayer.setLocation(&roomMap[roomId]);
    }
    else { cout << "Error reading player location from " << fileName << endl; return 0; }
    
    // Player inventory
    // j is the item index that also corresponds to game Items' Ids
    // values read should be either 0 for not in room/not available
    // or 1 for is in room/ is available
    int hasItem;
    Item* tempItPtr;
    for (int j = 0; j < numItems; j++) {
        if (playerFile >> hasItem) {
            tempItPtr = &(gameItems[j]);
            if (hasItem != 1) {
                hasItem = 0;
            }
            curPlayer.setItem(j, hasItem, tempItPtr);
        }
        else { cout << j << ": Failed to read bag contents from " << fileName << endl; return 0; }
    }
    
    // close file
    playerFile.close();
    
    return 1;
}



/*-------------------------------------------  Game Engine Functions  -------------------------------------------*/

// runs the game loop, checks and executes player's commands
void Game::startGame(string type){
	std::system("clear");
	cout << "\nWelcome to Darkwood Castle, " << curPlayer.getName() << "! ";

	if (type == "new") {
		cout << "Your adventure begins...\n" << endl;
	}
	if (type != "new") {
		cout << "Back again, " << curPlayer.getName() << "? May you have better luck this time..." << endl;
		cout << "Use the command \"INSTRUCTIONS\" to view the instructions or \n\"HELP\" to see a list of commands.\n" << endl;
	}

	if (type == "new")
    {
        // start new game
    	viewInstructions();
    	
        cout << "\n\n                                                     "<<endl;
        cout << "                               .:        :.              "<<endl;
        cout << "         WILL                  v v      v v              "<<endl;
        cout << "                              ||  v_()_v  ||             "<<endl;
        cout << "         YOU                  ||  |    |  ||             "<<endl;
        cout << "                              ||  |    |  ||             "<<endl;
        cout << "         MAKE                  v  |____|  v              "<<endl;
        cout << "                                x_x-||-x_x               "<<endl;
        cout << "         IT                         ||                   "<<endl;
        cout << "                                    ||                   "<<endl;
        cout << "         OUT                        ||                   "<<endl;
        cout << "                                    xx                   "<<endl;
        cout << "         ALIVE??                    xx                   "<<endl;
        cout << "                                    xx                   "<<endl;
        cout << "                                    xx                   "<<endl;
        cout << "                                    vv                   "<<endl;
        cout << "                                    ,'                   "<<endl;
        cout << "                                                         "<<endl;
        cout << "    DARKWOOD CASTLE GAME START                           "<<endl;
        cout << "        Hit enter to begin                           \n\n"<<endl;

		cin.ignore();
	}

	cin.get();
	
	// Display description of starting room before beginning game loop
	std::system("clear");
	displayRoomDescription();

    do{ 
        cout<<": ";
        getline(cin,userInput);
        p = parse(userInput);   
        // DO NOT DELETE- PARSER TEST. 
		/*
        cout << "This is a test area" << endl;
        cout << "command = " << p.command << endl;
        cout << "length = " << p.command.length() << endl;
        cout << "objectOne = " << p.objectOne << endl;
        cout << "objectOne length = " << p.objectOne.length() << endl;
        cout << "objectTwo = " << p.objectTwo << endl;
        cout << "objectTwo length = " << p.objectTwo.length() << endl;
        */    
		 // cout << "testing: " << p.command << ", " << p.objectOne << ", " << p.objectTwo << endl;
		cout << endl;
        if(p.command == "GO") {     // Move rooms, if valid connection
			move(p.objectOne);
		}
		else if (p.command == "TAKE" && p.objectOne == "RAINBOW GEM") {  // add Rainbow Gem to inventory
			takeRainbowGem();
		}
        else if(p.command =="TAKE") {     // remove item from current room and add it to player's bag
			takeItemInRoom(p.objectOne);
        }
		else if (p.command == "DROP" && p.objectOne == "RAINBOW GEM") {     // remove item from player's bag and add it to current room
			dropRainbowGem(p.objectOne);
		}
        else if(p.command == "DROP") {     // remove item from player's bag and add it to current room
            dropItem(p.objectOne);     
        }	
		else if (p.command == "LOOK AT") {     // Examine a room feature or an item
			lookAt(p.objectOne);
		}
        else if(p.command == "LOOK AROUND"){     // display current room contents
				curPlayer.getLocation()->displayRoomContents();
        }
		else if (p.command == "LOOK" && p.objectOne != ""){     // display current room long description
			cout << "Try \"LOOK AT [item or feature] \" or \"LOOK AROUND\"" << endl;
		}
		else if (p.command == "LOOK") {     // display current room long description
			displayText(curPlayer.getLocation()->getLongDesc());
		}
        else if (p.command == "TURN") {     // Turn Action
			string turn = "Looks like You can't turn anything at the moment. However, the Windlass in the Gatehouse hasn't been turned since the curse was placed on the dragon";
			displayText(turn);
		}
        else if (p.command == "UNLOCK") {     
            if(curPlayer.isInBag(6)){ //Check if player have key
				string unlock = "Looks like You can't unlock anything at the moment. However, the guards in the Gatehouse need to be unlocked to help you escape";
				displayText(unlock);
			}
            else{
                cout<< "You need a Key to try the Unlock Action"<<endl;
            }
        }
        else if (p.command == "CUT") {    
            if(curPlayer.isInBag(1)){ //Check if have Sword 
                string cut = "Looks like You can't use your sword here. However, if the curse is broken, the dragon needs your help to cut his chains";
				displayText(cut);
			}
            else{
                cout<< "You need a Sword to try the Cut Action"<<endl;
            }
        }
		else if (p.command == "MAP") {     // view castle map
			viewMap();
		}
        else if(p.command == "HELP") {     // display available commands
            viewCommands();
        }
		else if (p.command == "INSTRUCTIONS") {     // display instructions
			viewInstructions();
		}
        else if(p.command == "INVENTORY") {     // display items in player's bag/inventory
            displayPlayerBagItems();
        }
        else if(p.command == "EXAMINE") {     // test command room features
			curPlayer.getLocation()->examineRoomFeature(0);
			curPlayer.getLocation()->examineRoomFeature(1);
        }
		else if (p.command == "EXIT") {     // end game
			exitGame();
            break;
		}
		else{
            cout << "I do not understand. \nType \"HELP\" for a list of available commands or \"INSTRUCTIONS\" to see the instructions again" << endl;
        }
		cout << endl;
		if (winGame()) {
			break;
		}
      
    } while (p.command != "EXIT");
}


// Utilizes parser to checks if player wants to save game data before exiting
void Game::exitGame() {
	cout << "Do you want to Save your game? " << endl;
	getline(cin, userInput);
	p = parse(userInput);

	if (p.command == "YES") {
		cout << "Ok. Saving game..." << endl << endl;
		saveGameData("../data/");
	}
	else {
		cout << "Ok. Exit without saving..." << endl << endl;

	}
	cout << "See You Next Time!" << endl;
}


//
bool Game::winGame() {
	if (curPlayer.getLocation()->getId() == 14
		&& roomMap[14].getFeature(0)->getUnlocked() == 0
		&& roomMap[14].getFeature(1)->getUnlocked() == 0) {
		cout << "You DID it, you have successfully Escaped Alive. Congratulations!!!!!" << endl;
		return true;
	}

	return false;
}



/*-------------------------------------------  Help Functions  -------------------------------------------*/

//Print Instructions for Users
void Game::viewInstructions() {
	cout << "Darkwood Castle Instructions:" << endl;
	cout << endl;

	displayText(instructionsObjectives);
	cout << endl;

	displayText(instructionsFeatures);
	cout << endl;

	displayText(instructionItems);
	cout << endl;

	displayText(instructionsTips);
	cout << endl;
}


// displays a list of valid commands
void Game::viewCommands() {
	cout << "Here is a list of commands:    " << endl;
	cout << "   Rooms:" << endl;
	cout << "      LOOK:  View the long description of a room" << endl;
	cout << "      LOOK AT MAP:  View a map of the Darkwood Castle" << endl;
	cout << "   Items:" << endl;
	cout << "      TAKE [item name] " << endl;
	cout << "      DROP [item name] " << endl;
	cout << "      INVENTORY:  View your collected items" << endl;
	cout << "   Features and Items: " << endl;
	cout << "      LOOK AROUND:  View available features and items in a room" << endl;
	cout << "      LOOK AT [item or feature]:  View an item's description or reveal a room feature" << endl;
	cout << "   Navigation:" << endl;
	cout << "      GO [North, East, South, or West]:  travel to an adjacent room" << endl;
	cout << "      GO [room name]:  travel to an adjacent room" << endl;
	cout << "   Game:" << endl;
	cout << "      HELP:  View a list of valid commands" << endl;
	cout << "      INSTRUCTIONS:  View Darkwook Castle instructions" << endl;
	cout << "      EXIT:  Exit Game" << endl;
}


// displays a map of the castle rooms and their connections
void Game::viewMap(){

cout << "                                                                     \n" << endl;
cout << "                 *DUNGEON*       *STABLES*                           \n" << endl;
cout << "                     ^               ^                               \n" << endl;
cout << "                     ^               ^                               \n" << endl;
cout << "                     ^               ^                               \n" << endl;
cout << "                     ^            *MAGE'S*     *GREAT*               \n" << endl;
cout << " *GATEHOUSE* <-> *TREASURY*  <->  *TOWER*  <->  *HALL* <-> *KITCHEN* \n" << endl;
cout << "                                     ^            ^            ^     \n" << endl;
cout << "                                     ^            ^            ^     \n" << endl;
cout << "                                     ^            ^            ^     \n" << endl;
cout << "                                   *THE*       *GRAND*      *DARK*   \n" << endl;
cout << "                                  *SOLAR*  <-> *FOYER* <-> *BALLROOM*\n" << endl;
cout << "                                     ^            ^            ^     \n" << endl;
cout << "                                     ^            ^            ^     \n" << endl;
cout << "                   *COURT*        *ROYAL*      *THRONE*        ^     \n" << endl;
cout << "                    *YARD*  <->  *CHAMBERS* <-> *ROOM*  <-> *ARMORY* \n" << endl;
cout << "                      ^                                              \n" << endl;
cout << "                      ^                                              \n" << endl;
cout << "                      ^                                              \n" << endl;
cout << "                   *GARDEN*                                          \n" << endl;
cout << "                                                                     \n" << endl;
}



/*-------------------------------------------  Player Functions  -------------------------------------------*/

// Displays items in a player's inventory
void Game::displayPlayerBagItems(){
    //Display Items in Player Bag
    curPlayer.listBag();
}


//Given a name, get item id in Player Bag, if not found return -1 
int Game::getPlayerItemId(string itemName) {
	for (int i = 0; i<16; i++) {
		if (curPlayer.isInBag(i)
			&& compareStringIgnoreCase(gameItems[i].getName(), itemName) == 0) {
			return i;
		}
	}
	return -1;
}


//Move Player to different room
void Game::move(string direction) {
	if (direction == "") {
		cout << "Specify a room name or direction. [NORTH,EAST,SOUTH,WEST]" << endl;
	}
	else {
		bool isDirection = false;
		bool cheatStatus = curPlayer.getCheatStatus();
		Room* newLocation = NULL;
		// check if param is a valid direction
		for (int i = 0; i < 4; i++) {   //directions[4] = {"NORTH","EAST","SOUTH","WEST"};
			if (direction == directions[i]) {
				// if the param is a direction, set bool and check if accessbile
				isDirection = true;
				// cheat status doesn't affect this loop, because an invalid direction
				// will still return a nulptr even if cheatStatus

				// if accessible, update newLocation
				if (curPlayer.getLocation()->isAccessible(direction)) {
					newLocation = curPlayer.getLocation()->getConnection(i);
				}
				else {
					cout << "There is no door in that direction. " << endl;
					return;
				}
			}
		}

		// if the param IS NOT a valid direction and IS a valid room
		int newRoomId = getRoomIdByName(direction);
		if (!isDirection && newRoomId != -1) {
			// if newRoomId is the same as current room Id
			if (newRoomId == curPlayer.getLocation()->getId()) {
				cout << "You are already in the " << curPlayer.getLocation()->getName() << "." << endl;
				return;
			}
			// if cheat status and if not the same as current room Id, update newLocation
			else if (cheatStatus) {
				newLocation = &roomMap[newRoomId];
			}
			// if not cheat status and if not the same as current room Id
			else {
				// compare to each connecting roomId
				for (int i = 0; i < 4; i++) {
					if (curPlayer.getLocation()->getConnection(i)) {
						// if newRoomId is a match to a current room connection, update newLoaction pointer
						if (newRoomId == curPlayer.getLocation()->getConnection(i)->getId()) {
							newLocation = curPlayer.getLocation()->getConnection(i);
							break;
						}
						
					}
				}
			}
		}

		// if newLocation is updated
		if (newLocation) {
			//If not cheat status and dragon feature is still unlocked, no access to Guarded Gatehouse
			if (!cheatStatus && newLocation->getId() == 14 && gameFeatures[27].getUnlocked() != 0) {
				cout << "The door to Guarded Gatehouse is Locked, You need Fight the Dragon First!!!" << endl;
			}
			else {
				curPlayer.setLocation(newLocation);
				std::system("clear");
				displayRoomDescription();
			}
		}
		else if(newRoomId != -1){
			cout << "The " << curPlayer.getLocation()->getName() << " is not connected to the " << roomMap[newRoomId].getName() << "." << endl;
		}
		// if input is not a valid direction or a valid room name
		else {
			cout << "I do not understand. \nType \"HELP\" for a list of commands, or \"INSTRUCTIONS\" to view game instructions" << endl;
		}
	}
}



/*-------------------------------------------  Room Functions  -------------------------------------------*/

//Print Room Long Description if first time visted, 
// otherwise, Room short description is displayed
void Game::displayRoomDescription() {
	Room* location = curPlayer.getLocation();
	cout << "--------------------------------------  " << location->getName() << "  -----------------------------------------" << endl;
	cout << endl;
	if (!location->getVisited()) {
		displayText(location->getLongDesc());
		location->setVisited(1);
	}
	else {
		displayText(location->getShortDesc());
	}
}


// Returns pointer to room stored in roomMap, specified by id
Room* Game::getRoom(int roomId) {
	return &roomMap[roomId];
}


// Compares a string to the name of each room in roomMap[]
// if a match is found, the room id is returned,
// else, return -1
int Game::getRoomIdByName(string roomName) {
	string lookingFor = roomName;
	string compareTo;
	int lookingForLen = lookingFor.length();
	transform(lookingFor.begin(), lookingFor.end(), lookingFor.begin(), ::tolower);

	for (int i = 0; i < numRooms; i++) {
		compareTo = roomMap[i].getName();
		transform(compareTo.begin(), compareTo.end(), compareTo.begin(), ::tolower);
		int same = lookingFor.compare(0, lookingForLen, compareTo, 0, lookingForLen);
		if (same == 0) {
			return i;
		}
	}
	return -1;
}



/*-------------------------------------------  Item Functions  -------------------------------------------*/

// checks if string name is an item is in player's bag, and item
// available in the current room, or a feature in the current room
// If an item in the player's bag, a message displayed with item description
// If an available Room item, the description is displayed
// if an available feature, the examineCurRoomFeature function is called// 
void Game::lookAt(string objName) {
	// if objName is an item in the room
	int featureIndex = getCurRoomFeatureIndex(objName);
	//cout << "featureIndex " << featureIndex << endl;
	int itemIndex = getItemIdFromName(objName);
	//cout << "itemIndex " << itemIndex << endl;
	if (featureIndex == 0 || featureIndex == 1) {		// if objName is a feature of the current room
		examineCurRoomFeature(featureIndex);			//     else examine room feature
		return;
	}
	else if (itemIndex != -1) {
		if (curPlayer.isInBag(itemIndex) == 1) {					// if objName is an item in the player's inventory
			cout << "Looking in your bag at the " << gameItems[itemIndex].getName() << ": " << endl;
			displayText(gameItems[itemIndex].getDescription());			//   display message and item description
		}
		else if (curPlayer.getLocation()->isItemInRoom(itemIndex) == 1) {	// if objName is an item available in the current room 
			displayText(gameItems[itemIndex].getDescription());				//     display item description
		}
		else {
			cout << "There is nothing in this room or your bag called " << objName << endl;
		}
	}
	else {
		cout << "There is nothing in this room or your bag called " << objName << endl;
	}

}


//Pick item into bag based on name
int Game::takeItemInRoom(string itemName) {
	int curItemId = getItemInRoom(itemName);	// check if item is available in the room

	bool cheatStatus = curPlayer.getCheatStatus();
	if (cheatStatus) {
		curItemId = getItemIdFromName(itemName);
		curPlayer.setItem(curItemId, 1, &(gameItems[curItemId]));
		//curPlayer.getLocation()->setRoomContents(curItemId, 0, &(gameItems[curItemId]));
		cout << "Item added to your bag: " << itemName << endl;
		return 1;
	}
	else {
		if (curItemId == -1) {	// Ret -1 if not
			cout << "No item found in this room called " << itemName << endl;
			return -1;
		}
		else if (curItemId != -1 && curPlayer.isInBag(curItemId)) {	// error checking statement
			cout << "ERROR. Item is in both Room and Inventory." << endl;	// item should not be available in both
			return -1;																// bag and room
		}
		else { // if item is avaiable, add item to bag, remove it from the room, then ret 1
			curPlayer.setItem(curItemId, 1, &(gameItems[curItemId]));
			curPlayer.getLocation()->setRoomContents(curItemId, 0, &(gameItems[curItemId]));
			cout << "Item added to your bag: " << itemName << endl;
			return 1;
		}
	}
}


// special function for adding Rainbow gem to bag
// set player's cheat status to True
// Note: The rainbow gem is not actually placed in
// any room.
void Game::takeRainbowGem() {
	// add Rainbow Gem to bag, whether if it is already there or not or 
	// whether it has been added before or not
	if (curPlayer.isInBag(15) != 1) {
		curPlayer.setItem(15, 1, &(gameItems[15]));
		curPlayer.setCheatStatus();
		cout << "Rainbow Gem added in your bag." << endl;
	}
	else {
		cout << "The Rainbow Gem is already in your bag." << endl;
	}
}


// drop item from bag based on name
// and place item in the current room
void Game::dropItem(string itemName) {
	// get id of item with name == itemName from
	// player's bag, returns -1 if item not in bag[]
	int curItemId = getPlayerItemId(itemName);
	// if item is not in player's bag
	if (curItemId == -1) {
		cout << "No item found in your bag called " << itemName << endl;
	}
	else {
		// remove item from player's bag
		curPlayer.setItem(curItemId, 0, &(gameItems[curItemId]));
		// add item to current room contents
		curPlayer.getLocation()->setRoomContents(curItemId, 1, &(gameItems[curItemId]));
		cout << "Item dropped: " << itemName << endl;
	}
}


// Removes rainbow gem from player's bag, but does
// not add the rainbow gem to to current room
// sets player's cheat status to False
void Game::dropRainbowGem(string rainbowGem) {
	if (curPlayer.isInBag(15) == 1) {
		curPlayer.setItem(15, 0, &(gameItems[15]));
		curPlayer.setCheatStatus();
		cout << "Rainbow Gem dropped." << endl;
	}
	else {
		cout << "No item found in your bag called " << rainbowGem << endl;
	}
}


//Given a name, get id of item in Room, if not found return -1 
int Game::getItemInRoom(string itemName) {
	Room* roomPtr = curPlayer.getLocation();		// temp Room* for player current location
	int itemId = getItemIdFromName(itemName);		// get item id from item name

	if (roomPtr->isItemInRoom(itemId) == 1) {		// if the item is available in the room
		return itemId;								// return item id
	}

	return -1;										// else, return -1
}


// Given a name, get item id from gameItems[]
int Game::getItemIdFromName(string itemName) {
	string lookingFor = itemName;
	string compareTo;
	int lookingForLen = lookingFor.length();
	transform(lookingFor.begin(), lookingFor.end(), lookingFor.begin(), ::tolower);

	for (int i = 0; i<16; i++) {
		compareTo = gameItems[i].getName();
		transform(compareTo.begin(), compareTo.end(), compareTo.begin(), ::tolower);
		int same = lookingFor.compare(0, lookingForLen, compareTo, 0, lookingForLen);
		if (same == 0) {
			return i;
		}
	}
	return -1;
}



/*-------------------------------------------  Feature Functions  -------------------------------------------*/

// Displays the Feature stored in the feature pointed to by roomFeatures[i]
// checks player's bag for requirements
// checks if text is required, if so, triggers a puzzle
// 
void Game::examineCurRoomFeature(int i) {
	if (i != 0 && i != 1) {
		cout << "Invalid feature" << endl;
		return;
	}

	Feature* curFeature = curPlayer.getLocation()->getFeature(i);
	if (curFeature->getUnlocked() == 0) {
		curFeature->displayDescription();
		return;
	}

	//Free Dragon Feature
	if (curPlayer.getLocation()->getId() == 13 && i == 1) {
		freeDragon();
		return;
	}

	//Move Windlass Feature
	if (curPlayer.getLocation()->getId() == 14 && i == 1) {
		moveWindlass();
		return;
	}
	//Unlock Guards Feature
	if (curPlayer.getLocation()->getId() == 14 && i == 0) {
		unlockShackles();
		return;
	}

	bool meetRequirement = false;

	if (curPlayer.getCheatStatus() //Able to examine GateHouse Feature if at Cheat Staus
		&& compareStringIgnoreCase(curPlayer.getLocation()->getName(), roomMap[14].getName()) == 0) {
		meetRequirement = true;
	}

	else if (curFeature->getRequireTextFlag()) {
		int puzzleIndex = 0;
		if (curFeature->getName() == "musician") {
			cout << "Musician asks you to solve a riddle in exchange for opening the locked box." << endl;
			if (curPlayer.isInBag(5)) { //Check if candle in
				cout << "You light up the Candle, and Look at the riddle" << endl;
				cout << "Let's see if you can can help him solve it." << endl;
				puzzleIndex = 0;
			}
			else {
				cout << "It's too dark here, You can't see riddle clearly, You need a Candle" << endl;
				return;
			}

		}
		else if (curFeature->getName() == "knights") {
			cout << "All the Knights are keep repeating a question. They asked if you know the answer" << endl;
			puzzleIndex = 1;
		}
		meetRequirement = puzzleArr[puzzleIndex].solvePuzzle(&curPlayer.availableHints);
	}
	else if (curFeature->getNumRequiredItems() > 0) {
		meetRequirement = true;
		for (int j = 0; j < curFeature->getNumRequiredItems(); j++) {
			if (!curPlayer.isInBag(curFeature->getRequiredItemId(j))) {
				meetRequirement = false;
				cout << "You need to have " << gameItems[curFeature->getRequiredItemId(j)].getName() << " in your bag" << endl;
			}
		}
	}
	else {
		meetRequirement = true;
	}
	//if all requirements meet, then we can examine the feature
	if (meetRequirement) {
		curPlayer.getLocation()->examineRoomFeature(i);
		curFeature->setUnlocked(0);
	}
}


//Function to check if user is examine room feature
bool Game::isExamineCurrentRoomFeature(string featureName) {
	//Check if user is trying to examine a feature
	//For example, if user type look at picture, this room has a feature called picture. return true, 
	for (int i = 0; i< 2; i++) {
		Feature* curFeature = curPlayer.getLocation()->getFeature(i);
		//cout<< curFeature->getName() <<endl;
		if (featureName.find(curFeature->getName()) != string::npos) {
			return true;
		}
	}
	return false;
}


//Function to get index of feature which user is trying to examine
int Game::getCurRoomFeatureIndex(string featureName) {
	string lookingFor = featureName;
	string compareTo;
	int lookingForLen = lookingFor.length();
	transform(lookingFor.begin(), lookingFor.end(), lookingFor.begin(), ::tolower);

	for (int i = 0; i < 2; i++) {
		compareTo = curPlayer.getLocation()->getFeature(i)->getName();
		transform(compareTo.begin(), compareTo.end(), compareTo.begin(), ::tolower);
		int same = lookingFor.compare(0, lookingForLen - 1, compareTo, 0, lookingForLen - 1);
		if (same == 0) {
			return i;
		}
	}
	return -1;
}


// Special Feature function for the dragon feature in the Dungeon
// can only be called from Room 13, the dungeon, when the player
// uses command "LOOK AT DRAGON"
// requires: sword, at least three gems and armor. 
// Post: Golden Key available in Dungeon Room, Feature unlocked change to 0
void Game::freeDragon() {
	std::system("clear");
	cout << "The Dragon is angry and thrashing around, molten lava and fire spouting " << endl;
	cout << "from his mouth. \"I am trapped here for eternity. Only the king's lost " << endl;
	cout << "treasure can break the curse.\"" << endl;

	//usleep(2000);
	if (curPlayer.getCheatStatus()) {
		cout << endl << "Suddenly the spewing molten rock and fire stop. The dragon sniffs " << endl;
		cout << "as he slowly approaches you. " << endl << endl;

		cout << "He pause and says with awe, \"You carry the most precious treasure of all... " << endl;
		cout << "The mythical Rainbow Gem.The breaker of all curses, and the liberator of " << endl;
		cout << "all enslaved." << endl << endl;

		cout << "You pull out the rainbow gem and lift it high. The curse is broken, and the " << endl;
		cout << "chains are gone!" << endl << endl;
	}
	else {
		//usleep(2000);
		cout << "You need to use the armor to be protected" << endl;
		if (!curPlayer.isInBag(0)) {   //Check Armor 
			cout << "Looks Like You don't have armor to protect yourself. " << endl;
			cout << "You must leave the dungeon." << endl;
			// return player to the treasury? or Foyer? or Game over?
			return;
		}
		cout << "The dragon breathes a large puff of fire. You hold the armor to " << endl;
		cout << "protect yourself. The dragon thrashes, held by chains." << endl;
		cout << "You better have enough of the king's treasure in your bag..." << endl;
		//usleep(2000);

		cout << "You say, \"Let me check...\"" << endl;
		curPlayer.listBag();
		cout << endl;
		int gemCount = 0;
		for (int i = 9; i <= 15; i++) {
			if (curPlayer.isInBag(i))
				gemCount++;
		}
		if (gemCount<3) { //Check if Player Has Three Gems
			cout << "Looks Like You don't have enough Gems lift the curse. You need at Least 3 Gems" << endl;
			cout << "You must leave the dungeon." << endl;
			// return player to treasury? or Foyer? or Game over?
			return;
		}
		cout << "\"I have found gems along the way,\" you say." << endl;
		cout << "\"You hand over a gem of every color. \"" << endl;
		
		cout << "\"I feel so refreshed! Like my old self again!!\" The dragon says. " << endl;
		cout << "His skin glimmers with the sunlight that comes in through the window." << endl;
		cout << "He skips around, like a friendly creature." << endl;
		
		//usleep(3000);
		cout << "\"But I am still bound. How will I get the chains off?\" The dragon says." << endl;
		cout << "\"If you can release me from the chains, I can break through the " << endl;
		cout << "ceiling and show you the way to the gatehouse! The exit is near!\"" << endl;

		cout << "We need to use the Sword to break the chains!" << endl;

		while (1) {
			cout << ": ";
			string userInput;
			getline(cin, userInput);
			ParsedInput userCommand = parse(userInput);

			if (userCommand.command == "EXIT") {
				cout << "Try again next time !!!!";
				return;
			}
			else if (userCommand.command == "CUT" && userCommand.objectOne == "CHAINS") {
				if (curPlayer.isInBag(1)) {
					break;
				}
				else {
					cout << "You need the Sword to free the dragon. " << endl;
					cout << "Find the room that stores weapons to gain what you lack." << endl;
					// return player to the treasury? or Foyer? or Game over?
					return;
				}
			}
			else {
				cout << "That doesn't seem to work. Type \"help\" for a list of commands." << endl;
			}
		}

		cout << "You pull out the sword and lift it high. You swing it down upon the " << endl;
		cout << "chain. It breaks!" << endl;
	}

	curPlayer.getLocation()->examineRoomFeature(1); // examine dragon feature to reveal golden key
	curPlayer.getLocation()->getFeature(1)->setUnlocked(0); // examine dragon feature to reveal golden key
}


// Special Feature funcion to guards' shackles in the gatehouse
// should only be called from Room 14 when the player uses the 
// command: "LOOK AT GUARDS"
// requires: Finsihed Dragon Feature and Golden key
// Post: set Guards Feature unlocked to 0
void Game::unlockShackles() {
	if (!curPlayer.getCheatStatus()) {
		while (1) {
			cout << "You need to unlock the Shackles to free the guards." << endl;
			cout << ": ";
			string userInput;
			getline(cin, userInput);
			ParsedInput userCommand = parse(userInput);

			if (userCommand.command == "EXIT") {
				cout << "Try again next time !!!!";
				return;
			}
			else if (userCommand.command == "UNLOCK" && userCommand.objectOne == "SHACKLES") {
				if (curPlayer.isInBag(6)) {
					break;
				}
				else {
					cout << "You need the Golden Key to unlock the Shackles" << endl;
					cout << "Fight the Dragon to get the key" << endl;
					return;
				}
			}
			else {
				cout << "That doesn't seems to work. Type \"help\" for a list of commands." << endl;
			}
		}
	}
	cout << "You unlocked the Shackles and free the guards" << endl;
	cout << "The guards are so grateful that you freed them, they want to help you exit the GateHouse" << endl;
	cout << "But You need to find a path to the gate and the code to open the gate." << endl;
	curPlayer.getLocation()->getFeature(0)->setUnlocked(0);
}


// Speacial Feature function for the windlass feature in the Gatehouse
// should only be called from Room 14 when the player uses the 
// command: "LOOK AT WINDLASS"
// requires: Finsihed Dragon Feature and Dungeon Code
// Post: set Windlass Feature unlocked to 0, Finished Game
void Game::moveWindlass() {
	if (curPlayer.getLocation()->getFeature(0)->getUnlocked()) {
		cout << "Hmm, You need the help from guards to turn the windlass" << endl;
	}
	else {
		if (!curPlayer.getCheatStatus()) {
			while (1) {
				cout << "You need to Turn the windlass to the exit gate." << endl;
				cout << ": ";
				string userInput;
				getline(cin, userInput);
				ParsedInput userCommand = parse(userInput);

				if (userCommand.command == "EXIT") {
					cout << "Try again next time !!!!";
					return;
				}
				else if (userCommand.command == "TURN" && userCommand.objectOne == "WINDLASS") {
					if (curPlayer.isInBag(7)) {
						break;
					}
					else {
						cout << "You need the Dungeon Code to unlock the door" << endl;
						cout << "Find it in the room which has a locked box" << endl;
						// return player to the treasury? or Foyer? or Game over?
						return;
					}
				}
				else {
					cout << "That doesn't seems to work. Type \"help\" for a list of commands." << endl;
				}
			}
			cout << "You and the guards turn the windlass and lower the drawbridge to the gate" << endl;
			cout << "You used the Dungeon Code open the exit gate. And Left the castle" << endl;
		}
		cout << "You and the guards turn the windlass and lower the drawbridge to the gate" << endl;
		cout << "You used the Dungeon Code open the exit gate. And Left the castle" << endl;
		curPlayer.getLocation()->getFeature(1)->setUnlocked(0);
	}
}



/*------------------------------------------- Save Game Data Functions -------------------------------------------*/
// Code adapted from Charlotte Murphy's submission for CS 344, program 2, the sources
// cited within that program, and the link below:
// https://stackoverflow.com/questions/25670559/opening-a-file-in-append-mode-but-truncating-the-file
// https://stackoverflow.com/questions/347949/how-to-convert-a-stdstring-to-const-char-or-char
// https://www.ntu.edu.sg/home/ehchua/programming/cpp/cp10_IO.html 

// operate in the same methods and loops to produce text files in identical
// format to the default text files, line for line, except containing updated
// values representing the current game state.

// A ../data sub directory is created, if it does not already exits, and named
// <player name>. The player's game data files are then saved in this sub directory
// for future reference. 

// 
void Game::saveGameData(string gameDir) {
	// build directory path
	string savedGameDir = gameDir + curPlayer.getName() + "/";
	// const char* savedGamePtr = new char[savedGameDir.size() + 1];
	// savedGamePtr = savedGameDir.c_str();
	mkdir(savedGameDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	chdir(savedGameDir.c_str());

	saveItemData(savedGameDir);
	saveRoomData(savedGameDir);
	saveConnectionsData(savedGameDir);
	saveFeatureData(savedGameDir);
	savePlayerData(savedGameDir);

}


// 
void Game::saveItemData(string gameDir) {
	// change directory to ../data/<savedGameName>/
	chdir(gameDir.c_str());

	// open file & verify file pointer
	ofstream itemsFile("items.txt", std::ios_base::out | std::ios_base::trunc);
	if (!itemsFile) { cout << "Could not open file \"items.txt\"" << endl; return; }

	// Write item names to file one at a time, one per line
	for (int i = 0; i < numItems; i++) {
		if (!(itemsFile << gameItems[i].getName() << '\n')) {
			cout << "Could not write to file \"items.txt\"" << endl; return;
		}
		if (!(itemsFile << gameItems[i].getDescription() << '\n')) {
			cout << "Could not write to file \"items.txt\"" << endl; return;
		}
	}

	// close file
	itemsFile.close();

	return;
}


// 
void Game::saveRoomData(string gameDir) {

	// change directory to ../data/<savedGameName>/
	chdir(gameDir.c_str());

	for (int i = 0; i < numRooms; i++) {
		// build path and filename
		string fileName = to_string(i);
		fileName += ".txt";

		// open file for writing, if it already existed, overwrite prev contents
		ofstream roomOut(fileName.c_str(), std::ios_base::out | std::ios_base::trunc);
		// verify file pointer
		if (!roomOut) { cout << "Could not open file \"" << fileName << "\"" << endl; return; }

		// Room name
		roomOut << roomMap[i].getName() << '\n';

		// long description
		roomOut << roomMap[i].getLongDesc() << '\n';

		// short description
		roomOut << roomMap[i].getShortDesc() << '\n';

		// room contents
		// j is the item index that also corresponds to game Items' Ids
		// values read should be either 0 for not in room/not available
		// or 1 for is in room/ is available
		//Item* tempItemPtr;
		for (int j = 0; j < numItems; j++) {
			roomOut << roomMap[i].isItemInRoom(j) << '\n';
		}

		// is visited
		roomOut << roomMap[i].getVisited() << '\n';

		// close file
		roomOut.close();
	}

	return;

}


// 
void Game::saveConnectionsData(string gameDir) {

	// change directory to ../data/<savedGameName>/
	chdir(gameDir.c_str());

	// build path and filename
	string fileName = "connections.txt";

	// open file for writing, if it already existed, overwrite prev contents
	ofstream connectionsOut(fileName.c_str(), std::ios_base::out | std::ios_base::trunc);
	// verify file pointer
	if (!connectionsOut) { cout << "Could not open file \"" << fileName << "\"" << endl; return; }

	// one line for each room
	// roomId connections[0].roomId ... connections[3].roomId
	for (int i = 0; i < numRooms; i++) {
		connectionsOut << roomMap[i].getId() << " ";
		for (int j = 0; j < 4; j++) {
			// writes room id for valid connection or -1 for invalid connection
			connectionsOut << roomMap[i].getConnectionId(j) << " ";
		}
		connectionsOut << '\n';
	}

	// close file
	connectionsOut.close();
}


//
void Game::savePlayerData(string gameDir) {

	// change directory to ../data/<savedGameName>/
	chdir(gameDir.c_str());

	string fileName = "player.txt";

	ofstream playerFile(fileName.c_str(), std::ios_base::out | std::ios_base::trunc);
	// verify file pointer
	if (!playerFile) { cout << "Could not open file \"" << fileName << "\"" << endl; return; }

	// player name
	playerFile << curPlayer.getName() << '\n';

	// player location
	playerFile << curPlayer.getLocation()->getId() << '\n';

	// player inventory
	for (int i = 0; i < numItems; i++) {
		playerFile << curPlayer.isInBag(i) << '\n';
	}

	playerFile.close();

	return;
}


//
void Game::saveFeatureData(string gameDir) {
	string test;
	int gameFeatureIds[2];	// holds feature id to reference gameFeatures[], 
	chdir(gameDir.c_str()); // change directory to ../data/<savedGameName>/

	string fileName = "features.txt";

	ofstream featuresFile(fileName.c_str(), std::ios_base::out | std::ios_base::trunc);
	// verify file pointer
	if (!featuresFile) { cout << "Could not open file \"" << fileName << "\"" << endl; return; }

	// for each room
	for (int i = 0; i < numRooms; i++) {
		gameFeatureIds[0] = i * 2;
		gameFeatureIds[1] = i * 2 + 1;
		// for each feature
		for (int j = 0; j < 2; j++) {
			// name
			featuresFile << gameFeatures[gameFeatureIds[j]].getName() << '\n';

			// description
			featuresFile << gameFeatures[gameFeatureIds[j]].getDescription() << '\n';

			// unlocked
			featuresFile << gameFeatures[gameFeatureIds[j]].getUnlocked() << '\n';

			// reveal item id
			featuresFile << gameFeatures[gameFeatureIds[j]].getRevealedItemId() << '\n';

			// require item ids
			for (int k = 0; k < 4; k++) {
				featuresFile << gameFeatures[gameFeatureIds[j]].getRequiredItemId(k) << " ";
			}
			featuresFile << '\n';
			// cout << endl;

			// require text flag
			featuresFile << gameFeatures[gameFeatureIds[j]].getRequireTextFlag() << '\n';

			// if require text flag > 0, required text
			if (gameFeatures[i + j].getRequireTextFlag() > 0) {
				featuresFile << gameFeatures[gameFeatureIds[j]].getRequiredText() << '\n';
				// cout << gameFeatures[gameFeatureIds[j]].getRequiredText() << endl;
			}

			// qty action
			featuresFile << gameFeatures[gameFeatureIds[j]].getRequireActionFlag() << '\n';

			// if qty action > 0, required action
			if (gameFeatures[i + j].getRequireActionFlag() > 0) {
				featuresFile << gameFeatures[gameFeatureIds[j]].getRequiredAction() << '\n';
			}
		}
	}
	featuresFile.close();
}



/*-------------------------------------------  Utility Functions  -------------------------------------------*/

// Function: displayText()
// Displays text in lines of approx 85 characters at a time, 
// adding a line break at the first " " character encountered after 85 characters
// INPUT: String
// Pre: None 
// Post: Text displayed on the screen with appropriate line breaks
void Game::displayText(string description) {
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
	// print remaining substring when i += 84 increments i past end of string 
	// leaving <85 chars remaining in string 
	cout << description.substr(startPos) << endl;
}


//Utility method to compare string ignore case, if equals, return 0;
int Game::compareStringIgnoreCase(string a, string b) {
	string tempA = a;
	string tempB = b;

	transform(tempA.begin(), tempA.end(), tempA.begin(), ::tolower);
	transform(tempB.begin(), tempB.end(), tempB.begin(), ::tolower);
	return tempA.compare(tempB);
}


//Utility method to check string a contains b ignore case, if contains, return 1;
int Game::containsStringIgnoreCase(string a, string b) {
	string tempA = a;
	string tempB = b;
	transform(tempA.begin(), tempA.end(), tempA.begin(), ::tolower);
	transform(tempB.begin(), tempB.end(), tempB.begin(), ::tolower);
	if (tempA.find(tempB) != string::npos)
		return 1;
	return 0;
}


// for testing
void Game::displayFeatureData(Feature* fPtr) {
	cout << "\tFeature Name: " << fPtr->getName() << endl;
	cout << "\tDescription "; fPtr->displayDescription();
	cout << "\tUnlocked: " << fPtr->getUnlocked() << endl;
	cout << "\tReveal Item Flag: " << fPtr->getRevealItemFlag() << "  Reveal Item:  " << fPtr->getRevealedItemId() << " ";
	if (fPtr->getRevealItemFlag()) { cout << fPtr->getRevealedItem()->getName(); }
	cout << endl;
	cout << "\tNumber of Required Items: " << fPtr->getNumRequiredItems() << ":  ";
	cout << "Required Item Ids: " << fPtr->getRequiredItemId(0) << " " << fPtr->getRequiredItemId(1) << " " << fPtr->getRequiredItemId(2) << " " << fPtr->getRequiredItemId(3) << " " << endl;
	cout << "\tRequired Text Flag: " << fPtr->getRequireTextFlag() << ":  ";
	if (fPtr->getRequireTextFlag() > 0) { cout << fPtr->getRequiredText(); }
	cout << endl;
	cout << "\tRequired Action Flag: " << fPtr->getRequireActionFlag() << ":  ";
	if (fPtr->getRequireActionFlag() > 0) { cout << fPtr->getRequiredAction(); }
	cout << endl;

}


//Simple Parser spilit user input to stringa array
vector<string> Game::explode(string str) {
	vector<string> v;
	stringstream ss(str);
	string temp;
	while (ss >> temp) {
		v.push_back(temp);
	}
	return v;
}