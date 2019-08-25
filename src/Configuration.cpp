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
}



void Configuration::releaseTextures()
{
    resEntities.release("Entities");
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


