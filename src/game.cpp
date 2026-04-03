#include "game.hpp"
#include "game_variables.hpp"
#include "pieces.hpp"
#include <iostream>

#include <vector>
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

int Board::handleAction(Action a) {
	switch(a.getActionId()) {
		case ACTION_PASS:
			a.getPiece()->setHasPlayedTT(true);
	}
	return 0;
}

// TurnManager
TurnManager::TurnManager(std::vector<char> t, Board& b) : teams(t), board(b) {}

std::vector<char> TurnManager::getTeams() {
	return this->teams;
}

Board& TurnManager::getBoard() {
	return this->board;
}

Piece& TurnManager::askPiece() {
	std::vector<Piece*> av_pieces = this->getBoard().getAvailablePiecesFromTeam(this->getTeams()[0]);
	std::cout << "selectionnez une pièce" << std::endl;
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

Action TurnManager::askAction(Piece& p) {
	Action a;
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
			a.setX(x); a.setY(y);
			break;

		case ACTION_MOVEANDATTACK: 
			do {
				std::cout << "Type coords you want to go (with a space in between)" << std::endl;
				std::cout << "> "; std::cin >> x; std::cin >> y;
			} while(x < 0 || y < 0 || x > BOARD_W || y > BOARD_H);
			a.setX(x); a.setY(y);

			choice = -1;
			for(int i = -1; i > 2; ++++i)
				for(int j = -1; j > 2; ++++j)
					adj_pieces.push_back(this->getBoard().getCell(x + i, y + j));
			if(adj_pieces.size() == 0) {std::cout << "Error : You cannot attack anyone from here." << std::endl; return a;}
			else if(adj_pieces.size() > 1) a.setTarget(adj_pieces.front()->getPiece());
			else do {
				std::cout << "What piece do you wish to attack ?" << std::endl;
				for(int i = 1; i < adj_pieces.size(); ++i)
					std::cout << i << ". " << adj_pieces[i]->getPiece()->getDisplayChar() << "  (" << x << ", " << y << ")"  << std::endl;
				std::cout << "> "; std::cin >> choice;
				if(!(choice > adj_pieces.size())) std::cout << "Please enter a valid target" << std::endl;

			} while(choice > adj_pieces.size());
			a.setTarget(adj_pieces[choice]->getPiece());

			break;

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
				for(int i = 1; i < s->getCanSpawn().size(); ++i)
					std::cout << i << ". " << s->getCanSpawn()[i-1] << std::endl; 
				std::cout << "> "; std::cin >> pid;
				if(pid > s->getCanSpawn().size()) std::cout << "Please enter a valid piece" << std::endl;

			} while(pid > s->getCanSpawn().size());

			a.setX(x); a.setY(y);
			a.setPieceId(s->getCanSpawn()[pid-1]);

			break;

	}

	a.setPiece(&p);
	a.setActionId(1 << choice);
	return a;
}
