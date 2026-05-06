#include "renderer.hpp"

#include "game_variables.hpp"
#include <ncurses.h>
Renderer::Renderer() {
	initscr();
	echo();
	
	int maxx, maxy;
	getmaxyx(stdscr, maxy, maxx);
	
	// Layout: 
	// status_win: top full width (0 to UI_STATUS_H)
	// board_win + history_win: middle section (UI_STATUS_H to maxy - UI_INPUT_H)
	// prompt_win: below history (maxy - UI_INPUT_H to maxy - UI_INPUT_H + UI_INPUT_H/2)
	// input_win: bottom (maxy - UI_INPUT_H + UI_INPUT_H/2 to maxy)
	
	int board_height = maxy - UI_STATUS_H - UI_INPUT_H;
	this->status_win = newwin(UI_STATUS_H, maxx, 0, 0);
	this->board_win = newwin(2*BOARD_H + 3, 2*BOARD_W + 6, UI_STATUS_H, 0);
	this->history_win = newwin(board_height, maxx - 2*BOARD_W - 6, UI_STATUS_H, 2*BOARD_W + 6);
	this->promptbox_win = newwin(UI_INPUT_H/2, maxx - 2*BOARD_W - 6, maxy - UI_INPUT_H, 2*BOARD_W + 6);
	this->prompt_win = newwin(UI_INPUT_H/2 - 2, maxx - 2*BOARD_W - 8, maxy - UI_INPUT_H + 1, 2*BOARD_W + 7);
	this->inputbox_win = newwin(UI_INPUT_H/2, maxx - 2*BOARD_W - 6, maxy - UI_INPUT_H/2, 2*BOARD_W + 6);
	this->input_win = newwin(UI_INPUT_H/2 - 2, maxx - 2*BOARD_W - 8, maxy - UI_INPUT_H/2 + 1, 2*BOARD_W + 7);

	if (has_colors() == FALSE) {
		endwin();
		printf("Your terminal doesn't support colors !\n");
		exit(1);
	}

	start_color();

	init_pair(1, COLOR_BLUE, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(6, COLOR_CYAN, COLOR_BLACK); 

	
	box(this->status_win, 0, 0);
	box(this->board_win, 0, 0);
	box(this->history_win, 0, 0);
	box(this->promptbox_win, 0, 0);
	box(this->inputbox_win, 0, 0);
	
	refresh();
	wrefresh(this->status_win);
	wrefresh(this->board_win);
	wrefresh(this->history_win);
	wrefresh(this->prompt_win);
	wrefresh(this->promptbox_win);
	wrefresh(this->input_win);
	wrefresh(this->inputbox_win);
}

Renderer::~Renderer() {
	wclear(this->board_win);
	wclear(this->status_win);
	wclear(this->history_win);
	wclear(this->inputbox_win);
	wclear(this->input_win);
	wclear(this->promptbox_win);
	wclear(this->prompt_win);
	
	delwin(this->board_win);
	delwin(this->status_win);
	delwin(this->history_win);
	delwin(this->inputbox_win);
	delwin(this->input_win);
	delwin(this->promptbox_win);
	delwin(this->prompt_win);
	
	endwin();
}

void Renderer::drawBoard(Board& b) {
	wclear(this->board_win);
	box(this->board_win, 0, 0);

	int y = 1;
	int lastY = 0;

	mvwprintw(this->board_win, y, 1, "  ");
	for(int j = 0; j < BOARD_W; ++j) {
		mvwprintw(this->board_win, y, 2*j + 3, "%d", j % 10);
	}
	y++;

	for(int i = 0; i < BOARD_H; ++i) {
		mvwprintw(this->board_win, y, 1, "%d", i);
		
		for(int j = 0; j < BOARD_W; ++j) {
			char s;
			Cell* c = b.getCell(j,i);

			if(c->getPiece() == nullptr) mvwaddch(this->board_win, y, 2*j + 3, ' ');
			else {
				s = c->getPiece()->getDisplayChar();

				int col = COLOR_PAIR(this->getColorPairForTeam(c->getPiece()->getTeam()));
				wattron(this->board_win, col);
				mvwaddch(this->board_win, y, 2*j + 3, s);
				wattroff(this->board_win, col);
			}
			
			if(j < BOARD_W - 1) {
				mvwaddch(this->board_win, y, 2*j + 4, '|');
			}
		}
		
		mvwprintw(this->board_win, y, 2*BOARD_W + 3, "%d", i);
		lastY = y;
		y++;

		if(i < BOARD_H - 1) {
			mvwprintw(this->board_win, y, 1, " ");
			for(int j = 0; j < BOARD_W; ++j) {
				mvwaddch(this->board_win, y, 2*j + 3, '-');
				if(j < BOARD_W - 1) {
					mvwaddch(this->board_win, y, 2*j + 4, '+');
				}
			}
			y++;
		}
	}

	mvwprintw(this->board_win, y, 1, "  ");
	for(int j = 0; j < BOARD_W; ++j) {
		mvwprintw(this->board_win, y, 2*j + 3, "%d", j % 10);
	}

	wrefresh(this->board_win);
	return;
}

void Renderer::drawStatus(Board& board) {
	wclear(this->status_win);
	box(this->status_win, 0, 0);
	std::deque<Team*> t = board.getTeams();
	mvwprintw(this->status_win, 1, 1, "");
	for(int i = 0; i < t.size(); ++i) {
		int col = COLOR_PAIR(this->getColorPairForTeam(t[i]->getId()));
		wattron(this->status_win, col);
		wprintw(this->status_win, "Team %c %dG\t", t[i]->getId(), t[i]->getGold());
		wattroff(this->status_win, col);
	}
	//mvwprintw(this->status_win, 1, 1, "Team %c", t.front()->getId());
	wrefresh(this->status_win);
}


void Renderer::drawAction(std::vector<Action*> history) {
	wclear(this->history_win);
	box(this->history_win, 0, 0);
	int maxy = getmaxy(this->history_win);
	int nblines = std::min(maxy/2, (int)history.size());
	int start = history.size() - nblines;
	for(int i = 0; i < nblines ; ++i) {
		int col = COLOR_PAIR(this->getColorPairForTeam(history[start + i]->getPiece()->getTeam()));
		wattron(this->history_win, col);
		mvwprintw(this->history_win, i * 2 + 1, 1, "%c %s ", history[start + i]->getPiece()->getTeam(), PIECE_CNAMES.at(history[start + i]->getPiece()->getDisplayChar()).c_str());
		wattroff(this->history_win, col);
		wprintw(this->history_win, "-> %s %s (%d, %d)", PIECE_NAMES.at(history[start + i]->getPieceId()).c_str(), ACTION_NAMES.at(history[start + i]->getActionId()).c_str(), history[start + i]->getX(), history[start + i]->getY());
	}
	wrefresh(this->history_win);
}

void Renderer::drawInput(const std::string& input) {
	wclear(this->input_win);
	mvwprintw(this->input_win, 0, 0, "%s", input.c_str());
	wrefresh(this->input_win);
}
void Renderer::drawPrompt(const std::string& prompt) {
	wclear(this->prompt_win);
	mvwprintw(this->prompt_win, 0, 0, "%s", prompt.c_str());
	wrefresh(this->prompt_win);
}

int Renderer::getInput() {
	return getch();
}

std::string Renderer::getInputLine() {
	wclear(this->input_win);
	mvwprintw(this->input_win, 0, 0, "> ");
	wrefresh(this->input_win);
	
	char buffer[256] = {0};
	wgetnstr(this->input_win, buffer, 255);
	
	return std::string(buffer);
}

int Renderer::getColorPairForTeam(char teamId) {
     int pair_id = (teamId - '0');
     if (pair_id < 1 || pair_id > 7) pair_id = 1;
     return pair_id;
 }

