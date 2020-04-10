#include <SFML/Graphics.hpp>
#include <iostream>
#include "Backend.h"

int main() {
    sf::RenderWindow gameWindow(sf::VideoMode(800, 600), "Minesweeper Game");
    Backend minesweeperBackend;
    while (gameWindow.isOpen()) {
        sf::Event windowEvent;
        while (gameWindow.pollEvent(windowEvent)) {
            if (windowEvent.type == sf::Event::Closed) {
                gameWindow.close();
            }
        }
        minesweeperBackend.DisplayGame(gameWindow);
        gameWindow.display();
    }
    return 0;
}
