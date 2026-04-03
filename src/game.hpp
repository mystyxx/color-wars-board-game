#pragma once
#include "pieces.hpp"
#include "game_variables.hpp"
#include "vector"

class Action {
	action_id action = ACTION_PASS;
	unsigned int x = 0;
	unsigned int y = 0;
	piece_id piece = PIECE_NONE;

	public:
		Action();
		action_id getActionId();
		unsigned int getX();
		unsigned int getY();
		piece_id getPiece();

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

		// TODO
		//int handleAction(Action);

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
