#include "game.hpp"
#include <iostream>

int main(void) {

	Team t1('1', 30);
	Team t2('2', 30);
	std::vector<Team*> teams;
	teams.push_back(&t1);
	teams.push_back(&t2);

	Board b(teams);
	Board& my_board = b;

	Lord l('1');
	Lord l2('2');
	TurnManager t(my_board);



	//b.printBoard();
	b.getCell(10, 0)->setPiece(&l);
	b.getCell(10, 19)->setPiece(&l2);
	//b.printBoard();

	while(!b.getPiecesFromTeam(teams.front()->getId()).empty()) {
		std::cout << "Au tour de l'équipe " << teams.front()->getId() << " (" << teams.front()->getGold() << " GOLD)"<< std::endl;
		while(!b.getAvailablePiecesFromTeam(teams.front()->getId()).empty()) {
			my_board.handleAction(t.askAction(t.askPiece()));
			b.printBoard();
		}
		teams.push_back(teams.front());
		teams.erase(teams.begin());
	
	}

	b.printBoard();


	return 0;
}
