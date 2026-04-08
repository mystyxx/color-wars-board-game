#ifndef RENDERER_HPP
#define RENDERER_HPP

#pragma once
#include <ncurses.h>
#include <panel.h>
#include "game_variables.hpp"
#include <iostream>
#include "game.hpp"

class Renderer {
	WINDOW* board_win;
	WINDOW* status_win;
	WINDOW* history_win;
	WINDOW* inputbox_win;
	WINDOW* input_win;
	WINDOW* promptbox_win;
	WINDOW* prompt_win;

	public: 
	Renderer();
	~Renderer();
	void drawBoard(Board&);
	void drawStatus(Board&);
	void drawAction(std::vector<Action*>);
	void drawInput(const std::string&);
	void drawPrompt(const std::string&);
	void waitForContinue();
	int getInput();
	std::string getInputLine();
	int getColorPairForTeam(char);
};

#endif
