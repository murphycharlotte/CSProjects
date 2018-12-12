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

ParsedInput parse(std::string );

#endif


