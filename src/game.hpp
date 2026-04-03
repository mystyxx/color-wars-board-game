#pragma once
#include "pieces.hpp"
#include "game_variables.hpp"
#include "vector"

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
	public:
		Board();

		Cell* getCell(int r, int c);

		Cell* findCell(Piece* piece);

		std::vector<Piece*> getPiecesFromTeam(char team);

		std::vector<Piece*> getAvailablePiecesFromTeam(char team);

		int manhattanDist(Cell c1, Cell c2);

		void printBoard();

		int handleAction(Action);

};

class TurnManager {
	std::vector<char> teams;
	Board& board;
	public:
		TurnManager(std::vector<char>, Board&);
		std::vector<char> getTeams();
		Board& getBoard();
		Piece& askPiece();
		Action askAction(Piece& piece);
};
