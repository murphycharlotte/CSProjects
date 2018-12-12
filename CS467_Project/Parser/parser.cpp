
#include <string>
#include <boost/algorithm/string.hpp>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>
#include "parser.h"


// defining basic commands

ParsedInput  Separate(std::string input);
ParsedInput ValidateInput(ParsedInput p);
std::string Capitalize (std::string str);
bool FindPreposition (std::string input);

ParsedInput  parse(std::string input)
{
    
    std::string cInput;
    boost::trim(input);
    cInput = Capitalize(input);
    
    ParsedInput p;
    p = Separate(cInput);
    
    return p;
}

ParsedInput  Separate(std::string input)
{
    
    
    ParsedInput p;
    p.isParsed = false;
    p.hasPreposition = false;
    
    const char *filterList[] = {
        "THE ",
        " THE",
        "'S",
        ".",
        "!",
        0};
    int x = 0;
    while (filterList[x] != '\0'){
        std::string temp (filterList[x]);
        std::string::size_type r = input.find(temp);
        while (r!= std::string::npos) {
            input.erase(r,temp.length());
            r = input.find(temp, r);
        }
        x++;
    
}
     int nwords = std::count(input.begin(), input.end(), ' ')+1;
    std::string words[nwords];
    int i = 0;
    std::stringstream ssin(input);
    while (ssin.good() && i < nwords){
        ssin >> words[i];
        ++i;
    }
    
    
               
               // ------ INCLUDE created text for map-SEE PARSER.TXT-------
               // Put, Look, Look at, Go, Take, Inventory, Help, Climb, Open, Remove, Talk, Drink, Wear, Steal
               
              std::map<std::string, std::string> m;
               
               m["HELP"] = "HELP";
               m["QUESTION"] = "HELP";
               m["SAVE"] = "SAVE";

               m["NORTH"] = "goDirection"; 
               m["SOUTH"] = "goDirection";
               m["EAST"] = "goDirection";
               m["WEST"] = "goDirection";           

 
               m["INVENTORY"] = "INVENTORY";
               m["BAG"] = "INVENTORY";
               m["DISPLAY"] = "INVENTORY";
              
               m["ATTACK"]="ATTACK"; 
               m["STRIKE"]="ATTACK";
               
               m["TAKE"] = "TAKE";
               m["PICK UP"] = "TAKE";
               m["GRAB"] = "TAKE";
               m["PICKUP"] = "TAKE";
               m["GET"] = "TAKE";
               m["OBTAIN"] = "TAKE";
               m["SECURE"] = "TAKE";
               m["ACQUIRE"] = "TAKE";
               
               m["GO"] = "GO";
               m["GO TO"] = "GO";
               m["GO IN"] = "GO";
               m["MOVE"] = "GO";
               m["TRAVEL"] = "GO";
               m["TRAVEL TO"] = "GO";
               m["HEAD"] = "GO";
               m["HEAD TO"] = "GO";
               m["MOVE"] = "GO";
               m["MOVE TO"] = "GO";
               m["WALK"] = "GO";
               m["WALK TO"] = "GO";
               m["GOING"] = "GO";
               m["MOVING"] = "GO";
               
               m["PUT"] = "PUT";
               m["PLACE"] = "PUT";
               m["DROP"] = "PUT";
               m["LEAVE"] = "PUT";
               
               m["LOOK"] = "LOOK";
               
               m["DRINK"] = "DRINK";
               m["SIP"] = "DRINK";
               m["CONSUME"] = "DRINK";
               m["EAT"] = "DRINK";
               
               m["WEAR"] = "WEAR";
               m["DRESS"] = "WEAR";
               
               m["STEAL"] = "STEAL";
               
               m["TALK"] = "TALK";
               m["TALK TO"] = "TALK";
               m["CONVERSATION"] = "TALK";
               m["SPEAK"] = "TALK";
               m["SPEAK TO"] = "TALK";
               m["CONVERSE"] = "TALK";
               
               m["OPEN"] = "OPEN";
               m["ENTER"] = "OPEN";
               
               m["CLIMB"] = "CLIMB";
               
               m["REMOVE"] = "REMOVE";
               m["DELETE"] = "REMOVE";
               
               m["LOOK AT"] = "LOOK AT";
               m["INSPECT"] = "LOOK AT";
               m["SEARCH"] = "LOOK AT";
               m["LOOK IN"] = "LOOK AT";
               
                  
    
    
    std::string command;
    int endCommand;
    
    
    for(i = 0; i <nwords; i++){
        command.append(words[i]);
        if ( m.find(command) != m.end() ) {
            p.command = m[command];
            p.isParsed = true;
            endCommand = i;
        }
        command.append(" ");
    }
    if(p.isParsed == false){
        p.error = "This command is not valid. " ;
        return p;
    }
    
    if(nwords == endCommand+1){
        
        if (p.command == "goDirection"){
            p.command = "GO";
            p.objectOne = words[0];
            p.isParsed = true;
            return p;
            
        }
        if (p.command != "SAVE" 
            && p.command != "INVENTORY"  && p.command != "HELP"
            && p.command != "STEAL"){
            p.isParsed = false;
            p.error = "No objects have been mentioned. Nothing for action to do!" ;
            return p;
} 


           
    }
    
    int objectOneStart = endCommand+1;
    const char *GamePrepositions[] = {
        "WITH",
        "INTO",
        "IN",
        "ONTO",
        "ON",
        "AT",
        0};
    int s=0;
    int t = 0;
    int objectOneEnd = endCommand+1;
    for(s =objectOneStart ; s <nwords; s++){
        for (t=0;GamePrepositions[t] != '\0';t++){
            if(std::strcmp (words[s].c_str(),GamePrepositions[t] ) == 0){
                p.hasPreposition = true;
                p.preposition = words[s];
                objectOneEnd = s-1;
                break;
            }
            
        }
    }

    
    
               
               std::map<std::string, std::string> n;;
               n["GEM"] = "GEM";
               n["TREASURE"] = "GEM";
               n["COIN"] = "GEM";
               
               n["CANDLE"] = "CANDLE";
               n["FLAME"] = "CANDLE";
               n["LIGHT"] = "CANDLE";
               
    n["WEAPON"] = "SWORD";
    n["SWORD"] = "SWORD";
    n["KNIFE"] = "SWORD";
               
    n["POTION"] = "POTION";
    n["POISON"] = "POTION";
    n["LIQUID"] = "POTION";
               
    n["KEY"] = "KEY";
               
    n["ARMOR"] = "ARMOR";
    n["CLOTHING"] = "ARMOR";
    n["GEAR"] = "ARMOR";

    n["IPAD"] = "IPAD";
    n["BOOK"] = "IPAD";
    n["PAGE"] = "IPAD";

    n["MAGNIFYING_GLASS"] = "MAGNIFYING_GLASS";

 
    if (p.hasPreposition == false){
        int w;
        for(w=objectOneStart ;w<nwords;w++){
          p.objectOne.append(words[w]);
            if (w<nwords-1){
                p.objectOne.append(" ");
            }
        }
        
        
        if ( n.find(p.objectOne) != n.end() ) {
            p.objectOne= n[p.objectOne];
        }
        return p;
    }
    
    int y;
    for(y=objectOneStart ;y<=objectOneEnd;y++){
        p.objectOne.append(words[y]);
        if (y<objectOneEnd-1){
            p.objectOne.append(" ");
        }
    }
    
    
    int z;
    for(z=objectOneEnd+2 ;z<nwords;z++){
        p.objectTwo.append(words[z]);
        if (z<nwords-1){
            p.objectTwo.append(" ");
        }
    }
    
    
    if ( n.find(p.objectOne) != n.end() ) {
        p.objectOne= n[p.objectOne];
    }
    
    if ( n.find(p.objectTwo) != n.end() ) {
        p.objectTwo= n[p.objectTwo];
    }
    
    
    return p;
    
}
 std::string Capitalize (std::string str)
{
boost::to_upper(str);
return str;

}
