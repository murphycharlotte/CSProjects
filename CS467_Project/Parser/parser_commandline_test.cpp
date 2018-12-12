
// begining file on Command Line Testing

#include <string>
#include <iostream>
#include "parser.h"

void Check(std::string input);

int main()
{
    
    std::string user_input;
    
    do {
        std::cout << "> ";
        std::getline (std::cin,user_input);
        Check(user_input);
    }while(1);
}

void Check(std::string input){
    
    ParsedInput p;
    p = parse(input);
    
    if (p.isParsed == true){
        std::cout << "Command: " << p.command  << "\t";
        std::cout << "The First Object: " << p.objectOne  << std::endl;
        if (p.hasPreposition == true){
            std::cout << "The Preposition: " << p.preposition  << "\t";
            std::cout << "The Second Object: " << p.objectTwo << std::endl;
        }
    }else {
        std::cout << "An error has been found: " << p.error << std::endl;
    }
    
}
