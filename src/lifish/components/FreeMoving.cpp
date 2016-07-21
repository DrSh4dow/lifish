#include "FreeMoving.hpp"

using Game::FreeMoving;

FreeMoving::FreeMoving(Game::Entity *const owner, float speed, sf::Vector2f velocity)
	: Game::Moving(owner, speed)
	, velocity(velocity)
{}

void FreeMoving::update() {
	Game::Component::update();
	if (!moving) return;

	sf::Vector2f shift(0.f, 0.f);
	sf::Time frameTime = frameClock->restart();

	if (frameTime > MAX_FRAME_TIME)
		frameTime = MAX_FRAME_TIME;
	
	shift += velocity * speed;

	// TODO
	//if (!colliding) {
		owner->setPosition(owner->getPosition() + shift * frameTime.asSeconds());
		distTravelled += speed * frameTime.asSeconds();
	//}
}

