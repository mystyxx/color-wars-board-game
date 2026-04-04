#pragma once
#include "pieces.hpp"
#include "game_variables.hpp"
#include "vector"

class Team {
	char id;
	int gold;
	public:
		Team(char id, int gold); 
		char getId();
		int getGold(); 
		void setGold(int);
		bool addGold(int);
};

class Action {
	action_id action;
	unsigned int x;
	unsigned int y;
	piece_id pid;
	Piece* piece;
	Piece* target;

	public:
		Action(Piece* piece = nullptr, action_id action = ACTION_PASS, unsigned int x = 0, unsigned int y = 0, piece_id pid = PIECE_NONE);
		action_id getActionId(); void setActionId(action_id);
		unsigned int getX(); void setX(unsigned int);
		unsigned int getY(); void setY(unsigned int);
		piece_id getPieceId(); void setPieceId(piece_id);
		Piece* getPiece(); void setPiece(Piece*);
		Piece* getTarget(); void setTarget(Piece*);
};

class Cell {
	Piece* piece;
	public:
		Piece* getPiece();
		void setPiece(Piece* p);
		unsigned int row, col;
};

class Board {
	Cell cells[BOARD_W][BOARD_H];
	std::vector<Team*> teams;
	public:
		Board(std::vector<Team*>);
		Cell* getCell(int r, int c);
		Cell* findCell(Piece* piece);
		std::vector<Team*> getTeams();
		Team* getTeam(char);
		std::vector<Piece*> getPiecesFromTeam(char team);
		std::vector<Piece*> getAvailablePiecesFromTeam(char team);
		int manhattanDist(Cell c1, Cell c2);
		void printBoard();
		bool handleAction(Action*);
};

class TurnManager {
	Board& board;
	public:
		TurnManager(Board&);
		Board& getBoard();
		Piece& askPiece();
		Action* askAction(Piece& piece);
};
