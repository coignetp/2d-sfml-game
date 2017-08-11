#include "Item.h"
#include "../Error/ValueException.h"



Item::Item()
{
}


Item::~Item()
{
}


// Return the id
int Item::getId() const
{
	return m_id;
}


// Return the name
std::string Item::getName() const
{
	return m_name;
}


// Return the description
std::string Item::getDescription() const
{
	return m_description;
}


// Return the price when bought
int Item::getPriceBuy() const
{
	return m_priceBuy;
}


// Return the price when sold
int Item::getPriceSell() const
{
	return m_priceSell;
}


// Return the effects
std::deque<BattleEffects> Item::getEffects() const
{
	return m_effects;
}


// Return the effects with modifying possibilities
std::deque<BattleEffects>& Item::getRealEffects()
{
	return m_effects;
}


// Modify the id
int Item::setId(const int &id)
{
	if (id < 0)
		THROW_VALUE("Impossible id : " + std::to_string(id));

	m_id = id;

	return 0;
}


// Modify the name
int Item::setName(const std::string &name)
{
	m_name = name;

	return 0;
}


// Modify the description
int Item::setDescription(const std::string &description)
{
	m_description = description;

	return 0;
}


// Modify the price when bought
int Item::setPriceBuy(const int &p)
{
	if (p < 0)
		THROW_VALUE("Impossible price : " + std::to_string(p));

	m_priceBuy = p;

	return 0;
}


// Modify the price when sold
int Item::setPriceSell(const int &p)
{
	if (p < 0)
		THROW_VALUE("Impossible price : " + std::to_string(p));

	m_priceSell = p;

	return 0;
}