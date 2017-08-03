#include "BattleEventManager.h"



BattleEventManager::BattleEventManager() : m_eventInConstruction(nullptr, nullptr)
{
}


BattleEventManager::~BattleEventManager()
{
}


// Return the list of battle events
std::list<std::shared_ptr<B_Event>> BattleEventManager::getBattleEvents() const
{
	return m_battleEvents;
}


// Return the list of battle events with modifying possibilities
std::list<std::shared_ptr<B_Event>>& BattleEventManager::getRealBattleEvents()
{
	return m_battleEvents;
}


// Add an event to the top of the list
int BattleEventManager::addToFront(std::shared_ptr<B_Event> b)
{
	m_battleEvents.push_front(b);

	return 0;
}


// Create an event
int BattleEventManager::createEvent(std::shared_ptr<Fighter> source, std::shared_ptr<B_Event> type)
{
	if (m_eventInConstruction.first != nullptr)
		return -1;

	m_eventInConstruction = std::make_pair(source, type);



	return 0;
}


// Execute the header event
int BattleEventManager::execute(Battle *b)
{
	// [TODO]
	auto it = m_battleEvents.front();
	m_battleEvents.pop_front();

	return 0;
}