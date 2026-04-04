#include "game.hpp"
#include "game_variables.hpp"
#include "pieces.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>

// Team
Team::Team(char id, int gold) {
	this->id= id;
	this->gold = gold;
}

char Team::getId() { return this->id; }
int Team::getGold() { return this->gold; }
void Team::setGold(int amount) { this->gold = amount; }
bool Team::addGold(int amount) {
	if(this->gold + amount < 0)
		return false;
	else this->gold += amount;
	return true;
}


// Action
Action::Action(Piece* piece, action_id action, unsigned int x, unsigned int y, piece_id pid) { 
	this->action = ACTION_PASS;
	this->x = 0;
	this->y = 0;
	this->pid = pid;
	this->piece = piece;
}

action_id Action::getActionId() {
	return this->action;
}
unsigned int Action::getX() {
	return this->x;
}
unsigned int Action::getY() {
	return this->y;
}
Piece* Action::getPiece() {
	return this->piece;
}
Piece* Action::getTarget() {
	return this->target;
}
void Action::setActionId(action_id a) {
	this->action = a;
}
void Action::setX(unsigned int x) {
	this->x = x;
}
void Action::setY(unsigned int y) {
	this->y = y;
}
void Action::setPiece(Piece* p) {
	this->piece = p;
}
void Action::setTarget(Piece* p) {
	this->target = p;
}
void Action::setPieceId(piece_id p) {
	this->pid = p;
}

// Cell
Piece* Cell::getPiece() {
	return this->piece;
}
void Cell::setPiece(Piece* p) {
	this->piece = p;
}

// Board
Board::Board(std::vector<Team*> vt) : teams(vt) {
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

std::vector<Team*> Board::getTeams() { return this->teams; }

Team* Board::getTeam(char team) {
	for(int i = 0; i < teams.size(); ++i) 
		if(team == this->teams[i]->getId())
			return teams[i];
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
	return std::abs((int)c1.row - (int)c2.row) + std::abs((int)c1.col - (int)c2.col);
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

bool Board::handleAction(Action* a) {
	if(!a) return false;
	switch(a->getActionId()) {
		case ACTION_PASS: {
			a->getPiece()->setHasPlayedTT(true);
			break;
						  }
		case ACTION_MOVE: {
			Mobile* mob = dynamic_cast<Mobile*>(a->getPiece());
			if(!mob) return false;

			if(a->getX() < 0 || a->getX() > BOARD_W || a->getY() < 0 || a->getY() > BOARD_H) {
				std::cout << "Error : tile is out of bounds !" << std::endl;
				return false;
			}

			int dist = this->manhattanDist(*this->getCell(a->getX(), a->getY()), *this->findCell(a->getPiece()));
			if(dist > mob->getMoveSpeed()) {
				std::cout << "Error : tile is too far !" << std::endl;
				return false;
			}

			Cell* dest = this->getCell(a->getX(), a->getY());
			if(dest->getPiece() != nullptr) {
				std::cout << "Error : Tile occupied !";
				return false;
			}

			Cell* c = this->findCell(mob);
			c->setPiece(nullptr);
			this->getCell(a->getX(), a->getY())->setPiece(mob);
			return true;
						  }
		case ACTION_MOVEANDATTACK: {
			Fighter* mob = dynamic_cast<Fighter*>(a->getPiece()); 
			if(!mob) return false;

			if(a->getX() < 0 || a->getX() > BOARD_W || a->getY() < 0 || a->getY() > BOARD_H) {
				std::cout << "Error : tile is out of bounds !" << std::endl;
				return false;
			}
			int dist = this->manhattanDist(*this->getCell(a->getX(), a->getY()), *this->findCell(a->getPiece()));
			if(dist > mob->getMoveSpeed()) {
				std::cout << "Error : tile is too far !" << std::endl;
				return false;
			}

			Cell* dest = this->getCell(a->getX(), a->getY());
			if(dest->getPiece() != nullptr) {
				std::cout << "Error : Tile occupied !";
				return false;
			}

		    Cell* c = this->findCell(mob);
			Piece* target = a->getTarget();
			if(!target) return false;
			c->setPiece(nullptr);
			target->setHp(target->getHp() - mob->getPower());
			dest->setPiece(mob);

			if(target->getHp() <= 0) {
				this->findCell(target)->setPiece(nullptr);
				std::cout << "You have eliminated " << target->getDisplayChar() << "  (" << c->row  << ", " << c->col << ")" << std::endl;
			}

			return true;
								   }

		case ACTION_GATHER: {
			Gatherer* g = dynamic_cast<Gatherer*>(a->getPiece());
			if(!g) return false;

			Team* t = this->getTeams()[0];
			if(!t) return false;

			t->addGold(g->getProd());
			a->getPiece()->setHasPlayedTT(true);
			return true;

							}
		default: return false;
	}
	return false;
}

// TurnManager
TurnManager::TurnManager(Board& b) : board(b) {}

Board& TurnManager::getBoard() {
	return this->board;
}

Piece& TurnManager::askPiece() {
	std::vector<Piece*> av_pieces = this->getBoard().getAvailablePiecesFromTeam(this->getBoard().getTeams()[0]->getId());
	std::cout << "Select a piece : " << std::endl;
	for(int i = 1; i < av_pieces.size()+1; ++i) {
		Cell* c = this->getBoard().findCell(av_pieces[i-1]);
		int x = c->row; int y = c->col;
		std::cout << i << ". " << av_pieces[i-1]->getDisplayChar() << "  (" << x << ", " << y << ")"  << std::endl;
	}

	unsigned short choice = -1;
	while(choice > av_pieces.size()) {
		std::cout << "> "; std::cin >> choice;
		if(choice > av_pieces.size()) 
			std::cout << "Incorrect piece number" << std::endl;
	}


	return *av_pieces[choice-1];
}

Action* TurnManager::askAction(Piece& p) {
	Action* a = new Action();
	if(p.getAutorizedActions() & ACTION_PASS) 
		std::cout << "0. Pass turn \t";
	if(p.getAutorizedActions() & ACTION_MOVE) 
		std::cout << "1. Move \t";
	if(p.getAutorizedActions() & ACTION_MOVEANDATTACK) 
		std::cout << "2. Move & attack \t";
	if(p.getAutorizedActions() & ACTION_GATHER) 
		std::cout << "3. Gather GOLD \t";
	if(p.getAutorizedActions() & ACTION_SPAWN) 
		std::cout << "4. Spawn PIECE \t";
	std::cout << std::endl;

	unsigned short choice = 0;
	do {
		std::cout << "> "; std::cin >> choice;
		if(!(1 << choice & p.getAutorizedActions()))
			std::cout << "Action not allowed for this piece" << std::endl;
	}
	while(!(p.getAutorizedActions() & 1 << choice));

	unsigned int x, y;
	std::vector<Cell*> adj_pieces;
	unsigned short pid = -1;

	switch(1 << choice) {
		case ACTION_MOVE: 
			do {
				std::cout << "Type coords you want to go (with a space in between)" << std::endl;
				std::cout << "> "; std::cin >> x; std::cin >> y;
			} while(x < 0 || y < 0 || x > BOARD_W || y > BOARD_H);
			a->setX(x); a->setY(y);
			break;

		case ACTION_MOVEANDATTACK: {
			do {
				std::cout << "Type coords you want to go (with a space in between)" << std::endl;
				std::cout << "> "; std::cin >> x; std::cin >> y;
			} while(x < 0 || y < 0 || x > BOARD_W || y > BOARD_H);
			a->setX(x); a->setY(y);


			choice = -1;
			int dx[] = {-1, 1, 0, 0};
			int dy[] = {0, 0, -1, 1};

				for(int i = 0; i < 4; ++i) {
					Cell* c = this->getBoard().getCell(x + dx[i], y + dy[i]);
					if(c)
						if(c->getPiece() && c->getPiece()->getTeam() != p.getTeam())
							adj_pieces.push_back(c);
				}
			if(adj_pieces.size() == 0) {std::cout << "Error : You cannot attack anyone from here." << std::endl; return nullptr;}
			else if(adj_pieces.size() == 1) { a->setTarget(adj_pieces.front()->getPiece()); }
			else {
				do {
					std::cout << "Which piece do you wish to attack ?" << std::endl;
					for(int i = 0; i < adj_pieces.size(); ++i)
						std::cout << i + 1 << ". " << adj_pieces[i]->getPiece()->getDisplayChar() << "  (" << x << ", " << y << ")"  << std::endl;
					std::cout << "> "; std::cin >> choice;
					if(choice > adj_pieces.size()) std::cout << "Please enter a valid target" << std::endl;

				} while(choice > adj_pieces.size());
				a->setTarget(adj_pieces[choice - 1]->getPiece());
			}

			break;
								   }

		case ACTION_SPAWN:
			Spawner* s = dynamic_cast<Spawner*>(&p);
			do {
				std::cout << "Type coords you want your piece to spawn to (with a space in between)" << std::endl;
				std::cout << "> "; std::cin >> x; std::cin >> y;
			} while(x < 0 || y < 0 || x > BOARD_W || y > BOARD_H);

			if(s->getCanSpawn().size() == 1) {
				pid = s->getCanSpawn().front();

			}
			else do {
				std::cout << "What piece do you wish to spawn ?" << std::endl;
				for(int i = 0; i < s->getCanSpawn().size(); ++i)
					std::cout << i + 1 << ". " << s->getCanSpawn()[i] << std::endl; 
				std::cout << "> "; std::cin >> pid;
				if(pid > s->getCanSpawn().size()) std::cout << "Please enter a valid piece" << std::endl;

			} while(pid > s->getCanSpawn().size());

			a->setX(x); a->setY(y);
			a->setPieceId(s->getCanSpawn()[pid-1]);

			break;

	}

	a->setPiece(&p);
	a->setActionId(1 << choice);
	return a;
}
