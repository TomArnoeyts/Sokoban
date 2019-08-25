#include "Configuration.h"
thor::ResourceHolder <sf::Texture, std::string> Configuration::resEntities;

thor::ResourceHolder <sf::SoundBuffer, std::string> Configuration::resSoundEffects;

thor::ResourceHolder <sf::Font, std::string> Configuration::resFonts;

void Configuration::initialize()
{
    initTextures();
    initSounds();
    initFonts();
}
void Configuration::release()
{
    releaseTextures();
    releaseSounds();
    releaseFonts();

}

void Configuration::initTextures()
{
    resEntities.acquire("Entities", thor::Resources::fromFile<sf::Texture>("resources/Spritesheet/Sokoban_Spritesheet.png"));

    resEntities.acquire("PauseBackground", thor::Resources::fromFile<sf::Texture>("resources/Menus/Pause.png"));
}



void Configuration::releaseTextures()
{
    resEntities.release("Entities");

    resEntities.release("PauseBackground");
}


void Configuration::initSounds()
{
}

void Configuration::releaseSounds()
{

}

void Configuration::initFonts()
{

}

void Configuration::releaseFonts()
{

}


