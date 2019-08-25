#ifndef GAME_H
#define GAME_H
#include <TGUI/TGUI.hpp>
#include <stdio.h>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <Configuration.h>
class Game
{
 public:
        Game();
        Game(const Game& other)=delete;
        Game& operator=(const Game& other)=delete;
        ~Game();
        void run(int frames_per_second);


    protected:

    private:
        sf::RenderWindow _window;

        void processEvents();
        void update (sf::Time deltaTime);
        void render();

        enum GameState {Menu, Playing, Credits, GameOver};
        GameState _GameState;

        void createGui();

        tgui::Gui* gameMenu;

        void initGame(std::string strLevelName);

        void parseLevel(std::string strLevelName);

        enum enBlockType {Empty, Wall, Box, Destination, Player};

        int fieldWidth;
        int fieldHeight;

        enBlockType** fieldValues;

        sf::RectangleShape bgShape;
        sf::Sprite*** gridSprites;

        float scale;
};

#endif // GAME_H
