#include "game.hpp"

int main(void) {

	Team t1('1', 30);
	Team t2('2', 30);
	std::vector<Team*> teams;
	teams.push_back(&t1);
	teams.push_back(&t2);

	Board b(teams);
	Board& my_board = b;

	Lord l('1');
	TurnManager t(my_board);


	//b.printBoard();
	b.getCell(0, 0)->setPiece(&l);
	b.printBoard();

	my_board.handleAction(t.askAction(t.askPiece()));

	b.printBoard();


	return 0;
}
