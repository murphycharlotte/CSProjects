// beginning format for parser_test.cpp
#include <string>
#include <iostream>
#include "parser.h"

ParsedInput Check(std::string input);

int main()
{
    
    std::string text;
    ParsedInput r;


text = "go north";
r = Check(text);
if (r.command == "GO" && r.objectOne == "NORTH"){
    std::cout << "This is valid. The test has passed." << std::endl;
    }else {
        std::cout << "This is not valid." << std::endl;
        return 0;
    }
    
    
    text = "drink poison";
    r = Check(text);
    if (r.command == "DRINK" && r.objectOne == "POTION"){
        std::cout << "This is valid. The test has passed." << std::endl;
    }else {
        std::cout << "This is not valid." << std::endl;
        return 0;
    }
    
    text = "drink the poison";
    r = Check(text);
    if (r.command == "DRINK" && r.objectOne == "POTION"){
        std::cout << "This is valid. The test has passed." << std::endl;
    }else {
        std::cout << "This is not valid." << std::endl;
        return 0;
    }
    
    text = "attack dragon";
    r = Check(text);
    if (r.command == "ATTACK" && r.objectOne == "DRAGON"){
        std::cout << "This is valid. The test has passed." << std::endl;
    }else {
        std::cout << "This is not valid." << std::endl;
    return 0;
}   
 
    text = "attack dragon with sword";
    r = Check(text);
    if (r.command == "ATTACK" && r.objectOne == "DRAGON"
        && r.preposition == "WITH" && r.objectTwo == "SWORD" ){
        std::cout << "This is valid. The test has passed." << std::endl;
    }else {
        std::cout << "This is not valid." << std::endl;
        return 0;
    }
    
    text = "talk to wizard";
    r = Check(text);
    if (r.command == "TALK" && r.objectOne == "WIZARD"
       ){ 
        std::cout << "This is valid. The test has passed." << std::endl;
    }else {
        std::cout << "This is not valid." << std::endl;
      return 0;
    }
    
    text = "speak to knight";
    r = Check(text);
    if (r.command == "TALK" && r.objectOne == "KNIGHT"){
        std::cout << "This is valid. The test has passed." << std::endl;
    }else {
        std::cout << "This is not valid." << std::endl;
        return 0;
    }
    

    text= "steal gem";
  r = Check(text);
  if (r.command == "STEAL" && r.objectOne == "GEM"){
      std::cout << "This is valid. The test has passed." << std::endl;
  }else {
      std::cout << "This is not valid." << std::endl;
     return 0;
  }
    
    text = "LOOK AT GEM";
    r = Check(text);
    if (r.command == "LOOK AT" && r.objectOne == "GEM"){
        std::cout << "This is valid. The test has passed." << std::endl;
    }else {
        std::cout << "This is not valid." << std::endl;
        return 0;
    }
    
    text = "Save";
    r = Check(text);
    if (r.command == "SAVE" ){
        std::cout << "This is valid. The test has passed." << std::endl;
    }else {
        std::cout << "This is not valid." << std::endl;
        return 0;
    }
    
}
ParsedInput Check(std::string input){
    
    ParsedInput p;
    p = parse(input);
    
    
    if (p.isParsed == true){
        std::cout << input << std::endl;
        std::cout << "Command: " << p.command  << "\t";
        std::cout << "First Object: " << p.objectOne  << std::endl;
        if (p.hasPreposition == true){
            std::cout << "Preposition: " << p.preposition  << "\t";
            std::cout << "Second Object: " << p.objectTwo << std::endl;
        }
    }else {
        std::cout << "Original: " << input << std::endl;
        std::cout << "Parse Error: " << p.error << std::endl;
    }
    
    return p;
    
}
    
