#include "game.hpp"

int main(void) {
	Board b;
	Lord l;

	b.printBoard();
	b.getCell(0, 0)->setPiece(&l);
	b.printBoard();

	return 0;
}
