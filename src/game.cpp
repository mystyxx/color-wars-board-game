#include "game_variables.hpp"
#include "pieces.hpp"

class Cell {
	Piece* piece;
	public:
		Piece* getPiece();
		void setPiece();
		int row, col;
};

class Board {
	Cell cells[BOARD_W][BOARD_H];
	public:
		Cell* findCell(Piece* piece);
};
