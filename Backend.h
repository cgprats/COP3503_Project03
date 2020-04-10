#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <vector>
#include "BoardTile.h"

class Backend {
	private:
		int _mineCount;
		int _flagCount;
		int _numRevealed;
		std::map<std::string, sf::Texture> _gameImages;
		std::vector<BoardTile> _boardTiles;
		sf::Sprite _debug;
		sf::Sprite _status;
		sf::Sprite _testOne;
		sf::Sprite _testTwo;
		sf::Sprite _testThree;
		sf::Sprite _negativesPlaceTicker;
		sf::Sprite _hundredsPlaceTicker;
		sf::Sprite _tensPlaceTicker;
		sf::Sprite _onesPlaceTicker;
		bool _gameEnded;
		bool _gameVictory;
		std::vector<int> _randomizedMines;
		std::vector<int> _testBoardOne;
		std::vector<int> _testBoardTwo;
		std::vector<int> _testBoardThree;
	public:
		Backend();
		void DisplayGame(sf::RenderWindow &gameWindow);
		void Refresh(sf::RenderWindow &gameWindow);
		void LoadTextures();
		void InitializeBoard(std::map<std::string, sf::Texture> &gameImages);
		void GameStatus();
		std::vector<int> RandomizeMines();
		std::vector<int> LoadBoardFromFile(std::string);
		void SetMines(std::vector<int>);
		void CalculateAdjecency(std::vector<int>);
		void DisplayAdjacent(int);
};