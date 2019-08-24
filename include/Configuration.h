#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <Thor/Resources.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
class Configuration
{
    public:
        Configuration() = delete;
        Configuration(const Configuration& other) = delete;
        Configuration& operator=(const Configuration& other) = delete;

        static thor::ResourceHolder <sf::Texture, std::string> resEntities;

        static thor::ResourceHolder <sf::SoundBuffer, std::string> resSoundEffects;

        static thor::ResourceHolder <sf::Font, std::string> resFonts;

        static void initialize();
        static void release();
    protected:

    private:
        static void initTextures();
        static void initSounds();
        static void initFonts();
        static void releaseTextures();
        static void releaseSounds();
        static void releaseFonts();
};

#endif // CONFIGURATION_H
