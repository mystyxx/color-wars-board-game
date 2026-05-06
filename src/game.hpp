#ifndef GAME_HPP
#define GAME_HPP 

#pragma once
#include "pieces.hpp"
#include "game_variables.hpp"
#include <deque>
#include <fstream>
#include <sstream>
class Renderer;
#endif

class Team {
	char id;
	int gold;
	public:
		Team(char id, int gold); 
		char getId();
		int getGold(); 
		void setGold(int);
		bool addGold(int);
};

class Action {
	action_id action;
	unsigned int x;
	unsigned int y;
	piece_id pid;
	Piece* piece;
	Piece* target;

	public:
		Action(Piece* piece = nullptr, action_id action = ACTION_PASS, unsigned int x = 0, unsigned int y = 0, piece_id pid = PIECE_NONE);
		action_id getActionId(); void setActionId(action_id);
		unsigned int getX(); void setX(unsigned int);
		unsigned int getY(); void setY(unsigned int);
		piece_id getPieceId(); void setPieceId(piece_id);
		Piece* getPiece(); void setPiece(Piece*);
		Piece* getTarget(); void setTarget(Piece*);
};

class Cell {
	Piece* piece;
	public:
		Piece* getPiece();
		void setPiece(Piece* p);
		unsigned int row, col;
};

class Board {
	Cell** cells;
	int width;
	int height;
	std::deque<Team*> teams;
	Renderer* renderer;
	public:
		Board(std::deque<Team*>, int w = BOARD_W, int h = BOARD_H, Renderer* r = nullptr);
		~Board();
		Cell* getCell(int r, int c);
		Cell* findCell(Piece* piece);
		std::deque<Team*>& getTeams();
		Team* getTeam(char);
		std::vector<Piece*> getPiecesFromTeam(char team);
		std::vector<Piece*> getAvailablePiecesFromTeam(char team);
		int manhattanDist(Cell c1, Cell c2);
		void printBoard();
		bool handleAction(Action*);
		int getWidth() const;
		int getHeight() const;
};

class TurnManager {
	Board& board;
	Renderer& renderer;
	
	int getInputNumber();
	bool getInputCoords(unsigned int& x, unsigned int& y);
	
	public:
		TurnManager(Board&, Renderer&);
		Board& getBoard();
		Piece& askPiece();
		Action* askAction(Piece& piece);
		void save(const char* filename);
		void load(const char* filename);
		void showSaveLoadMenu();
};
