#include "game_variables.hpp"
#include "pieces.hpp"
#include "vector"

class Cell {
	Piece* piece;
	public:
		Piece* getPiece();
		void setPiece(Piece* p);
		int row, col;
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
};
