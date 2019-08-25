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

        enum GameState {Menu, Playing, Pause, Credits, GameOver};
        GameState _GameState;

        void createGui();

        tgui::Gui* gameMenu;

        void initGame(std::string strLevelName);

        void parseLevel(std::string strLevelName);

        void unloadLevel();

        enum enBlockType {Empty, Wall, Box, Destination, Player};

        int fieldWidth;
        int fieldHeight;

        enBlockType** fieldValues;
        enBlockType** currentFieldValues;

        sf::RectangleShape bgShape;
        sf::Sprite*** gridSpritesGround;
        sf::Sprite*** gridSpritesWalls;
        sf::Sprite*** gridSpritesMoveable;

        float scale;

        enum moveDirection {Up, Left, Down, Right};

        bool moveIsAllowed(moveDirection md);

        sf::Vector2i vecPlayerPosition;

        sf::Sprite bigSprite;

};

#endif // GAME_H
