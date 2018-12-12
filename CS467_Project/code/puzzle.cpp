/*
*	File: feature.cpp
*	Authors: Charlotte Murphy, Lauren Reitz, and Yongshi Ye
*	Course: CS 467-400
*	Assignment: Capstone Project
*	Due: March 16, 2018
*	Desc: For feature that requires text, player needs to solve the puzzles. 
*	Usage: When player tries to examine a feature, he will need to solve the puzzles first.
*
*/

#include "puzzle.h"

// method definitions
Puzzle::Puzzle() { 
}

Puzzle::Puzzle(int i) { 
    id = i;   
}

void Puzzle::init(string filename){
    string path = "../data/"+filename;
    ifstream puzzleFile;
    puzzleFile.open(path);
    
    if (!puzzleFile.is_open()) {
        cout << "Could not open file \"" << path << "\"" << endl;
        return;
    }
    string puzzleQuestion;
    getline(puzzleFile, puzzleQuestion);
    string puzzleHint;
    getline(puzzleFile, puzzleHint);
    string puzzleAnswer;
    getline(puzzleFile, puzzleAnswer);
    question = puzzleQuestion;
    answer = puzzleAnswer;
    hint = puzzleHint;
}


bool Puzzle::solvePuzzle(int * availableHints) {
	srand (time(NULL));
    cout<<"Question:"<<endl;
    cout<< question << endl;
    string userInput;
    while(1){
    	getline(cin, userInput);
    	if(userInput == answer){
            cout<< "You did it!!! Nice Job!!!!" << endl;
    		return true;
    	}
    	else if(userInput == "quit"){
    		cout<< "Try Next Time !!!!";
    		break;
    	}
        else if(userInput == "hint"){
            if(*availableHints>0){
                *availableHints = *availableHints-1;
                cout << "Let me give you some hint!"<<endl;
                cout << hint << endl;
            }
            else{
                cout<< "You have used up all your hints"<<endl;
            }
        }
    	else{
    		cout<< wrongAnswerResponse[rand()%3] << endl;
            cout<< "You can Type quit to leave this feature"<< endl;
    	}
    }
    return false;
}
