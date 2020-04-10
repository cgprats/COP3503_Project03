#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include "Backend.h"
#include "BoardTile.h"

Backend::Backend() {
	this->LoadTextures();
	this->InitializeBoard(_gameImages);
	_debug.setTexture(_gameImages["debug"]);
	_debug.setPosition(496, 512);
	_gameEnded = false;
	_gameVictory = false;
	this->GameStatus();
	_status.setPosition(368, 512);
	_randomizedMines = this->RandomizeMines();
	this->CalculateAdjecency(_randomizedMines);
	this->SetMines(_randomizedMines);
	_testBoardOne = this->LoadBoardFromFile("boards/testboard1.brd");
	_testOne.setTexture(_gameImages["test_1"]);
	_testOne.setPosition(560, 512);
	_testBoardTwo = this->LoadBoardFromFile("boards/testboard2.brd");
	_testTwo.setTexture(_gameImages["test_2"]);
	_testTwo.setPosition(624, 512);
	_testBoardThree = this->LoadBoardFromFile("boards/testboard3.brd");
	_testThree.setPosition(688, 512);
	_testThree.setTexture(_gameImages["test_3"]);
	_negativesPlaceTicker.setTexture(_gameImages["digits"]);
	_negativesPlaceTicker.setTextureRect(sf::Rect<int>(210, 0, 21, 32));
	_hundredsPlaceTicker.setTexture(_gameImages["digits"]);
	_tensPlaceTicker.setTexture(_gameImages["digits"]);
	_onesPlaceTicker.setTexture(_gameImages["digits"]);
}
void Backend::DisplayGame(sf::RenderWindow &gameWindow) {
		gameWindow.clear();
		this->Refresh(gameWindow);
		gameWindow.draw(_debug);
		gameWindow.draw(_status);
		gameWindow.draw(_testOne);
		gameWindow.draw(_testTwo);
		gameWindow.draw(_testThree);
		for (unsigned int i = 0; i < _boardTiles.size(); i++) {
			_boardTiles[i].DisplayTile(gameWindow);
		}
		if (_flagCount < 0) {
			_negativesPlaceTicker.setPosition(0, 512);
			_hundredsPlaceTicker.setPosition(21, 512);
			_tensPlaceTicker.setPosition(42, 512);
			_onesPlaceTicker.setPosition(63, 512);
			gameWindow.draw(_negativesPlaceTicker);
		}
		else {
			_hundredsPlaceTicker.setPosition(0, 512);
			_tensPlaceTicker.setPosition(21, 512);
			_onesPlaceTicker.setPosition(42, 512);
		}
		gameWindow.draw(_hundredsPlaceTicker);
		gameWindow.draw(_tensPlaceTicker);
		gameWindow.draw(_onesPlaceTicker);
}
void Backend::Refresh(sf::RenderWindow &gameWindow) {
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2i clickPos = sf::Mouse::getPosition(gameWindow);
		if (clickPos.y >= 0 && clickPos.y < 512 && !_gameEnded) {
			int clickedTile = ((clickPos.y / 32) * 25) + (clickPos.x / 32);
			if (!_boardTiles[clickedTile].GetRevealed()) {
				_boardTiles[clickedTile].SetRevealed(true);
				if (_boardTiles[clickedTile].GetAdjacent() == 0) {
					this->DisplayAdjacent(clickedTile);
				}
			}
			if (_boardTiles[clickedTile].GetMine() && !_boardTiles[clickedTile].GetFlag()) {
				_gameEnded = true;
				_gameVictory = false;
				for (int i = 0; i < _boardTiles.size(); i++) {
					if (_boardTiles[i].GetMine()) {
						_boardTiles[i].SetRevealed(true);
					}
				}
			}
		}
		else if (clickPos.y >= 512) {
			if (clickPos.x >= 496 && clickPos.x < 560) {
				for (unsigned int i = 0; i < _boardTiles.size(); i++) {
					if (!_boardTiles[i].GetDebug()) {
						_boardTiles[i].SetDebug(true);
					}
					else {
						_boardTiles[i].SetDebug(false);
					}
				}
			}
			else if (clickPos.x >= 560 && clickPos.x < 624) {
				_gameEnded = false;
				this->CalculateAdjecency(_testBoardOne);
				this->SetMines(_testBoardOne);
				_mineCount = 0;
				for (unsigned int i = 0; i < _boardTiles.size(); i++) {
					_boardTiles[i].SetRevealed(false);
					_boardTiles[i].SetFlag(false);
					if (_testBoardOne[i] == 1) {
						_mineCount++;
					}
				}
				_flagCount = _mineCount;
				_gameEnded = false;
			}
			else if (clickPos.x >= 624 && clickPos.x < 688) {
				_gameEnded = false;
				this->CalculateAdjecency(_testBoardTwo);
				this->SetMines(_testBoardTwo);
				_mineCount = 0;
				for (unsigned int i = 0; i < _boardTiles.size(); i++) {
					_boardTiles[i].SetRevealed(false);
					_boardTiles[i].SetFlag(false);
					if (_testBoardTwo[i] == 1) {
						_mineCount++;
					}
				}
				_flagCount = _mineCount;
				_gameEnded = false;
			}
			else if (clickPos.x >= 688 && clickPos.x < 752) {
				_gameEnded = false;
				this->CalculateAdjecency(_testBoardThree);
				this->SetMines(_testBoardThree);
				_mineCount = 0;
				for (unsigned int i = 0; i < _boardTiles.size(); i++) {
					_boardTiles[i].SetRevealed(false);
					_boardTiles[i].SetFlag(false);
					if (_testBoardThree[i] == 1) {
						_mineCount++;
					}
				}
				_flagCount = _mineCount;
				_gameEnded = false;
			}
			else if (clickPos.x >= 368 && clickPos.x < 432) {
				_gameEnded = false;
				_randomizedMines = this->RandomizeMines();
				this->CalculateAdjecency(_randomizedMines);
				this->SetMines(_randomizedMines);
				for (unsigned int i = 0; i < _boardTiles.size(); i++) {
					_boardTiles[i].SetRevealed(false);
					_boardTiles[i].SetFlag(false);
				}
				_gameEnded = false;
			}
		}
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !_gameEnded) {
		sf::Vector2i clickPos = sf::Mouse::getPosition(gameWindow);
		if (clickPos.y >= 0 && clickPos.y < 512) {
			int clickedTile = ((clickPos.y / 32) * 25) + (clickPos.x / 32);
			if (_boardTiles[clickedTile].GetFlag()) {
				_boardTiles[clickedTile].SetFlag(false);
				_flagCount++;
			}
			else {
				_boardTiles[clickedTile].SetFlag(true);
				_flagCount--;
			}
		}
	}
	std::string temp;
	if (_flagCount < 0) {
		temp = std::to_string(_flagCount * -1);
	}
	else {
		temp = std::to_string(_flagCount);
	}
	while (temp.size() < 3) {
		temp = '0' + temp;
	}
	std::string tempInt;
	tempInt = temp[0];
	_hundredsPlaceTicker.setTextureRect(sf::Rect<int>(21 * stoi(tempInt), 0, 21, 32));
	tempInt = temp[1];
	_tensPlaceTicker.setTextureRect(sf::Rect<int>(21 * stoi(tempInt), 0, 21, 32));
	tempInt = temp[2];
	_onesPlaceTicker.setTextureRect(sf::Rect<int>(21 * stoi(tempInt), 0, 21, 32));
	_numRevealed = 0;
	for (int i = 0; i < _boardTiles.size(); i++) {
		if (_boardTiles[i].GetRevealed()) {
			_numRevealed++;
		}
	}
	if (_numRevealed == (_boardTiles.size() - _mineCount)) {
		_gameVictory = true;
		_gameEnded = true;
	}
	this->GameStatus();
}
void Backend::LoadTextures() {
	sf::Texture tempFile;
	tempFile.loadFromFile("images/debug.png");
	_gameImages.emplace("debug", tempFile);
	tempFile.loadFromFile("images/digits.png");
	_gameImages.emplace("digits", tempFile);
	tempFile.loadFromFile("images/face_happy.png");
	_gameImages.emplace("face_happy", tempFile);
	tempFile.loadFromFile("images/face_lose.png");
	_gameImages.emplace("face_lose", tempFile);
	tempFile.loadFromFile("images/face_win.png");
	_gameImages.emplace("face_win", tempFile);
	tempFile.loadFromFile("images/flag.png");
	_gameImages.emplace("flag", tempFile);
	tempFile.loadFromFile("images/mine.png");
	_gameImages.emplace("mine", tempFile);
	tempFile.loadFromFile("images/number_1.png");
	_gameImages.emplace("number_1", tempFile);
	tempFile.loadFromFile("images/number_2.png");
	_gameImages.emplace("number_2", tempFile);
	tempFile.loadFromFile("images/number_3.png");
	_gameImages.emplace("number_3", tempFile);
	tempFile.loadFromFile("images/number_4.png");
	_gameImages.emplace("number_4", tempFile);
	tempFile.loadFromFile("images/number_5.png");
	_gameImages.emplace("number_5", tempFile);
	tempFile.loadFromFile("images/number_6.png");
	_gameImages.emplace("number_6", tempFile);
	tempFile.loadFromFile("images/number_7.png");
	_gameImages.emplace("number_7", tempFile);
	tempFile.loadFromFile("images/number_8.png");
	_gameImages.emplace("number_8", tempFile);
	tempFile.loadFromFile("images/test_1.png");
	_gameImages.emplace("test_1", tempFile);
	tempFile.loadFromFile("images/test_2.png");
	_gameImages.emplace("test_2", tempFile);
	tempFile.loadFromFile("images/test_3.png");
	_gameImages.emplace("test_3", tempFile);
	tempFile.loadFromFile("images/tile_hidden.png");
	_gameImages.emplace("tile_hidden", tempFile);
	tempFile.loadFromFile("images/tile_revealed.png");
	_gameImages.emplace("tile_revealed", tempFile);
}
void Backend::InitializeBoard(std::map<std::string, sf::Texture> &gameImages) {
	int x = 0;
	int y = 0;
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 25; j++) {
			BoardTile temp = BoardTile(_gameImages, x, y);
			_boardTiles.push_back(temp);
			x += 32;
		}
		x = 0;
		y += 32;
	}
}
void Backend::GameStatus() {
	if (_gameEnded) {
		if (_gameVictory) {
			_status.setTexture(_gameImages["face_win"]);
		}
		else {
			_status.setTexture(_gameImages["face_lose"]);
		}
	}
	else {
		_status.setTexture(_gameImages["face_happy"]);
	}
}
std::vector<int> Backend::RandomizeMines() {
	std::vector<int> randomizedMines;
	int mineCount = 0;
	int mineVal = 0;
	int mineTile = 0;
	srand(time(NULL)); 
	for (int i = 0; i < 400; i++) {
		mineVal = rand() % 2;
		randomizedMines.push_back(0);
	}
	while (mineCount < 50) {
		mineTile = rand() % 400;
		if (randomizedMines[mineTile] != 1) {
			randomizedMines[mineTile] = 1;
			mineCount++;
		}
	}
	_mineCount = 0;
	for (int i = 0; i < randomizedMines.size(); i++) {
		if (randomizedMines[i] == 1) {
			_mineCount++;
		}
	}
	_flagCount = _mineCount;
	return randomizedMines;
}
std::vector<int> Backend::LoadBoardFromFile(std::string fileName) {
	std::ifstream file;
	file.open(fileName);
	std::vector<int> data;
	std::string parseData;
	std::string parsedData;
	while (!file.eof()) {
		getline(file, parseData);
		for (unsigned int i = 0; i < parseData.size(); i++) {
			parsedData = parseData[i];
			data.push_back(std::stoi(parsedData));
		}
	}
	return data;
}
void Backend::SetMines(std::vector<int> boardVector) {
	for (unsigned int i = 0; i < boardVector.size(); i++) {
		if (boardVector[i] == 0) {
			_boardTiles[i].SetMine(false);
		}
		else if (boardVector[i] == 1) {
			_boardTiles[i].SetMine(true);
		}
	}
}
void Backend::CalculateAdjecency(std::vector<int> boardVector) {
	int adjacentCount = 0;
	for (int i = 0; i < boardVector.size(); i++) {
		adjacentCount = 0;
		if (i == 0) {
			if (boardVector[i + 1] == 1) {
				adjacentCount++;
			}
			if (boardVector[i + 25] == 1) {
				adjacentCount++;
			}
			if (boardVector[i + 26] == 1) {
				adjacentCount++;
			}
		}
		else if (i < 24 && i > 0) {
			if (boardVector[i - 1] == 1) {
				adjacentCount++;
			}
			if (boardVector[i + 1] == 1) {
				adjacentCount++;
			}
			if (boardVector[i + 24] == 1) {
				adjacentCount++;
			}
			if (boardVector[i + 25] == 1) {
				adjacentCount++;
			}
			if (boardVector[i + 26] == 1) {
				adjacentCount++;
			}
		}
		else if (i == 24) {
			if (boardVector[i - 1] == 1) {
				adjacentCount++;
			}
			if (boardVector[i + 24] == 1) {
				adjacentCount++;
			}
			if (boardVector[i + 25] == 1) {
				adjacentCount++;
			}
		}
		else if (i % 25 == 0 && (i + 25) < boardVector.size()) {
			if (boardVector[i + 1] == 1) {
				adjacentCount++;
			}
			if (boardVector[i + 25] == 1) {
				adjacentCount++;
			}
			if (boardVector[i + 26] == 1) {
				adjacentCount++;
			}
			if (boardVector[i - 25] == 1) {
				adjacentCount++;
			}
			if (boardVector[i - 24] == 1) {
				adjacentCount++;
			}
		}
		else if (((i - 24) % 25) == 0 && (i + 1) < boardVector.size()) {
			if (boardVector[i - 1] == 1) {
				adjacentCount++;
			}
			if (boardVector[i - 25] == 1) {
				adjacentCount++;
			}
			if (boardVector[i - 26] == 1) {
				adjacentCount++;
			}
			if (boardVector[i + 24] == 1) {
				adjacentCount++;
			}
			if (boardVector[i + 25] == 1) {
				adjacentCount++;
			}
		}
		else if (i > (boardVector.size() - 25) && (i + 1) < boardVector.size()) {
			if (boardVector[i - 1] == 1) {
				adjacentCount++;
			}
			if (boardVector[i + 1] == 1) {
				adjacentCount++;
			}
			if (boardVector[i - 26] == 1) {
				adjacentCount++;
			}
			if (boardVector[i - 25] == 1) {
				adjacentCount++;
			}
			if (boardVector[i - 24] == 1) {
				adjacentCount++;
			}
		}
		else if (i == (boardVector.size() - 25)) {
			if (boardVector[i + 1] == 1) {
				adjacentCount++;
			}
			if (boardVector[i - 25] == 1) {
				adjacentCount++;
			}
			if (boardVector[i - 24] == 1) {
				adjacentCount++;
			}
		}
		else if (i == (boardVector.size() - 1)) {
			if (boardVector[i - 1] == 1) {
				adjacentCount++;
			}
			if (boardVector[i - 25] == 1) {
				adjacentCount++;
			}
			if (boardVector[i - 26] == 1) {
				adjacentCount++;
			}
		}
		else {
			if (boardVector[i - 1] == 1) {
				adjacentCount++;
			}
			if (boardVector[i + 1] == 1) {
				adjacentCount++;
			}
			if (boardVector[i - 26] == 1) {
				adjacentCount++;
			}
			if (boardVector[i - 25] == 1) {
				adjacentCount++;
			}
			if (boardVector[i - 24] == 1) {
				adjacentCount++;
			}
			if (boardVector[i + 24] == 1) {
				adjacentCount++;
			}
			if (boardVector[i + 25] == 1) {
				adjacentCount++;
			}
			if (boardVector[i + 26] == 1) {
				adjacentCount++;
			}
		}
		_boardTiles[i].SetAdjacent(adjacentCount, _gameImages);
	}
}
void Backend::DisplayAdjacent(int revealedTile) {
	int reveal = revealedTile - 26;
	if (reveal >= 0 && !_boardTiles[reveal].GetMine() && !_boardTiles[reveal].GetFlag() && !_boardTiles[reveal].GetRevealed()) {
		_boardTiles[reveal].SetRevealed(true);
		if (_boardTiles[reveal].GetAdjacent() == 0 && (reveal % 25) > 0 && (((reveal - 24) % 25) > 0) && reveal > 24 && reveal < 376) {
			this->DisplayAdjacent(reveal);
		}
	}
	reveal = revealedTile - 25;
	if (reveal >= 0 && !_boardTiles[reveal].GetMine() && !_boardTiles[reveal].GetFlag() && !_boardTiles[reveal].GetRevealed()) {
		_boardTiles[reveal].SetRevealed(true);
		if (_boardTiles[reveal].GetAdjacent() == 0 && (reveal % 25) > 0 && (((reveal - 24) % 25) > 0) && reveal > 24 && reveal < 376) {
			this->DisplayAdjacent(reveal);
		}
	}
	reveal = revealedTile - 24;
	if (reveal >= 0 && !_boardTiles[reveal].GetMine() && !_boardTiles[reveal].GetFlag() && !_boardTiles[reveal].GetRevealed()) {
		_boardTiles[reveal].SetRevealed(true);
		if (_boardTiles[reveal].GetAdjacent() == 0 && (reveal % 25) > 0 && (((reveal - 24) % 25) > 0) && reveal > 24 && reveal < 376) {
			this->DisplayAdjacent(reveal);
		}
	}
	reveal = revealedTile - 1;
	if (reveal >= 0 && !_boardTiles[reveal].GetMine() && !_boardTiles[reveal].GetFlag() && !_boardTiles[reveal].GetRevealed()) {
		_boardTiles[reveal].SetRevealed(true);
		if (_boardTiles[reveal].GetAdjacent() == 0 && (reveal % 25) > 0 && (((reveal - 24) % 25) > 0) && reveal > 24 && reveal < 376) {
			this->DisplayAdjacent(reveal);
		}
	}
	reveal = revealedTile + 1;
	if (reveal < _boardTiles.size() && !_boardTiles[reveal].GetMine() && !_boardTiles[reveal].GetFlag() && !_boardTiles[reveal].GetRevealed()) {
		_boardTiles[reveal].SetRevealed(true);
		if (_boardTiles[reveal].GetAdjacent() == 0 && (reveal % 25) > 0 && (((reveal - 24) % 25) > 0) && reveal > 24 && reveal < 376) {
			this->DisplayAdjacent(reveal);
		}
	}
	reveal = revealedTile + 24;
	if (reveal < _boardTiles.size() && !_boardTiles[reveal].GetMine() && !_boardTiles[reveal].GetFlag() && !_boardTiles[reveal].GetRevealed()) {
		_boardTiles[reveal].SetRevealed(true);
		if (_boardTiles[reveal].GetAdjacent() == 0 && (reveal % 25) > 0 && (((reveal - 24) % 25) > 0) && reveal > 24 && reveal < 376) {
			this->DisplayAdjacent(reveal);
		}
	}
	reveal = revealedTile + 25;
	if (reveal < _boardTiles.size() && !_boardTiles[reveal].GetMine() && !_boardTiles[reveal].GetFlag() && !_boardTiles[reveal].GetRevealed()) {
		_boardTiles[reveal].SetRevealed(true);
		if (_boardTiles[reveal].GetAdjacent() == 0 && (reveal % 25) > 0 && (((reveal - 24) % 25) > 0) && reveal > 24 && reveal < 376) {
			this->DisplayAdjacent(reveal);
		}
	}
	reveal = revealedTile + 26;
	if (reveal < _boardTiles.size() && !_boardTiles[reveal].GetMine() && !_boardTiles[reveal].GetFlag() && !_boardTiles[reveal].GetRevealed()) {
		_boardTiles[reveal].SetRevealed(true);
		if (_boardTiles[reveal].GetAdjacent() == 0 && (reveal % 25) > 0 && (((reveal - 24) % 25) > 0) && reveal > 24 && reveal < 376) {
			this->DisplayAdjacent(reveal);
		}
	}
}