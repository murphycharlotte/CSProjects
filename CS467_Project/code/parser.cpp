/*
*	File: parser.cpp
*	Authors: Charlotte Murphy, Lauren Reitz, and Yongshi Ye
*	Course: CS 467-400
*	Assignment: Capstone Project
*	Due: March 16, 2018
*	Desc: Game Commands, prepositonal terms and directional terms 
*	Usage:
*
*/

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
    transform(input.begin(), input.end(), input.begin(), ::tolower);
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
		"THIS",
		" THIS"
        "'S",
        ".",
        "!",
        0};
    int x = 0;
    while (filterList[x] != '\0'){
        std::string temp (filterList[x]);
        std::string::size_type r = input.find(temp);
        while (r != std::string::npos) {
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
	m["QUESTIONs"] = "HELP";
	m["QUESTION"] = "HELP";

	m["INSTRUCTIONS"] = "INSTRUCTIONS";
	m["INSTRUCTION"] = "INSTRUCTIONS";

	m["MAP"] = "MAP";
	m["LAYOUT"] = "MAP";
	m["ROOMS"] = "MAP";
	m["ALL ROOMS"] = "MAP";

    m["NORTH"] = "goDirection"; 
    m["SOUTH"] = "goDirection";
    m["EAST"] = "goDirection";
    m["WEST"] = "goDirection";           
	 
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

	m["LOOK"] = "LOOK";

	m["LOOK AT"] = "LOOK AT";
	m["INSPECT"] = "LOOK AT";
	m["SEARCH"] = "LOOK AT";
	m["LOOK IN"] = "LOOK AT";
	m["VIEW"] = "LOOK AT";

	m["LOOK AROUND"] = "LOOK AROUND";
	m["ROOM CONTENTS"] = "LOOK AROUND";
	m["ROOM ITEMS"] = "LOOK AROUND";

	m["INVENTORY"] = "INVENTORY";
	m["BAG"] = "INVENTORY";
	m["STUFF"] = "INVENTORY";
	m["ITEMS"] = "INVENTORY";

	m["TAKE"] = "TAKE";
	m["PICK UP"] = "TAKE";
	m["GRAB"] = "TAKE";
	m["PICKUP"] = "TAKE";
	m["GET"] = "TAKE";
	m["OBTAIN"] = "TAKE";
	m["SECURE"] = "TAKE";
	m["ACQUIRE"] = "TAKE";
	m["STEAL"] = "TAKE";

    m["DROP"] = "DROP";
	m["LEAVE"] = "DROP";
	m["TAKE OUT"] = "DROP";
	m["REMOVE"] = "DROP";
	m["DELETE"] = "DROP";
	 
	m["CUT"] = "CUT";

	m["UNLOCK"] = "UNLOCK";

	m["TURN"] = "TURN";

	m["YES"] = "YES";
	m["OKAY"] = "YES";
	m["OK"] = "YES";
	m["O.K."] = "YES";
	m["O.K"] = "YES";
	m["YEP"] = "YES";
	m["SI"] = "YES";

    m["QUIT"] = "EXIT";
    m["EXIT"] = "EXIT";
	m["END"] = "EXIT";
	m["STOP"] = "EXIT";

	// just for testing features, not for game play
	m["EXAMINE"] = "EXAMINE";

	/*------------------------------ UNUSED COMMANDS -------------------------------*/
	// can I delete these?

	m["PUT"] = "PUT";
	m["PLACE"] = "PUT";
	m["DRINK"] = "DRINK";
	m["SIP"] = "DRINK";
	m["CONSUME"] = "DRINK";
	m["EAT"] = "DRINK";

	m["WEAR"] = "WEAR";
	m["DRESS"] = "WEAR";
	m["PUT ON"] = "WEAR";

	m["TALK"] = "TALK";
	m["TALK TO"] = "TALK";
	m["CONVERSATION"] = "TALK";
	m["SPEAK"] = "TALK";
	m["SPEAK TO"] = "TALK";
	m["CONVERSE"] = "TALK";

	m["OPEN"] = "OPEN";
	m["ENTER"] = "OPEN";

	m["CLIMB"] = "CLIMB";

	m["ATTACK"] = "ATTACK";
	m["STRIKE"] = "ATTACK";

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
			&& p.command != "STEAL") {
			p.isParsed = false;
			p.error = "No objects have been mentioned. Nothing for action to do!";
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
		"OUT",
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

	/*------------------------------ ROOMS -------------------------------*/
	n["GRAND FOYER"] = "GRAND FOYER";
	n["FOYER"] = "GRAND FOYER";
	n["FOYER"] = "GRAND FOYER";
	n["ENTRY"] = "GRAND FOYER";
	n["ENTRYWAY"] = "GRAND FOYER";
	n["START"] = "GRAND FOYER";
	n["BEGINNING"] = "GRAND FOYER";
	n["FIRST"] = "GRAND FOYER";
	n["STARTING ROOM"] = "GRAND FOYER";
	n["BEGINNING ROOM"] = "GRAND FOYER";
	n["FIRST ROOM"] = "GRAND FOYER";

	n["ROYAL CHAMBERS"] = "ROYAL CHAMBERS";
	n["BEDROOM"] = "ROYAL CHAMBERS";
	n["SLEEPING QUARTERS"] = "ROYAL CHAMBERS";
	n["CHAMBERS"] = "ROYAL CHAMBERS";

	n["THRONE ROOM"] = "THRONE ROOM";

	n["ARMORY"] = "ARMORY";
	n["WEAPONS ROOM"] = "ARMORY";
	n["WEAPON ROOM"] = "ARMORY";

	n["DARK BALLROOM"] = "DARK BALLROOM";
	n["BALLROOM"] = "DARK BALLROOM";

	n["COURTYARD"] = "COURTYARD";

	n["GARDEN"] = "GARDEN";

	n["MAGE'S TOWER"] = "MAGE'S TOWER";
	n["TOWER"] = "MAGE'S TOWER";

	n["GREAT HALL"] = "GREAT HALL";
	n["HALL"] = "GREAT HALL";

	n["KITCHEN"] = "KITCHEN";
	n["GALLEY"] = "KITCHEN";

	n["SOLAR"] = "THE SOLAR";

	n["STABLES"] = "STABLES";

	n["TREASURY"] = "TREASURY";

	n["DUNGEON"] = "DUNGEON";
	
	n["GATE"] = "GATE";
	n["EXIT GATE"] = "GATE";


	/*------------------------------ GAME ITEMS -------------------------------*/
	n["BLUE GEM"] = "BLUE GEM";
	n["BLUE"] = "BLUE GEM";
	n["BLUEGEM"] = "BLUE GEM";

	n["GREEN GEM"] = "GREEN GEM";
	n["GREEN"] = "GREEN GEM";
	n["GREENGEM"] = "GREEN GEM";

	n["PURPLE GEM"] = "PURPLE GEM";
	n["PURPLE"] = "PURPLE GEM";
	n["PURPLEGEM"] = "PURPLE GEM";

	n["RED GEM"] = "RED GEM";
	n["RED"] = "RED GEM";
	n["REDGEM"] = "RED GEM";

	n["WHITE GEM"] = "WHITE GEM";
	n["WHITE"] = "WHITE GEM";
	n["WHITEGEM"] = "WHITE GEM";

	n["YELLOW GEM"] = "YELLOW GEM";
	n["YELLOW"] = "YELLOW GEM";
	n["YELLOWGEM"] = "YELLOW GEM";

	n["RAINBOW GEM"] = "RAINBOW GEM";	// grants player "cheat status"
	n["RAINBOW"] = "RAINBOW GEM";
	n["RAINBOWGEM"] = "RAINBOW GEM";
               
    n["CANDLE"] = "CANDLE";
    n["FLAME"] = "CANDLE";
    n["LIGHT"] = "CANDLE";
               
    n["WEAPON"] = "SWORD";
    n["SWORD"] = "SWORD";
    n["KNIFE"] = "SWORD";
               
    n["POTION"] = "POSION";
    n["POISON"] = "POSION";
    n["LIQUID"] = "POSION";
               
	n["GOLDEN KEY"] = "GOLDEN KEY";
	n["KEY"] = "GOLDEN KEY";
               
    n["ARMOR"] = "ARMOR";
    n["CLOTHING"] = "ARMOR";
    n["GEAR"] = "ARMOR";

    n["IPAD"] = "IPAD";
    n["BOOK"] = "IPAD";
    n["PAGE"] = "IPAD";

    n["MAGNIFYING GLASS"] = "MAGNIFYING GLASS";
	n["MAGNIFYING"] = "MAGNIFYING GLASS";


	/*------------------------------ GAME FEATURES -------------------------------*/
	n["PAINTING"] = "PAINTING";
	n["PICTURE"] = "PAINTING";
	n["ARTWORK"] = "PAINTING";
	n["ART"] = "PAINTING";

	n["CHEST"] = "CHEST";
	n["WOODEN CHEST"] = "CHEST";
	n["TREASURE CHEST"] = "CHEST";
	n["BOX"] = "CHEST";
	n["SAFE"] = "CHEST";

	n["CHANDELIER"] = "CHANDELIER";
	n["LIGHT FIXTURE"] = "CHANDELIER";

	n["BED"] = "BED";
	n["FOUR POSTER BED"] = "BED";
	n["BIG BED"] = "BED";
	n["KING SIZE BED"] = "BED";

	n["THRONE"] = "THRONE";
	n["TARNISHED THRONE"] = "THRONE";
	n["KING'S THRONE"] = "THRONE";

	n["DOG"] = "DOG";
	n["WOLF HOUND"] = "DOG";
	n["WOLFHOUND"] = "DOG";
	n["PUPPY"] = "DOG";
	n["KING DOG"] = "DOG";
	n["OLD DOG"] = "DOG";
	n["BIG DOG"] = "DOG";

	n["SILVER"] = "SILVER";
	n["SILVER WALLS"] = "SILVER";
	n["SILVER FLOOR"] = "SILVER";
	n["SHINY WALLS"] = "SILVER";
	n["SHINY FLOOR"] = "SILVER";
	n["METALIC WALLS"] = "SILVER";
	n["METALIC FLOOR"] = "SILVER";
	
	n["KNIGHT"] = "KNIGHT";
	n["TALKING KNIGHT"] = "KNIGHT";

	n["CHAIR"] = "CHAIR";
	n["CHAISE"] = "CHAIR";
	n["LOUNGE CHAIR"] = "CHAIR";
	n["SEAT"] = "CHAIR";

	n["DESK"] = "DESK";
	n["WRITING DESK"] = "DESK";
	n["DUSTY DESK"] = "DESK";
	n["DUSTY WRITING DESK"] = "DESK";
	n["DUST"] = "DESK";

	n["PIANO"] = "PIANO";
	n["MUSIC"] = "PIANO";
	n["INSTRUMENT"] = "PIANO";
	n["KEYBOARD"] = "PIANO";

	n["MUSICIAN"] = "MUSICIAN";
	n["PLAYER"] = "MUSICIAN";
	n["MUSIC PLAYER"] = "MUSICIAN";
	n["PIANIST"] = "MUSICIAN";
	n["PERFORMER"] = "MUSICIAN";

	n["FOG"] = "FOG";
	n["FOGGY"] = "FOG";
	n["CLOUD"] = "FOG";
	n["CLOUDY"] = "FOG";
	n["MIST"] = "FOG";
	n["MISTY"] = "FOG";

	n["STONE"] = "STONE";
	n["BRICK"] = "STONE";
	n["ROCK"] = "STONE";
	n["CRUMBLING STONE"] = "STONE";
	n["CRUMBLING BRICK"] = "STONE";
	n["CRUMBLING ROCK"] = "STONE";

	n["HEDGE"] = "HEDGE";
	n["SHRUB"] = "HEDGE";
	n["SHRUBERY"] = "HEDGE";
	n["BUSH"] = "HEDGE";

	n["FOUNTAIN"] = "FOUNTAIN";

	n["TABLE"] = "TABLE";

	n["CURTAIN"] = "CURTAIN";
	n["DRAPE"] = "CURTAIN";
	n["BLIND"] = "CURTAIN";
	n["WINDOW TREATMENT"] = "CURTAIN";
	n["WINDOW COVERING"] = "CURTAIN";

	n["WALL"] = "WALL";
	n["WINDOW"] = "WALL";
	n["TALL WINDOW"] = "WALL";
	n["TALL WALL"] = "WALL";
	n["LONG WALL"] = "WALL";

	n["FOOD"] = "FOOD";
	n["LEFTOVER FOOD"] = "FOOD";
	n["LEFT-OVER FOOD"] = "FOOD";
	n["LEFTOVER"] = "FOOD";
	n["LEFT-OVER"] = "FOOD";
	n["HALF-EATEN"] = "FOOD";
	n["ROTTING FOOD"] = "FOOD";
	n["OLD FOOD"] = "FOOD";
	n["DINING TABLE"] = "FOOD";
	n["DINNER FOOD"] = "FOOD";

	n["PANTRY"] = "PANTRY";
	n["CLOSET"] = "PANTRY";
	n["CUPBOARD"] = "PANTRY";
	n["KITCHEN PANTRY"] = "PANTRY";

	n["HEARTH"] = "HEARTH";
	n["FIREPLACE"] = "HEARTH";
	n["MANTLE"] = "HEARTH";
	n["FIRE"] = "HEARTH";

	n["HORSE"] = "HORSE";
	n["PONY"] = "HORSE";
	n["STEED"] = "HORSE";
	n["WAR HORSE"] = "HORSE";
	n["BATTLE HORSE"] = "HORSE";
	n["GHOST"] = "HORSE";
	n["GHOST HORSE"] = "HORSE";
	n["GHOSTLY HORSE"] = "HORSE";

	n["LOFT"] = "LOFT";
	n["HAY LOFT"] = "LOFT";
	n["HAYLOFT"] = "LOFT";
	n["LADDER"] = "LOFT";

	n["GOLD"] = "GOLD";
	n["TREASURE"] = "GOLD";
	n["COIN"] = "GOLD";
	n["JEWELRY"] = "GOLD";

	n["SHACKLES"] = "SHACKLES";
	n["CUFFS"] = "SHACKLES";
	n["HANDCUFFS"] = "SHACKLES";

	n["CHAINS"] = "CHAINS";
	n["CHAIN"] = "CHAINS";

	n["LAIR"] = "DRAGON";
	n["DRAGON LAIR"] = "DRAGON";
	n["DRAGON"] = "DRAGON";


	n["GUARD"] = "GUARD";

	n["SHACKLES"] = "SHACKLES";

	n["WINDLASS"] = "WINDLASS";
 
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
