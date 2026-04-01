#include "pieces.hpp"
#include "game_variables.hpp"
#include <iostream>
class Cell;

// Piece
Piece::Piece(char display_char, int cost) : display_char(display_char), cost(cost) {}
int Piece::getHp() {
	return this->hp;
} 
Piece::~Piece() {}
void Piece::setHp(int hp) {
	this->hp = hp;
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

void Piece::passTurn() {
	this->setHasPlayedTT(true);
}

// Mobile 
Mobile::Mobile(char c, int cost) : Piece(c, cost) {}
Mobile::~Mobile() {}
int Mobile::getMoveSpeed() {
	return this->movespeed;
}
void Mobile::setMoveSpeed(int movespeed) {
	this->movespeed = movespeed;
}

// Gatherer
Gatherer::Gatherer(char c, int cost) : Piece(c, cost) {}
Gatherer::~Gatherer() {}
int Gatherer::getProd() {
	return this->prod;
}
void Gatherer::setProd(int prod) {
	this->prod = prod;
}

// Fighter
Fighter::Fighter(char c, int cost) : Mobile(c, cost) {}
Fighter::~Fighter() {}
int Fighter::getPower() {
	return this->power;
}
void Fighter::setPower(int power) {
	this->power = power;
}

// Spawner
Spawner::Spawner(char c, int cost) : Piece(c, cost) {}
Spawner::~Spawner() {
	delete[] this->can_spawn;
}
Piece** Spawner::getCanSpawn() {
	return this->can_spawn;
}


/* -------------- PIECES REELLES -------------- */

// Castle
Castle::Castle() : Piece('C', PIECE_COST_CASTLE), Gatherer('C', PIECE_COST_CASTLE), Spawner('C', PIECE_COST_CASTLE) {
	this->setHp(20);
	this->setProd(2);
	can_spawn = new Piece*[3];
	can_spawn[0] = new Farmer();
	can_spawn[1] = new Warrior();
	can_spawn[2] = new Lord();
}
void Castle::sayUniqueLine() {
	std::cout << "« Haha i'm safe inside those walls ! »" << std::endl;
}

Lord::Lord() : Piece('L', PIECE_COST_LORD), Fighter('L', PIECE_COST_LORD), Spawner('L', PIECE_COST_LORD) {
	//can_spawn[0] = new Castle();
	this->setPower(3);
	this->setHp(5);
	this->setMoveSpeed(1);
}
void Lord::sayUniqueLine() {
	std::cout << " « Protect me, my fellow warriors ! »" << std::endl;
}

Warrior::Warrior() : Piece('W', PIECE_COST_WARRIOR), Fighter('W', PIECE_COST_WARRIOR) {
	this->setPower(3);
	this->setHp(10);
	this->setMoveSpeed(3);
}
void Warrior::sayUniqueLine() {
	std::cout << "« Ready to follow orders ! »" << std::endl;
}

Farmer::Farmer() : Piece('F', PIECE_COST_FARMER), Gatherer('F', PIECE_COST_FARMER), Mobile('F', PIECE_COST_FARMER) {
	this->setHp(1);
	this->setMoveSpeed(2);
	this->setProd(5);
}
void Farmer::sayUniqueLine() {
	std::cout << "« I'm doing my best to help my beloved king ! »" << std::endl;
}
