#include <cstdlib>
class Cell;

class Piece {
	int hp;
	int cost;
	char display_char;
	char team;
	int has_played_tt;

	public:
		Piece();
		virtual ~Piece();

		int getHp(); void setHp();
		int getCost();
		char getDisplayChar();
		char getTeam(); void setTeam();
		int getHasPlayedTT(); int setHasPlayedTT();

		virtual void sayUniqueLine() = 0;

		// actions
		void passTurn();
};

class Mobile : public Piece {
		int movespeed;
	public:
		Mobile();
		virtual ~Mobile();

		int getMoveSpeed(); int setMoveSpeed();

		// actions
		void goTo(int a, int b);
};

class Gatherer : public Piece {
	int prod;

	public:
		Gatherer();
		virtual ~Gatherer();

		int getProd(); void setProd();

		// actions
		void gather();
};

class Fighter : public Mobile {
	int power;
	public:
		Fighter();
		virtual ~Fighter();

		int getPower(); void setPower();
	
		// actions
		void attack(Cell* target);
};


class Spawner : public Piece {
	protected:
		Piece** can_spawn;

	public:
		Spawner();
		virtual ~Spawner() {
			delete[] can_spawn;
		};

		Piece* getCanSpawn();

		// actions
		void pieceSpawn(Piece*, Cell*);
};


/* -------------- PIECES REELLES -------------- */

class Castle : public Spawner {
	Castle();
	~Castle();
};
class Lord : public Fighter, public Spawner {
	Lord();
	~Lord();
};
class Warrior : public Fighter {
	Warrior();
	~Warrior();
};
class Farmer : public Mobile, public Gatherer {
	Farmer();
	~Farmer();
};
