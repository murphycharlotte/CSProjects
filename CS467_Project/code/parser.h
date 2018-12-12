/*
*	File: parser.h
*	Authors: Charlotte Murphy, Lauren Reitz, and Yongshi Ye
*	Course: CS 467-400
*	Assignment: Capstone Project
*	Due: March 16, 2018
*	Desc: Header file for parser.cpp, defining all commands, directional terms, prepositions
*	Usage:
*
*/

#ifndef PARSER_H
#define PARSER_H


struct ParsedInput{
    bool isParsed;
    std::string error;
    bool hasPreposition;
    std::string command;
    std::string preposition;
    std::string objectOne;
    std::string objectTwo;
};

ParsedInput parse(std::string);

#endif


