/*
*	File: puzzle.h
*	Authors: Charlotte Murphy, Lauren Reitz, and Yongshi Ye
*	Course: CS 467-400
*	Assignment: Capstone Project
*	Due: March 16, 2018
*	Desc: This is the header file for puzzle.cpp
*	Usage: When player tries to examine a feature, he will need to solve the puzzles first.
*
*/

#ifndef PUZZLE
#define PUZZLE

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class Puzzle {
	private:
		int id;
		string question;
		string answer;
		string hint;

	public:
		string wrongAnswerResponse[3] = {"Your answer is not correct", "You are getting close, Let's try again", "Hmm, that doesn't seems correct"};
		Puzzle();
		Puzzle(int i);
		void init(string);
		bool solvePuzzle(int*);
};

#endif