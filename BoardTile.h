#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
class BoardTile {
	private:
		sf::Sprite _bg;
		sf::Sprite _fg;
		sf::Sprite _flagImage;
		sf::Sprite _mineImage;
		sf::Sprite _adjacentImage;
		bool _debug;
		bool _mine;
		bool _revealed;
		bool _flag;
		int _adjacent;
	public:
		BoardTile(std::map<std::string, sf::Texture> &gameImages, int, int);
		void DisplayTile(sf::RenderWindow &gameWindow);
		void SetMine(bool);
		void SetFlag(bool);
		void SetDebug(bool);
		void SetRevealed(bool);
		void SetAdjacent(int, std::map<std::string, sf::Texture> &gameImages);
		bool GetMine();
		bool GetFlag();
		bool GetDebug();
		bool GetRevealed();
		int GetAdjacent();
};