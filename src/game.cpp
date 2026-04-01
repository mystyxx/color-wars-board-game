#include "game.hpp"
#include "game_variables.hpp"
#include <iostream>

// Cell
Piece* Cell::getPiece() {
	return this->piece;
}
void Cell::setPiece(Piece* p) {
	this->piece = p;
}

// Board
Board::Board() {
	for(int i = 0; i < BOARD_W; ++i) {
		for(int j = 0; j < BOARD_H; ++j) {
			this->cells[i][j].setPiece(nullptr);
			this->cells[i][j].row = i;
			this->cells[i][j].col = j;
		}
	}
}

Cell* Board::getCell(int r, int c) {
	if(r < 0 || c < 0 || r > BOARD_W || r > BOARD_H) return nullptr;
	return &this->cells[r][c];
} 
Cell* Board::findCell(Piece* piece) {
	for(int i = 0; i < BOARD_W; ++i) {
		for(int j = 0; j < BOARD_H; ++j) {
			if(this->cells[i][j].getPiece() == piece)
				return &this->cells[i][j];
		}
	}
	return nullptr;
}

std::vector<Piece*> Board::getPiecesFromTeam(char team) {
	std::vector<Piece*> res;
	for(int i = 0; i < BOARD_W; ++i) {
		for(int j = 0; j < BOARD_H; ++j) {
			Piece* p = this->cells[i][j].getPiece();
			if(p != nullptr && p->getTeam() == team) {
				res.push_back(this->cells[i][j].getPiece());
			}
		}
	}
	return res;
}

std::vector<Piece*> Board::getAvailablePiecesFromTeam(char team) {
	std::vector<Piece*> res;
	for(int i = 0; i < BOARD_W; ++i) {
		for(int j = 0; j < BOARD_H; ++j) {
			Piece* p = this->cells[i][j].getPiece();
			if(p != nullptr && p->getTeam() == team && p->getHasPlayedTT() == false) {
				res.push_back(this->cells[i][j].getPiece());
			}
		}
	}
	return res;
}

int Board::manhattanDist(Cell c1, Cell c2) {
	int res = (c1.row - c2.row + c1.col - c2.col);
	return res < 0 ? -res : res;
}

void Board::printBoard() {
	for(int j = 0; j < BOARD_W; ++j) {
		std::cout << "--";
	}
	std::cout << "-" << std::endl;

	for(int i = 0; i < BOARD_W; ++i) {
		for(int j = 0; j < BOARD_H; ++j) {
			char s;
			Cell* c = &this->cells[i][j];

			if(c->getPiece() == nullptr) s = ' ';
			else s = c->getPiece()->getDisplayChar();

			std::cout << "|" << s;
		}
		std::cout << "|" << std::endl;

		for(int j = 0; j < BOARD_W; ++j) {
			std::cout << "--";
		}
		std::cout << "-" << std::endl;
	}
	return;
}
