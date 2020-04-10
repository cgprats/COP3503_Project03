#include <SFML/Graphics.hpp>
#include <iostream>
#include "BoardTile.h"

BoardTile::BoardTile(std::map<std::string, sf::Texture> &gameImages, int x, int y) {
	_debug = false;
	_mine = false;
	_revealed = false;
	_flag = false;
	_adjacent = 0;
	_bg.setPosition(x, y);
	_fg.setPosition(x, y);
	_mineImage.setPosition(x, y);
	_flagImage.setPosition(x, y);
	_adjacentImage.setPosition(x, y);
	_bg.setTexture(gameImages["tile_revealed"]);
	_fg.setTexture(gameImages["tile_hidden"]);
	_mineImage.setTexture(gameImages["mine"]);
	_flagImage.setTexture(gameImages["flag"]);
}
void BoardTile::DisplayTile(sf::RenderWindow &gameWindow) {
	gameWindow.draw(_bg);
	if (!_revealed) {
		gameWindow.draw(_fg);
		if (_flag) {
			gameWindow.draw(_flagImage);
		}
	}
	if (_revealed && _adjacent > 0) {
		gameWindow.draw(_adjacentImage);
	}
	if (_mine && _revealed) {
		gameWindow.draw(_mineImage);
	}
	if (_debug) {

		if (!_revealed) {
			gameWindow.draw(_fg);
		}		
		if (_mine) {
			gameWindow.draw(_mineImage);
		}
		if (_flag) {
			gameWindow.draw(_flagImage);
		}
	}
}
void BoardTile::SetMine(bool mine) {
	_mine = mine;
}
void BoardTile::SetFlag(bool flag) {
	_flag = flag;
	if (_revealed) {
		_flag = false;
	}
}
void BoardTile::SetDebug(bool debug) {
	_debug = debug;
}
void BoardTile::SetRevealed(bool revealed) {
	if (!_flag) {
		_revealed = revealed;
	}
	else if (_flag) {
		_revealed = false;
	}
}
void BoardTile::SetAdjacent(int adjacent, std::map<std::string, sf::Texture> &gameImages) {
	_adjacent = adjacent;
	switch (_adjacent) {
		case 1:
			_adjacentImage.setTexture(gameImages["number_1"]);
			break;
		case 2:
			_adjacentImage.setTexture(gameImages["number_2"]);
			break;
		case 3:
			_adjacentImage.setTexture(gameImages["number_3"]);
			break;
		case 4:
			_adjacentImage.setTexture(gameImages["number_4"]);
			break;
		case 5:
			_adjacentImage.setTexture(gameImages["number_5"]);
			break;
		case 6:
			_adjacentImage.setTexture(gameImages["number_6"]);
			break;
		case 7:
			_adjacentImage.setTexture(gameImages["number_7"]);
			break;
		case 8:
			_adjacentImage.setTexture(gameImages["number_8"]);
			break;
	}
}
bool BoardTile::GetMine() {
	return _mine;
}
bool BoardTile::GetFlag() {
	return _flag;
}
bool BoardTile::GetDebug() {
	return _debug;
}
bool BoardTile::GetRevealed() {
	return _revealed;
}
int BoardTile::GetAdjacent() {
	return _adjacent;
}