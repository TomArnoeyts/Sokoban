#include "Game.h"

Game::Game():_window(sf::VideoMode(800,600), "Sokoban")
{

    _GameState=GameState::Menu;

    gameMenu=new tgui::Gui(_window);

    createGui();

}

Game::~Game()
{

}
void Game::createGui()
{
    gameMenu->loadWidgetsFromFile("resources\\Menus\\MainMenu.txt");
}

void Game::run(int frames_per_second)
{
    //initGame();
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time TimePerFrame=sf::seconds(1.f/frames_per_second);
    while (_window.isOpen())
    {
        //We gaan ALTIJD de events verwerken
        processEvents();

        bool repaint=false;
        timeSinceLastUpdate+=clock.restart();
        while (timeSinceLastUpdate>TimePerFrame)
        {
            timeSinceLastUpdate-=TimePerFrame;
            repaint=true;
            //update 30 keer per seconde

            update(TimePerFrame);
        }

        //We renderen eigenlijk maar 30 keer per seconde
        if (repaint)
            render();
    }
}
void Game::processEvents()
{
    sf::Event event;
    while(_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            //_gameStatus=StatusExit;
            _window.close();
        }

        switch (_GameState)
        {
            case GameState::Menu:
                gameMenu->handleEvent(event);
            default:
                break;
        }
    }
}

void Game::update(sf::Time deltaTime)
{
    switch (_GameState)
    {
        case GameState::Menu:
            gameMenu->updateTime(deltaTime);
        default:
            break;
    }
}

void Game::render()
{
    _window.clear();

    switch (_GameState)
    {
    case GameState::Menu:
        gameMenu->draw();
        break;
    case GameState::Credits:
    case GameState::GameOver:
        break;
    case GameState::Playing:
        break;
    default:
        _window.clear();

    }

    _window.display();
}
