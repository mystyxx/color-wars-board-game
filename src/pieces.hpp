#pragma once
#include "game_variables.hpp"
#include <vector>
class Cell;

class Piece {
	int hp;
	const int cost;
	char display_char;
	char team; 
	int has_played_tt;
	action_id autorized_actions;

	protected:
		void addPermission(action_id action);

	public:
		Piece(char team, char display_char, int cost);
		virtual ~Piece();

		int getHp(); void setHp(int);
		int getCost(); 
		char getDisplayChar();
		char getTeam(); void setTeam(char);
		int getHasPlayedTT(); void setHasPlayedTT(int);
		action_id getAutorizedActions();

		virtual void sayUniqueLine() = 0;

		// actions
		void passTurn();
};

class Mobile : virtual public Piece {
		int movespeed;
	public:
		Mobile(char team, char c, int cost);
		virtual ~Mobile();

		int getMoveSpeed(); void setMoveSpeed(int);

};

class Gatherer : virtual public Piece {
	int prod;

	public:
		Gatherer(char team, char c, int cost);
		virtual ~Gatherer();

		int getProd(); void setProd(int);
};

class Fighter : public Mobile {
	int power;
	public:
		Fighter(char team, char c, int cost);
		virtual ~Fighter();

		int getPower(); void setPower(int);
	
		// actions
		void attack(Cell* target);
};


class Spawner : virtual public Piece {
	protected:
		std::vector<piece_id> can_spawn;

	public:
		Spawner(char team, char c, int cost);

		std::vector<piece_id> getCanSpawn();

		// actions
		void pieceSpawn(Piece*, Cell*);
};


/* -------------- PIECES REELLES -------------- */

class Castle : public Spawner, public Gatherer {
	public:
		Castle(char);
		void sayUniqueLine();
};
class Lord : public Fighter, public Spawner {
	public:
		Lord(char);
		void sayUniqueLine();
};
class Warrior : public Fighter {
	public:
		Warrior(char);
		void sayUniqueLine();
};
class Farmer : public Mobile, public Gatherer {
	public:
		Farmer(char);
		void sayUniqueLine();
};
