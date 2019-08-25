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

    struct dirent *entry = nullptr;
    DIR *dp = nullptr;

    dp = opendir("resources\\Levels\\E_Sokoban\\");
    if (dp != nullptr) {
        while ((entry = readdir(dp)))
        {
            std::string filename(entry->d_name);
            std::string needle("ErimS");
            std::size_t found = filename.find(needle);
            if (found!=std::string::npos)
                gameMenu->get<tgui::ListBox>("lstLevel")->addItem(entry->d_name);
        }

    }

    closedir(dp);
    gameMenu->get<tgui::ListBox>("lstLevel")->setSelectedItemByIndex(0);

    gameMenu->get("btnStartGame")->connect("pressed", [&]()
                                           {
                                              initGame("resources\\Levels\\E_Sokoban\\" + gameMenu->get<tgui::ListBox>("lstLevel")->getSelectedItem().toAnsiString());
                                           });

    gameMenu->get<tgui::Button>("btnExitGame")->connect("pressed", [&]()
                                                         {
                                                            _window.close();
                                                         }
                                                         );
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
            break;
        case GameState::Playing:
            for (int y=0; y<fieldHeight; y++)
            {
                for (int x=0; x<fieldWidth; x++)
                {
                    (*gridSprites[y][x]).setTexture(Configuration::resEntities["Entities"]);
                    (*gridSprites[y][x]).setPosition(250+x*64*scale, 50+y*64*scale);
                    (*gridSprites[y][x]).setScale(scale, scale);
                    if (fieldValues[y][x]==enBlockType::Wall)
                    {
                        (*gridSprites[y][x]).setTextureRect(sf::IntRect(384,512,64,64));
                    }
                    if (fieldValues[y][x]==enBlockType::Empty)
                    {
                        (*gridSprites[y][x]).setTextureRect(sf::IntRect(64,128,64,64));
                    }
                    if (fieldValues[y][x]==enBlockType::Box)
                    {
                        (*gridSprites[y][x]).setTextureRect(sf::IntRect(0,256,64,64));
                    }
                    if (fieldValues[y][x]==enBlockType::Destination)
                    {
                        (*gridSprites[y][x]).setTextureRect(sf::IntRect(0,64,64,64));
                    }
                    if (fieldValues[y][x]==enBlockType::Player)
                    {
                        (*gridSprites[y][x]).setTextureRect(sf::IntRect(448,240,64,48));
                    }

                }
            }
            break;
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
        _window.draw(bgShape);

        for (int y=0; y<fieldHeight; y++)
        {
            for (int x=0; x<fieldWidth; x++)
            {
                _window.draw(*gridSprites[y][x]);
            }
        }
        break;
    default:
        _window.clear();

    }

    _window.display();
}

void Game::initGame(std::string strLevelName)
{
    _GameState=GameState::Playing;

    parseLevel(strLevelName);

    bgShape.setSize(sf::Vector2f(500,500));
    bgShape.setFillColor(sf::Color::Black);
    bgShape.setPosition(250,50);
}

void Game::parseLevel(std::string strLevelName)
{
    fieldHeight=0;

    std::string lengthMeter;

    unsigned int maxLength=0;

    std::ifstream inFile;

    inFile.open(strLevelName);

    while (!inFile.eof())
    {
        std::getline(inFile, lengthMeter);
        if (lengthMeter.find(";")==std::string::npos && lengthMeter.find("Title")==std::string::npos && lengthMeter.find("Author")==std::string::npos)
        {

            if (maxLength<lengthMeter.length())
            {
                //We houden rekening met deze lijn:
                maxLength=lengthMeter.length();

            }
            fieldHeight++;
        }
    }

    inFile.close();

    fieldWidth = maxLength;

    fieldValues=new enBlockType*[fieldHeight];
    gridSprites=new sf::Sprite**[fieldHeight];


    for(int i = 0; i < fieldHeight; i++)
    {
        fieldValues[i] = new enBlockType[fieldWidth];
        gridSprites[i] = new sf::Sprite*[fieldWidth];

        for (int j=0; j<fieldWidth; j++)
        {
            gridSprites[i][j]=new sf::Sprite();


        }
    }
    scale=(float)500/((float)(64*fieldWidth));
    if (fieldWidth<fieldHeight)
        scale=(float)500/((float)(64*fieldHeight));

    //scale=1;
    for (int j=0; j<fieldHeight; j++)
        for (int i=0; i<fieldWidth; i++)
            fieldValues[j][i]=enBlockType::Empty;

    inFile.open(strLevelName);
    int rowIndex=0;
    while (!inFile.eof())
    {
        std::getline(inFile, lengthMeter);

        if (lengthMeter.find(";")==std::string::npos && lengthMeter.find("Title")==std::string::npos && lengthMeter.find("Author")==std::string::npos)
        {
            int colIndex=0;
            for (char& c : lengthMeter)
            {
                fieldValues[rowIndex][colIndex]=enBlockType::Empty;
                if (c=='#')
                    fieldValues[rowIndex][colIndex]=enBlockType::Wall;
                if (c=='$')
                    fieldValues[rowIndex][colIndex]=enBlockType::Box;
                if (c=='.')
                    fieldValues[rowIndex][colIndex]=enBlockType::Destination;
                if (c=='@')
                    fieldValues[rowIndex][colIndex]=enBlockType::Player;
                colIndex++;
            }
            rowIndex++;
        }
    }

    inFile.close();

}
