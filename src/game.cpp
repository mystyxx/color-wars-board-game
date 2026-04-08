#include "game.hpp"
#include "game_variables.hpp"
#include "renderer.hpp"

#include <sstream>
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
piece_id Action::getPieceId() {
	return this->pid;
}

// Cell
Piece* Cell::getPiece() {
	return this->piece;
}
void Cell::setPiece(Piece* p) {
	this->piece = p;
}

// Board
Board::Board(std::deque<Team*> vt, Renderer* r) : teams(vt), renderer(r) {
	for(int i = 0; i < BOARD_H; ++i) {
		for(int j = 0; j < BOARD_W; ++j) {
			this->cells[i][j].setPiece(nullptr);
			this->cells[i][j].row = i;
			this->cells[i][j].col = j;
		}
	}
}
Board::~Board() {
	for(int i = 0; i < teams.size(); ++i) {
        delete teams[i];
    }
	for(int i = 0; i < BOARD_W; ++i) {
		for(int j = 0; j < BOARD_H; ++j) {
			delete cells[i][j].getPiece();
			cells[i][j].setPiece(nullptr);
		}
	}
}

Cell* Board::getCell(int r, int c) {
	if(r < 0 || c < 0 || c > BOARD_W || r > BOARD_H) return nullptr;
	return  &this->cells[c][r];
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

std::deque<Team*>& Board::getTeams() { return this->teams; }

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
	if(!this->renderer) return;
	std::stringstream board_output;
	for(int j = 0; j < BOARD_W; ++j) {
		board_output << "--";
	}
	board_output << "-\n";

	for(int i = 0; i < BOARD_W; ++i) {
		for(int j = 0; j < BOARD_H; ++j) {
			char s;
			Cell* c = &this->cells[i][j];

			if(c->getPiece() == nullptr) s = ' ';
			else s = c->getPiece()->getDisplayChar();

			board_output << "|" << s;
		}
		board_output << "|\n";

		for(int j = 0; j < BOARD_W; ++j) {
			board_output << "--";
		}
		board_output << "-\n";
	}
	this->renderer->drawPrompt(board_output.str());
	return;
}

bool Board::handleAction(Action* a) {
	if(!a) return false;
	a->getPiece()->sayUniqueLine();
	switch(a->getActionId()) {
		case ACTION_PASS: {
			a->getPiece()->setHasPlayedTT(true);
			break;
						  }
		case ACTION_MOVE: {
			Mobile* mob = dynamic_cast<Mobile*>(a->getPiece());
			if(!mob) return false;

			if(a->getX() < 0 || a->getX() > BOARD_W || a->getY() < 0 || a->getY() > BOARD_H) {
				if(this->renderer) this->renderer->drawPrompt("Error : Tile is out of bounds !");
				getch(); return false;
			}

			int dist = this->manhattanDist(*this->getCell(a->getX(), a->getY()), *this->findCell(a->getPiece()));
			if(dist > mob->getMoveSpeed()) {
				if(this->renderer) this->renderer->drawPrompt("Error : Tile is too far !");
				getch(); return false;
			}

			Cell* dest = this->getCell(a->getX(), a->getY());
			Cell* c = this->findCell(mob);
			if(dest->getPiece() != nullptr && dest != c) {
				if(this->renderer) this->renderer->drawPrompt("Error : Tile occupied !");
				getch(); return false;
			}

			if(c != dest) {
				c->setPiece(nullptr);
				this->getCell(a->getX(), a->getY())->setPiece(mob);
			}

			a->getPiece()->setHasPlayedTT(true);
			return true;
						  }
		case ACTION_MOVEANDATTACK: {
			Fighter* mob = dynamic_cast<Fighter*>(a->getPiece()); 
			if(!mob) return false;

			if(a->getX() < 0 || a->getX() > BOARD_W || a->getY() < 0 || a->getY() > BOARD_H) {
				if(this->renderer) this->renderer->drawPrompt("Error : Tile is out of bounds !");
				getch(); return false;
			}
			int dist = this->manhattanDist(*this->getCell(a->getX(), a->getY()), *this->findCell(a->getPiece()));
			if(dist > mob->getMoveSpeed()) {
				if(this->renderer) this->renderer->drawPrompt("Error : Tile is too far !");
				getch(); return false;
			}

			Cell* dest = this->getCell(a->getX(), a->getY());
		    Cell* c = this->findCell(mob);
			if(dest->getPiece() != nullptr && dest != c) {
				if(this->renderer) this->renderer->drawPrompt("Error : Tile occupied !");
				getch(); return false;
			}

			if(c != dest) {
				c->setPiece(nullptr);
				dest->setPiece(mob);
			}

			Piece* target = a->getTarget();
			if(!target) return false;
			target->setHp(target->getHp() - mob->getPower());

			if(target->getHp() <= 0) {
				this->findCell(target)->setPiece(nullptr);
				std::stringstream elim_msg;
				elim_msg << "You have eliminated " << target->getDisplayChar() << "  (" << c->col << ", " << c->row << ")";
				if(this->renderer) this->renderer->drawPrompt(elim_msg.str());
				delete target;
				target = nullptr;
			}

			a->getPiece()->setHasPlayedTT(true);
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
		case ACTION_SPAWN: {
		    Spawner* s = dynamic_cast<Spawner*>(a->getPiece());
			if(!s) return false;

			Cell* c = this->getCell(a->getX(), a->getY());
			Cell* spawnerCell = this->findCell(a->getPiece());

			if(c->getPiece()) {
				if(this->renderer) this->renderer->drawPrompt("Error : Tile is occupied !");
				getch(); return false;
			}
			if(this->manhattanDist(*c, *this->findCell(a->getPiece())) > 1) {
				if(this->renderer) this->renderer->drawPrompt("Error : Tile is too far ! (range is 1)");
				getch(); return false;
			}

			switch(a->getPieceId()) {
				case PIECE_LORD: {
					if(this->getTeam(a->getPiece()->getTeam())->getGold() < PIECE_COST_LORD) {
						if(this->renderer) this->renderer->drawPrompt("Error : Not enough gold for Lord!");
						getch(); return false;
					}
					Lord* n = new Lord(a->getPiece()->getTeam());
					c->setPiece(n);
					a->getPiece()->setHasPlayedTT(true);
					break;  
				}
				case PIECE_CASTLE: {
					if(this->getTeam(a->getPiece()->getTeam())->getGold() < PIECE_COST_CASTLE) {
						if(this->renderer) this->renderer->drawPrompt("Error : Not enough gold for Castle!");
						getch(); return false;
					}
					Castle* n = new Castle(a->getPiece()->getTeam());
					c->setPiece(n);
					a->getPiece()->setHasPlayedTT(true);
					break; 
				}
				case PIECE_WARRIOR: {
					if(this->getTeam(a->getPiece()->getTeam())->getGold() < PIECE_COST_WARRIOR) {
						if(this->renderer) this->renderer->drawPrompt("Error : Not enough gold for Warrior!");
						getch(); return false;
					}
					Warrior* n = new Warrior(a->getPiece()->getTeam());
					c->setPiece(n);
					a->getPiece()->setHasPlayedTT(true);
					break;
				}
				case PIECE_FARMER: {
					if(this->getTeam(a->getPiece()->getTeam())->getGold() < PIECE_COST_FARMER) {
						if(this->renderer) this->renderer->drawPrompt("Error : Not enough gold for Farmer!");
					a->getPiece()->setHasPlayedTT(true);
						getch(); return false;
					}
					Farmer* n = new Farmer(a->getPiece()->getTeam());
					c->setPiece(n);
					a->getPiece()->setHasPlayedTT(true);
					break;
				}
				default:
					return false;
			}
			return true;
						   }
		default: return false;
	}
	return false;
}

// TurnManager
TurnManager::TurnManager(Board& b, Renderer& r) : board(b), renderer(r) {}

Board& TurnManager::getBoard() {
	return this->board;
}

int TurnManager::getInputNumber() {
	int input = this->renderer.getInput();
	if(input >= '0' && input <= '9') {
		return input - '0';
	}
	return -1;
}

bool TurnManager::getInputCoords(unsigned int& x, unsigned int& y) {
	int input_x = this->getInputNumber();
	int input_y = this->getInputNumber();
	if(input_x < 0 || input_y < 0) {
		return false;
	}
	x = input_x;
	y = input_y;
	return true;
}

Piece& TurnManager::askPiece() {
	std::vector<Piece*> av_pieces = this->getBoard().getAvailablePiecesFromTeam(this->getBoard().getTeams()[0]->getId());
	std::stringstream prompt;
	prompt << "Select a piece :" << std::endl;
	for(int i = 1; i < av_pieces.size()+1; ++i) {
		Cell* c = this->getBoard().findCell(av_pieces[i-1]);
		int x = c->col; int y = c->row;
		prompt << i << ". " << PIECE_CNAMES.at(av_pieces[i-1]->getDisplayChar()) << "  (" << x << ", " << y << ") ";
	}
	prompt << std::endl;

	unsigned short choice = -1;
	std::string error_msg = "";
	while(!choice || choice > av_pieces.size()) {
		std::stringstream full_prompt;
		full_prompt << prompt.str() << error_msg;
		this->renderer.drawPrompt(full_prompt.str());
		std::string input = this->renderer.getInputLine();
		if(input.length() > 0 && input[0] >= '1' && input[0] <= '9') {
			choice = input[0] - '0';
		}

		if(!choice || choice > av_pieces.size()) {
			error_msg = "Incorrect Piece number.\n";
		} else {
			error_msg = "";
		}
	}

	return *av_pieces[choice-1];
}

Action* TurnManager::askAction(Piece& p) {
	Action* a = new Action();
	std::stringstream s;
	if(p.getAutorizedActions() & ACTION_PASS) 
		s << "0. Pass turn \t";
	if(p.getAutorizedActions() & ACTION_MOVE) 
		s << "1. Move \t";
	if(p.getAutorizedActions() & ACTION_MOVEANDATTACK) 
		s << "2. Move & attack \t";
	if(p.getAutorizedActions() & ACTION_GATHER) 
		s << "3. Gather GOLD \t";
	if(p.getAutorizedActions() & ACTION_SPAWN) 
		s << "4. Spawn PIECE \t";
	std::string base_prompt = s.str();

	unsigned short choice = 0;
	Cell* c = this->getBoard().findCell(&p);
	std::stringstream selected_piece_ss;
	selected_piece_ss << "Selected piece : " << p.getDisplayChar() << " " << p.getTeam() << " @ (" << c->col << ", " << c->row << "), HP : " << p.getHp();
	std::string selected_piece_info = selected_piece_ss.str();
	std::string error_msg = "";
	do {
		std::stringstream full_prompt;
		full_prompt << selected_piece_info << "\n" << base_prompt << "\n" << error_msg;
		this->renderer.drawPrompt(full_prompt.str());
		std::string input = this->renderer.getInputLine();
		if(input.length() > 0) {
			choice = input[0] - '0';
		}
		if(!(1 << choice & p.getAutorizedActions()))
			error_msg = "Action not allowed for this piece\n";
		else
			error_msg = "";
	}
	while(!(p.getAutorizedActions() & 1 << choice));

	unsigned int x, y;
	std::vector<Cell*> adj_pieces;
	unsigned short pid = -1;

	switch(1 << choice) {
		case ACTION_MOVE: 
			error_msg = "";
			do {
				std::stringstream full_prompt;
				full_prompt << selected_piece_info << "\n" << "Type coords you want to go (with a space in between)\n" << error_msg;
				this->renderer.drawPrompt(full_prompt.str());
				std::string input = this->renderer.getInputLine();
				if(sscanf(input.c_str(), "%u %u", &x, &y) != 2) {
					error_msg = "Invalid input";
					x = -1; y = -1;
				}
			} while(x < 0 || y < 0 || x > BOARD_W || y > BOARD_H);
			a->setX(x); a->setY(y);
			break;

		case ACTION_MOVEANDATTACK: {
			error_msg = "";
			do {
				std::stringstream full_prompt;
				full_prompt << selected_piece_info << "\n" << "Type coords you want to go (with a space in between)\n" << error_msg;
				this->renderer.drawPrompt(full_prompt.str());
				std::string input = this->renderer.getInputLine();
				if(sscanf(input.c_str(), "%u %u", &x, &y) != 2) {
					error_msg = "Invalid input";
					x = -1; y = -1;
				}
			} while(x < 0 || y < 0 || x > BOARD_W || y > BOARD_H);
			a->setX(x); a->setY(y);


			int dx[] = {-1, 1, 0, 0};
			int dy[] = {0, 0, -1, 1};

				for(int i = 0; i < 4; ++i) {
					Cell* c = this->getBoard().getCell(x + dx[i], y + dy[i]);
					if(c)
						if(c->getPiece() && c->getPiece()->getTeam() != p.getTeam())
							adj_pieces.push_back(c);
				}
			if(adj_pieces.size() == 0) {this->renderer.drawPrompt("Error : You cannot attack anyone from here.\n"); getch(); return nullptr;}
			else if(adj_pieces.size() == 1) { a->setTarget(adj_pieces.front()->getPiece()); }
			else {
				std::stringstream target_prompt;
				target_prompt << "Which piece do you wish to attack ?\n";
				for(int i = 0; i < adj_pieces.size(); ++i)
					target_prompt << i + 1 << ". " << PIECE_CNAMES.at(adj_pieces[i]->getPiece()->getDisplayChar()) << "  (" << x << ", " << y << ")\n";
				
				error_msg = "";
				do {
					choice = -1;
					std::stringstream full_target_prompt;
					full_target_prompt << target_prompt.str() << error_msg;
					this->renderer.drawPrompt(full_target_prompt.str());
					std::string input = this->renderer.getInputLine();
					if(input.length() > 0 && input[0] >= '1') {
						choice = input[0] - '0';
					}
					if(choice > (int)adj_pieces.size()) 
						error_msg = "Please enter a valid target\n";
					else
						error_msg = "";

				} while(choice > (int)adj_pieces.size());
				a->setTarget(adj_pieces[choice - 1]->getPiece());
			}

			break;
								   }

		case ACTION_SPAWN: {
			Spawner* s = dynamic_cast<Spawner*>(&p);
			error_msg = "";
			do {
				std::stringstream full_prompt;
				full_prompt << selected_piece_info << "\n" << "Type coords you want to go (with a space in between)\n" << error_msg;
				this->renderer.drawPrompt(full_prompt.str());
				std::string input = this->renderer.getInputLine();
				if(sscanf(input.c_str(), "%u %u", &x, &y) != 2) {
					error_msg = "Invalid input";
					x = -1; y = -1;
				}
			} while(x < 0 || y < 0 || x > BOARD_W || y > BOARD_H);

			if(s->getCanSpawn().size() == 1) {
				a->setPieceId(s->getCanSpawn().front());
			}
			else { 
				std::stringstream spawn_prompt;
				spawn_prompt << "What piece do you wish to spawn ?\n";
				for(int i = 0; i < s->getCanSpawn().size(); ++i)
					spawn_prompt << i + 1 << ". " << PIECE_NAMES.at(s->getCanSpawn()[i]) << "\n";
				
				error_msg = "";
				do {
					std::stringstream full_spawn_prompt;
					full_spawn_prompt << spawn_prompt.str() << error_msg;
					this->renderer.drawPrompt(full_spawn_prompt.str());
					std::string input = this->renderer.getInputLine();
					if(input.length() > 0 && input[0] >= '1') {
						pid = input[0] - '0';
					}
					if(pid > s->getCanSpawn().size()) 
						error_msg = "Please enter a valid piece\n";
					else
						error_msg = "";

				} while(pid > s->getCanSpawn().size());
				a->setPieceId(s->getCanSpawn()[pid-1]);
			}

			a->setX(x); a->setY(y);

			break;
		}

	}

	a->setPiece(&p);
	a->setActionId(1 << choice);
	return a;
}
