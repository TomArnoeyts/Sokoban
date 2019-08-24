#ifndef GAME_H
#define GAME_H
#include <TGUI/TGUI.hpp>

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

};

#endif // GAME_H