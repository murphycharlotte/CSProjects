/*
*	File: main.cpp
*	Authors: Charlotte Murphy, Lauren Reitz, and Yongshi Ye
*	Course: CS 467-400
*	Assignment: Capstone Project
*	Due: March 16, 2018
*	Desc: Main function to play Darkwood castle. Give the user the option to start a new game,
*		return to a saved game, or exit. Verifies valid input.
*		A game data directory is determined based on the user's choice of new or saved game.
*	Usage: For use with the following files:
*		feature.h	feature.cpp
*		game.h		game.cpp
*		item.h		item.cpp
*		parser.h	parser.cpp
*		player.h	player.cpp
*		puzzle.h	puzzle.cpp
*		room.h		room.cpp
*/

#include <iostream>
#include <stdlib.h>
#include "game.h"

using namespace std; 

const string GAME_DIR_NAME = "../data/";

int main()
{

    cout << "                                                                              \n \n" << endl;
    cout << "         Welcome to Darkwood Castle                                                " << endl;     
    cout << "                                       .-.                                         " << endl;
    cout << "                                      /___|                                        " << endl;
    cout << "                                      |___|                                        " << endl;
    cout << "                                      |]_[|                                        " << endl;
    cout << "                                      / I |                                        " << endl;
    cout << "                                   JL/  |  |JL                                     " << endl;
    cout << "        .-.                    i   ()   |   ()   i                    .-.          " << endl;
    cout << "        |_|     .^.           /_|  LJ=======LJ  /_|           .^.     |_|          " << endl;
    cout << "     ._/___|._./___|_._._._._.L_J_|.-.     .-.|_L_J._._._._._/___|._./___|._._._   " << endl;
    cout << "            ., |-,-| .,       L_J  |_| [I] |_|  L_J       ., |-,-| .,        .,    " << endl;
    cout << "            JL |-O-| JL       L_J|||||||||||||||L_J       JL |-O-| JL        JL    " << endl;
    cout << "     IIIIII_HH_'-'-'_HH_IIIIII|_|=======H=======|_|IIIIII_HH_'-'-'_HH_IIIIII_HH    " << endl;
    cout << "     -------[]-------[]-------[_]----|.=I=./----[_]-------[]-------[]--------[]-   " << endl;
    cout << "      _/|_  ||||_I_//||  _/|_ [_] []_/_L_J_|_[] [_] _/|_  ||||_I_//||  _/r|  |||   " << endl;
    cout << "      |__|  ||=/_|_|=||  |__|_|_|   _L_L_J_J_   |_|_|__|  ||=/_|_|=||  |__|  ||-   " << endl;
    cout << "      |__|  |||__|__|||  |__[___]__--__===__--__[___]__|  |||__|__|||  |__|  |||   " << endl;
    cout << "     IIIIIII[_]IIIII[_]IIIIIL___J__II__|_|__II__L___JIIIII[_]IIIII[_]IIIIIIII[_]   " << endl;
    cout << "      |_I_/ [_]|_I_/[_] |_I_[_]|II/[]|_|I/_/[]|II/[_]|_I_/ [_]|_I_/[_] |_I_/ [_]   " << endl;
    cout << "     ./   |.L_J/   |L_J./   L_JI  I[]/     |[]I  IL_J    |.L_J/   |L_J./   r.L_J   " << endl;
    cout << "     |     |L_J|   |L_J|    L_J|  |[]|     |[]|  |L_J     |L_J|   |L_J|     |L_J   " << endl;
    cout << "     |_____JL_JL___JL_JL____|-||  |[]|     |[]|  ||-|_____JL_JL___JL_JL_____JL_J   " << endl;
    bool quitGame = false;

    while(!quitGame){
		cout << "\n\n";
	cout << "     -----------------------------  Main Menu  --------------------------------- " << endl;
		cout << "          1. Begin a new game" << endl;
        cout << "          2. Continue with an existing game" << endl;
        cout << "          3. Exit" << endl;
  
        cout << "\nPlease enter your choice" << endl;
		cout << ": ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:{
                cout << endl << "Please Enter Your Name: " << endl;
                string playerName;
				cout << ": ";
				cin >> playerName;
				cout << endl;
				// GAME_DIR_NAME reflect default data directory used for new games
                Game game(playerName,GAME_DIR_NAME);
                game.startGame("new");
                quitGame = true;
                break;
            }
            case 2:{
                cout << endl << "Please Enter Your Saved Game Name (the player name you entered for your saved game):" << endl;
                string gameName; // corresponds to player name of prev game and the name of the folder containing the saved game data
				cout << ": ";
				cin >> gameName;
				// GAME_DIR_NAME + gameName represents the data subdirectory containing the saved game data
                string dataFolder = GAME_DIR_NAME + gameName + "/";
                ifstream f(dataFolder.c_str());
                if(!f.good()){
                    cout<<"There is No Saved Game Called "<< gameName<<endl;
                    break;
                }
                Game game(gameName, dataFolder);
                game.startGame("saved");
				quitGame = true;
                break;
            }
            case 3:
                cout << endl << "See you next time!" << endl;
                quitGame = true;
                break;
            default:
                cout << endl << "Invalid choice. Please try 1, 2, or 3." << endl;
				// clear error flags and input stream before next input
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
        } 
	}

	return 0;
}


//  https://stackoverflow.com/questions/5655142/how-to-check-if-input-is-numeric-in-c
