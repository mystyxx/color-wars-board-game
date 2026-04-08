#ifndef _VARDEF_
#define _VARDEF_

#include <map>
#include <string>

#define BOARD_W 20
#define BOARD_H 20

enum piece_id {
	PIECE_NONE,
	PIECE_LORD,
	PIECE_CASTLE,
	PIECE_WARRIOR,
	PIECE_FARMER,
};

#define PIECE_COST_CASTLE 15
#define PIECE_COST_LORD 10
#define PIECE_COST_WARRIOR 10
#define PIECE_COST_FARMER 20

// actions
// spawn, gather, move&attack, move, pass
typedef unsigned short action_id;

#define ACTION_PASS 0b00001
#define ACTION_MOVE 0b00010
#define ACTION_MOVEANDATTACK 0b00100
#define ACTION_GATHER 0b01000
#define ACTION_SPAWN 0b10000

#define UI_STATUS_H 3
#define UI_INPUT_H 10

// Dictionnaires pour les affichages
const std::map<piece_id, std::string> PIECE_NAMES = {
	{PIECE_NONE, ""},
	{PIECE_LORD, "LORD"},
	{PIECE_CASTLE, "CASTLE"},
	{PIECE_WARRIOR, "WARRIOR"},
	{PIECE_FARMER, "FARMER"}
};
const std::map<char, std::string> PIECE_CNAMES = {
	{'\0', ""},
	{'L', "LORD"},
	{'C', "CASTLE"},
	{'W', "WARRIOR"},
	{'F', "FARMER"}
};

const std::map<action_id, std::string> ACTION_NAMES = {
	{ACTION_PASS, "Pass"},
	{ACTION_MOVE, "Move"},
	{ACTION_MOVEANDATTACK, "Attack"},
	{ACTION_GATHER, "Gather"},
	{ACTION_SPAWN, "Creation"}
};

#endif
