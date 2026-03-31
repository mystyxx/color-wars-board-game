#include "pieces.hpp"
class Cell;

// Piece
int Piece::getHp() {
	return this->hp;
} 
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
int Mobile::getMoveSpeed() {
	return this->movespeed;
}
void Mobile::setMoveSpeed(int movespeed) {
	this->movespeed = movespeed;
}

// Gatherer
int Gatherer::getProd() {
	return this->prod;
}
void Gatherer::setProd(int prod) {
	this->prod = prod;
}

// Fighter
int Fighter::getPower() {
	return this->power;
}
void Fighter::setPower(int power) {
	this->power = power;
}

// Spawner
Piece** Spawner::getCanSpawn() {
	return this->can_spawn;
}

/* -------------- PIECES REELLES -------------- */

// Castle
Castle::Castle() {
	this->setHp(20);
	this->setCost(15);
	this->setProd(2);
}
~Castle();

Lord();
~Lord();

Warrior();
~Warrior();

Farmer();
~Farmer();
