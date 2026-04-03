#include "game.hpp"

int main(void) {
	Board b;
	Board& my_board = b;

	std::vector<char> teams;
	teams.push_back('1');
	teams.push_back('2');
	Lord l('1');
	TurnManager t(teams, my_board);

	//b.printBoard();
	b.getCell(0, 0)->setPiece(&l);
	b.printBoard();


	t.askAction(t.askPiece());

	return 0;
}
