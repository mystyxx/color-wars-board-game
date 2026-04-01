#include "game_variables.hpp"
#include "pieces.hpp"
#include <vector>

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
		Cell* findCell(Piece* piece) {
			for(int i = 0; i < BOARD_W; ++i) {
				for(int j = 0; j < BOARD_H; ++j) {
				if(this->cells[BOARD_W][BOARD_H].getPiece() == piece)
					return &this->cells[BOARD_W][BOARD_H];
				}
			}
			return nullptr;
		}

		std::vector<Piece*> getPiecesFromTeam(char team) {
			std::vector<Piece*> res;
			for(int i = 0; i < BOARD_W; ++i) {
				for(int j = 0; i < BOARD_H; ++j) {
					Piece* p = this->cells[i][j].getPiece();
					if(p->getTeam() == team) {
						res.push_back(this->cells[i][j].getPiece());
					}
				}
			}
			return res;
		}

		int manhattanDist(Cell c1, Cell c2) {
			int res = (c1.row - c2.row + c1.col - c1.col);
			return res < 0 ? -res : res;
		}
};
