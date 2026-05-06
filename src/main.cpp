#include "game.hpp"
#include "renderer.hpp"

int main(void) {

	Team* t1 = new Team('1', 30);
	Team* t2 = new Team('2', 30);
	std::deque<Team*> teams;
	teams.push_back(t1);
	teams.push_back(t2);

	std::vector<Action*> history;

	Renderer* r = new Renderer();
	Board* b = new Board(teams, BOARD_W, BOARD_H, r);

	Lord* l = new Lord('1');
	Lord* l2 = new Lord('2');
	TurnManager t(*b, *r);

	//b->printBoard();
	b->getCell(10, 0)->setPiece(l);
	b->getCell(10, 19)->setPiece(l2);
	//b->printBoard();
	

	r->drawStatus(*b);
	while(!b->getPiecesFromTeam(b->getTeams().front()->getId()).empty()) {
		// std::cout << "Au tour de l'équipe " << b->getTeams().front()->getId() << " (" << b->getTeams().front()->getGold() << " GOLD)"<< std::endl;
		std::vector<Piece*> p = b->getPiecesFromTeam(b->getTeams().front()->getId());
		for(int i = 0; i < p.size(); ++i)
			p[i]->setHasPlayedTT(false);

		while(!b->getAvailablePiecesFromTeam(b->getTeams().front()->getId()).empty()) {
			r->drawBoard(*b);
			
			Action* a = t.askAction(t.askPiece());

			if(b->handleAction(a))
				history.push_back(new Action(*a));

			r->drawStatus(*b);
			r->drawAction(history);
			delete a;
		}
		b->getTeams().push_back(b->getTeams().front());
		b->getTeams().pop_front();
		r->drawStatus(*b);
		t.showSaveLoadMenu();
	
	}

	// std::cout << std::endl << "Partie remportée par l'équipe " << b->getTeams().front()->getId() << std::endl;

	b->printBoard();

	delete b;
	delete r;
	delete t1;
	delete t2;
	for(int i = 0; i < history.size(); ++i)
		delete history[i];

	return 0;
}
