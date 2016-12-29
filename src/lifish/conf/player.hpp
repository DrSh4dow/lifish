#pragma once

#include <SFML/System/Time.hpp>

namespace Game {

namespace Conf {
	namespace Player {
		constexpr unsigned short INITIAL_LIVES = 4;
		constexpr unsigned short MAX_LIFE = 16;
		constexpr unsigned short DEFAULT_MAX_BOMBS = 5;
		constexpr unsigned short MAX_MAX_BOMBS = 8;
		constexpr unsigned short INITIAL_CONTINUES = 3;
		constexpr float DEFAULT_SPEED = 150.f;
		/** The number of letters needed to get an extra life */
		constexpr unsigned short N_EXTRA_LETTERS = 5;
		const sf::Time DEATH_TIME = sf::seconds(5);
		const sf::Time DEATH_STOP_ANIM_TIME = sf::seconds(2);
		const sf::Time HURT_ANIM_DURATION = sf::milliseconds(120);
		const sf::Time RESURRECT_SHIELD_TIME = sf::seconds(5);
		const sf::Time DAMAGE_SHIELD_TIME = sf::seconds(1);
	}
}

}