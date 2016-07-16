#include "Screen.hpp"
#include "ScreenBuilder.hpp"
#include <iostream>
#include "utils.hpp"

using Game::UI::Screen;

Screen::Screen(const std::string& layoutFileName, const sf::RenderWindow& window)
	: window(window)
	, size(window.getSize())
{
	Game::UI::ScreenBuilder::build(*this, layoutFileName);	
}

void Screen::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(bgSprite, states);
	for (const auto& elem : nonInteractables)
		target.draw(*elem, states);
	for (const auto& inter : interactables)
		target.draw(*inter.second, states);
}

void Screen::update() {
	// TODO support joystick selection
	const auto mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	if (selected.second != nullptr) {
		if (selected.second->getGlobalBounds().contains(mousePos.x, mousePos.y))
			return;
		else {
			selected.second->setColor(sf::Color::White);
			selected.second = nullptr;
		}
	}
	for (auto& pair : interactables) {
		auto& inter = pair.second;
		if (inter->getGlobalBounds().contains(mousePos.x, mousePos.y)) {
			selected = std::make_pair(pair.first, inter.get());
			break;
		}
	}
	if (selected.second != nullptr) {
		selected.second->setColor(sf::Color::Red);
	}
}