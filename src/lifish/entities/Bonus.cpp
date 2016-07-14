#include "Bonus.hpp"
#include "Drawable.hpp"
#include "Sounded.hpp"
#include "Scored.hpp"
#include "Game.hpp"

using Game::Bonus;
using Game::TILE_SIZE;

const sf::Time Bonus::EXPIRE_TIME = sf::seconds(10);
const sf::Time Bonus::SHIELD_DURATION = sf::milliseconds(20000);
const sf::Time Bonus::SPEEDY_DURATION = sf::milliseconds(20000);

/** Relative bonus probabilities */
std::discrete_distribution<unsigned short> Game::bonusTypeDistribution
{ 
	5, // max bombs
	4, // quick fuse
	5, // max range
	5, // shield
	5, // speedy
	1, // zapper
	1, // sudden death
	5, // health small
	4, // health full
	490 // no bonus
};
	
Bonus::Bonus(const sf::Vector2f& pos, const Type type)
	: Game::Entity(pos)
	, type(type)
{
	sprite = addComponent(new Game::Sprite(this,
				Game::getAsset("graphics", "bonuses.png"), 
				sf::IntRect(
					static_cast<unsigned short>(type) * TILE_SIZE, 
					static_cast<unsigned short>(type) / 10 * TILE_SIZE, 
					TILE_SIZE,
					TILE_SIZE)));
	addComponent(new Game::Drawable(this, sprite));
	addComponent(new Game::Scored(this, VALUE));
	expireClock = addComponent(new Game::Clock(this));
	addComponent(new Game::Sounded(this, { Game::getAsset("test", "bonus_grab.ogg") }));
}

void Bonus::update() {
	Game::Entity::update();
	const float s = expireClock->getElapsedTime().asSeconds();
	if (EXPIRE_TIME.asSeconds() - s <= 3.) {
		const float diff = s - std::floor(s);
		if (5*diff - std::floor(5*diff) < 0.5)
			sprite->getSprite().setColor(sf::Color(0, 0, 0, 0));
		else
			sprite->getSprite().setColor(sf::Color(0, 0, 0, 255));
	}
}