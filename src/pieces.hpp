class Cell;

class Piece {
	int hp;
	const int cost;
	char display_char;
	char team;
	int has_played_tt;

	public:
		Piece(char display_char, int cost);
		virtual ~Piece();

		int getHp(); void setHp(int);
		int getCost(); 
		char getDisplayChar();
		char getTeam(); void setTeam(char);
		int getHasPlayedTT(); void setHasPlayedTT(int);

		virtual void sayUniqueLine() = 0;

		// actions
		void passTurn();
};

class Mobile : virtual public Piece {
		int movespeed;
	public:
		Mobile(char c, int cost);
		virtual ~Mobile();

		int getMoveSpeed(); void setMoveSpeed(int);

		// actions
		void goTo(int a, int b);
};

class Gatherer : virtual public Piece {
	int prod;

	public:
		Gatherer(char c, int cost);
		virtual ~Gatherer();

		int getProd(); void setProd(int);

		// actions
		void gather();
};

class Fighter : public Mobile {
	int power;
	public:
		Fighter(char c, int cost);
		virtual ~Fighter();

		int getPower(); void setPower(int);
	
		// actions
		void attack(Cell* target);
};


class Spawner : virtual public Piece {
	protected:
		Piece** can_spawn;

	public:
		Spawner(char c, int cost);
		virtual ~Spawner();

		Piece** getCanSpawn();

		// actions
		void pieceSpawn(Piece*, Cell*);
};


/* -------------- PIECES REELLES -------------- */

class Castle : public Spawner, public Gatherer {
	public:
		Castle();
		void sayUniqueLine();
};
class Lord : public Fighter, public Spawner {
	public:
		Lord();
		void sayUniqueLine();
};
class Warrior : public Fighter {
	public:
		Warrior();
		void sayUniqueLine();
};
class Farmer : public Mobile, public Gatherer {
	public:
		Farmer();
		void sayUniqueLine();
};
