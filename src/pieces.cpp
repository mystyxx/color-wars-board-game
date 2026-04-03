#include "pieces.hpp"
#include "game_variables.hpp"
#include <iostream>
class Cell;

// Piece
Piece::Piece(char team, char display_char, int cost) : team(team), display_char(display_char), cost(cost), autorized_actions(ACTION_PASS) {}
int Piece::getHp() {
	return this->hp;
} 
Piece::~Piece() {}
void Piece::setHp(int hp) {
	this->hp = hp;
}

void Piece::addPermission(action_id action) {
	this->autorized_actions = this->autorized_actions | action;
}

int Piece::getCost() {
	return this->cost;
}

char Piece::getDisplayChar() {
	return this->display_char;
}

char Piece::getTeam() {
	return this->team;
} 

void Piece::setTeam(char team) {
	this->team = team;
}

int Piece::getHasPlayedTT() {
	return has_played_tt;
}

void Piece::setHasPlayedTT(int has_played_tt) {
	this->has_played_tt = has_played_tt;
}

action_id Piece::getAutorizedActions() {
	return this->autorized_actions;
}

void Piece::passTurn() {
	this->setHasPlayedTT(true);
}

// Mobile 
Mobile::Mobile(char team, char c, int cost) : Piece(team, c, cost) {
	this->addPermission(ACTION_MOVE);
}
Mobile::~Mobile() {}
int Mobile::getMoveSpeed() {
	return this->movespeed;
}
void Mobile::setMoveSpeed(int movespeed) {
	this->movespeed = movespeed;
}

// Gatherer
Gatherer::Gatherer(char team, char c, int cost) : Piece(team, c, cost) {
	this->addPermission(ACTION_GATHER);
}
Gatherer::~Gatherer() {}
int Gatherer::getProd() {
	return this->prod;
}
void Gatherer::setProd(int prod) {
	this->prod = prod;
}

// Fighter
Fighter::Fighter(char team, char c, int cost) : Mobile(team, c, cost) {
	this->addPermission(ACTION_MOVEANDATTACK);
}
Fighter::~Fighter() {}
int Fighter::getPower() {
	return this->power;
}
void Fighter::setPower(int power) {
	this->power = power;
}

// Spawner
Spawner::Spawner(char team, char c, int cost) : Piece(team, c, cost) {
	this->addPermission(ACTION_SPAWN);
}

std::vector<piece_id> Spawner::getCanSpawn() {
	return this->can_spawn;
}


/* -------------- PIECES REELLES -------------- */

// Castle
Castle::Castle(char team) : Piece(team, 'C', PIECE_COST_CASTLE), Gatherer(team, 'C', PIECE_COST_CASTLE), Spawner(team, 'C', PIECE_COST_CASTLE) {
	this->setHp(20);
	this->setProd(2);
	can_spawn.push_back(PIECE_FARMER);
	can_spawn.push_back(PIECE_WARRIOR);
	can_spawn.push_back(PIECE_LORD);
}
void Castle::sayUniqueLine() {
	std::cout << "« Haha i'm safe inside those walls ! »" << std::endl;
}

Lord::Lord(char team) : Piece(team, 'L', PIECE_COST_LORD), Fighter(team, 'L', PIECE_COST_LORD), Spawner(team, 'L', PIECE_COST_LORD) {
	this->setPower(3);
	this->setHp(5);
	this->setMoveSpeed(1);
	can_spawn.push_back(PIECE_CASTLE);
}
void Lord::sayUniqueLine() {
	std::cout << " « Protect me, my fellow warriors ! »" << std::endl;
}

Warrior::Warrior(char team) : Piece(team, 'W', PIECE_COST_WARRIOR), Fighter(team, 'W', PIECE_COST_WARRIOR) {
	this->setPower(3);
	this->setHp(10);
	this->setMoveSpeed(3);
}
void Warrior::sayUniqueLine() {
	std::cout << "« Ready to follow orders ! »" << std::endl;
}

Farmer::Farmer(char team) : Piece(team, 'F', PIECE_COST_FARMER), Gatherer(team, 'F', PIECE_COST_FARMER), Mobile(team, 'F', PIECE_COST_FARMER) {
	this->setHp(1);
	this->setMoveSpeed(2);
	this->setProd(5);
}
void Farmer::sayUniqueLine() {
	std::cout << "« I'm doing my best to help my beloved king ! »" << std::endl;
}
