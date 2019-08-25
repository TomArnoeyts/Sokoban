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
            case GameState::Playing:
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code==sf::Keyboard::Up)
                    {
                        if (moveIsAllowed(moveDirection::Up))
                        {
                            if (currentFieldValues[vecPlayerPosition.y][vecPlayerPosition.x-1]==enBlockType::Box)
                                currentFieldValues[vecPlayerPosition.y][vecPlayerPosition.x-2]=enBlockType::Box;
                            currentFieldValues[vecPlayerPosition.y][vecPlayerPosition.x-1]=enBlockType::Player;
                            currentFieldValues[vecPlayerPosition.y][vecPlayerPosition.x]=enBlockType::Empty;
                            vecPlayerPosition.x--;
                        }

                    }
                    if (event.key.code==sf::Keyboard::Down)
                    {
                        if (moveIsAllowed(moveDirection::Down))
                        {
                            if (currentFieldValues[vecPlayerPosition.y][vecPlayerPosition.x+1]==enBlockType::Box)
                                currentFieldValues[vecPlayerPosition.y][vecPlayerPosition.x+2]=enBlockType::Box;
                            currentFieldValues[vecPlayerPosition.y][vecPlayerPosition.x+1]=enBlockType::Player;
                            currentFieldValues[vecPlayerPosition.y][vecPlayerPosition.x]=enBlockType::Empty;
                            vecPlayerPosition.x++;
                        }
                    }
                    if (event.key.code==sf::Keyboard::Left)
                    {
                        if (moveIsAllowed(moveDirection::Left))
                        {
                            if (currentFieldValues[vecPlayerPosition.y-1][vecPlayerPosition.x]==enBlockType::Box)
                                currentFieldValues[vecPlayerPosition.y-2][vecPlayerPosition.x]=enBlockType::Box;
                            currentFieldValues[vecPlayerPosition.y-1][vecPlayerPosition.x]=enBlockType::Player;
                            currentFieldValues[vecPlayerPosition.y][vecPlayerPosition.x]=enBlockType::Empty;
                            vecPlayerPosition.y--;
                        }
                    }
                    if (event.key.code==sf::Keyboard::Right)
                    {
                        if (moveIsAllowed(moveDirection::Right))
                        {
                            if (currentFieldValues[vecPlayerPosition.y+1][vecPlayerPosition.x]==enBlockType::Box)
                                currentFieldValues[vecPlayerPosition.y+2][vecPlayerPosition.x]=enBlockType::Box;
                            currentFieldValues[vecPlayerPosition.y+1][vecPlayerPosition.x]=enBlockType::Player;
                            currentFieldValues[vecPlayerPosition.y][vecPlayerPosition.x]=enBlockType::Empty;
                            vecPlayerPosition.y++;
                        }
                    }
                    if (event.key.code==sf::Keyboard::Escape)
                    {
                        _GameState=GameState::Pause;
                        bigSprite.setTexture(Configuration::resEntities["PauseBackground"]);
                    }
                }
                break;
            case GameState::Pause:
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code==sf::Keyboard::Return)
                    {
                        //Reset Game:
                        unloadLevel();
                        _GameState=GameState::Menu;

                    }
                    if (event.key.code==sf::Keyboard::Escape)
                    {

                        _GameState=GameState::Playing;
                    }
                }
                break;
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
                    (*gridSpritesMoveable[y][x]).setTexture(Configuration::resEntities["Entities"]);
                    (*gridSpritesMoveable[y][x]).setPosition(250+y*64*scale, 50+x*64*scale);
                    (*gridSpritesMoveable[y][x]).setScale(scale, scale);
                    (*gridSpritesMoveable[y][x]).setTextureRect(sf::IntRect(0,0,0,0));
                    if (fieldValues[y][x]==enBlockType::Wall)
                    {
                        (*gridSpritesWalls[y][x]).setTextureRect(sf::IntRect(384,512,64,64));
                    }
                    if (currentFieldValues[y][x]==enBlockType::Box)
                    {
                        (*gridSpritesMoveable[y][x]).setTextureRect(sf::IntRect(0,256,64,64));
                    }
                    if (currentFieldValues[y][x]==enBlockType::Destination)
                    {
                        (*gridSpritesGround[y][x]).setTextureRect(sf::IntRect(0,64,64,64));
                    }
                    if (currentFieldValues[y][x]==enBlockType::Player)
                    {
                        (*gridSpritesMoveable[y][x]).setTextureRect(sf::IntRect(448,240,64,48));
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
    case GameState::Pause:
        _window.draw(bigSprite);
        break;
    case GameState::GameOver:
        break;
    case GameState::Playing:
        _window.draw(bgShape);

        for (int y=0; y<fieldHeight; y++)
        {
            for (int x=0; x<fieldWidth; x++)
            {
                _window.draw(*gridSpritesGround[y][x]);
                _window.draw(*gridSpritesWalls[y][x]);
                _window.draw(*gridSpritesMoveable[y][x]);
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

    scale=(float)500/((float)(64*fieldWidth));
    if (fieldWidth<fieldHeight)
        scale=(float)500/((float)(64*fieldHeight));

    fieldValues=new enBlockType*[fieldHeight];
    currentFieldValues=new enBlockType*[fieldHeight];
    gridSpritesGround=new sf::Sprite**[fieldHeight];
    gridSpritesWalls=new sf::Sprite**[fieldHeight];
    gridSpritesMoveable=new sf::Sprite**[fieldHeight];


    for(int i = 0; i < fieldHeight; i++)
    {
        fieldValues[i] = new enBlockType[fieldWidth];
        currentFieldValues[i] = new enBlockType[fieldWidth];
        gridSpritesGround[i] = new sf::Sprite*[fieldWidth];
        gridSpritesWalls[i] = new sf::Sprite*[fieldWidth];
        gridSpritesMoveable[i] = new sf::Sprite*[fieldWidth];

        for (int j=0; j<fieldWidth; j++)
        {
            gridSpritesGround[i][j]=new sf::Sprite();
            (*gridSpritesGround[i][j]).setTexture(Configuration::resEntities["Entities"]);
            (*gridSpritesGround[i][j]).setPosition(250+i*64*scale, 50+j*64*scale);
            (*gridSpritesGround[i][j]).setScale(scale, scale);
            (*gridSpritesGround[i][j]).setTextureRect(sf::IntRect(64,128,64,64));
            gridSpritesWalls[i][j]=new sf::Sprite();
            (*gridSpritesWalls[i][j]).setTexture(Configuration::resEntities["Entities"]);
            (*gridSpritesWalls[i][j]).setPosition(250+i*64*scale, 50+j*64*scale);
            (*gridSpritesWalls[i][j]).setTextureRect(sf::IntRect(0,0,0,0));
            (*gridSpritesWalls[i][j]).setScale(scale, scale);
            gridSpritesMoveable[i][j]=new sf::Sprite();
        }
    }


    //scale=1;
    for (int j=0; j<fieldHeight; j++)
        for (int i=0; i<fieldWidth; i++)
        {
            fieldValues[j][i]=enBlockType::Empty;
            currentFieldValues[j][i]=enBlockType::Empty;
        }


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
                {
                    fieldValues[rowIndex][colIndex]=enBlockType::Player;
                    vecPlayerPosition.x=colIndex;
                    vecPlayerPosition.y=rowIndex;
                }

                currentFieldValues[rowIndex][colIndex]=fieldValues[rowIndex][colIndex];

                colIndex++;
            }
            rowIndex++;
        }
    }

    inFile.close();

}

bool Game::moveIsAllowed(moveDirection md)
{
    if (md==moveDirection::Up)
    {
        if (vecPlayerPosition.x==0)
            return false;
        if (fieldValues[vecPlayerPosition.y][vecPlayerPosition.x-1]==enBlockType::Wall)
            return false;
        if (currentFieldValues[vecPlayerPosition.y][vecPlayerPosition.x-1]==enBlockType::Box)
            if (currentFieldValues[vecPlayerPosition.y][vecPlayerPosition.x-2]==enBlockType::Box || fieldValues[vecPlayerPosition.y][vecPlayerPosition.x-2]==enBlockType::Wall)
                return false;
    }

    if (md==moveDirection::Down)
    {
        if (vecPlayerPosition.x==fieldWidth-1)
            return false;
        if (fieldValues[vecPlayerPosition.y][vecPlayerPosition.x+1]==enBlockType::Wall)
            return false;
        if (currentFieldValues[vecPlayerPosition.y][vecPlayerPosition.x+1]==enBlockType::Box)
            if (currentFieldValues[vecPlayerPosition.y][vecPlayerPosition.x+2]==enBlockType::Box || fieldValues[vecPlayerPosition.y][vecPlayerPosition.x+2]==enBlockType::Wall)
                return false;
    }

    if (md==moveDirection::Left)
    {
        if (vecPlayerPosition.y==0)
            return false;
        if (fieldValues[vecPlayerPosition.y-1][vecPlayerPosition.x]==enBlockType::Wall)
            return false;
        if (currentFieldValues[vecPlayerPosition.y-1][vecPlayerPosition.x]==enBlockType::Box)
            if (currentFieldValues[vecPlayerPosition.y-2][vecPlayerPosition.x]==enBlockType::Box || fieldValues[vecPlayerPosition.y-2][vecPlayerPosition.x]==enBlockType::Wall)
                return false;
    }

    if (md==moveDirection::Right)
    {
        if (vecPlayerPosition.y==fieldHeight-1)
            return false;
        if (fieldValues[vecPlayerPosition.y+1][vecPlayerPosition.x]==enBlockType::Wall)
            return false;
        if (currentFieldValues[vecPlayerPosition.y+1][vecPlayerPosition.x]==enBlockType::Box)
            if (currentFieldValues[vecPlayerPosition.y+2][vecPlayerPosition.x]==enBlockType::Box || fieldValues[vecPlayerPosition.y+2][vecPlayerPosition.x]==enBlockType::Wall)
                return false;
    }

    return true;
}

void Game::unloadLevel()
{
    for(int i = 0; i < fieldHeight; i++)
    {
        for (int j=0; j<fieldWidth; j++)
        {
            delete gridSpritesGround[i][j];
            delete gridSpritesWalls[i][j];
            delete gridSpritesMoveable[i][j];
        }
    }

    for(int i = 0; i < fieldHeight; i++)
    {
        delete gridSpritesGround[i];
        delete gridSpritesWalls[i];
        delete gridSpritesMoveable[i];
        delete fieldValues[i];
        delete currentFieldValues[i];
    }
}
