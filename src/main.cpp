#include "game.hpp"
#include <iostream>

int main(void) {

	Team t1('1', 30);
	Team t2('2', 30);
	std::deque<Team*> teams;
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

	while(!b->getPiecesFromTeam(b->getTeams().front()->getId()).empty()) {
		std::cout << "Au tour de l'équipe " << b->getTeams().front()->getId() << " (" << b->getTeams().front()->getGold() << " GOLD)"<< std::endl;
		std::vector<Piece*> p = b->getPiecesFromTeam(b->getTeams().front()->getId());
		for(int i = 0; i < p.size(); ++i)
			p[i]->setHasPlayedTT(false);

		while(!b->getAvailablePiecesFromTeam(b->getTeams().front()->getId()).empty()) {
			b->printBoard();
			my_board.handleAction(t.askAction(t.askPiece()));
		}
		b->getTeams().push_back(b->getTeams().front());
		b->getTeams().pop_front();
	
	}

	b.printBoard();


	return 0;
}
