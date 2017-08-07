#include "ManageSound.h"



ManageSound::ManageSound()
{
}


ManageSound::~ManageSound()
{
}


// Return the list of sounds
std::list<std::shared_ptr<sf::Sound>> ManageSound::getSounds() const
{
	return m_sounds;
}


// Return the stack of musics
std::stack<std::shared_ptr<sf::Music>> ManageSound::getMusics() const
{
	return m_musics;
}


// Return the list of sounds with modifying possibilities
std::list<std::shared_ptr<sf::Sound>>& ManageSound::getRealSounds()
{
	return m_sounds;
}


// Return the stack of musics with modifying possibilities
std::stack<std::shared_ptr<sf::Music>>& ManageSound::getRealMusics()
{
	return m_musics;
}