#pragma once

#include <array>
#include <ostream>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>
#include "Game.hpp"
#include "Direction.hpp"
#include "Animated.hpp"

#if defined(SFML_SYSTEM_WINDOWS) || defined(__MINGW32__)
#	include <windows.h>
#	define SLEEP_MS(ms) \
		Sleep(ms)
#else
#	include <thread>
#	include <chrono>
#	define SLEEP_MS(ms) \
		std::this_thread::sleep_for(std::chrono::milliseconds(ms))
#endif

namespace Game {

class LevelManager;

template<class T, size_t ROWS, size_t COLS>
using Matrix = std::array<std::array<T, COLS>, ROWS>;

/// Polyfill ///
template<typename T> 
constexpr T abs(T num) {
#ifdef IS_APPLE
	// Apple Clang is a "good" compiler...
	if (num < 0) return -num;
	return num;
#else
	return std::abs(num);
#endif
}

template<typename T>
inline std::string to_string(T val) {
#ifdef __MINGW32__
	std::stringstream ss;
	ss << val;
	return ss.str();
#else
	return std::to_string(val);
#endif
}
//////////////

/** Returns the aligned tile corresponding to position `pos` by
 *  flooring its components and dividing by TILE_SIZE.
 */
inline sf::Vector2i tile(const sf::Vector2f& pos) {
	return sf::Vector2i(
			short(pos.x) / Game::TILE_SIZE, 
			short(pos.y) / Game::TILE_SIZE);
}

inline sf::Vector2f aligned(const sf::Vector2f& pos) {
	return sf::Vector2f(
		float((short(pos.x)/TILE_SIZE) * TILE_SIZE),
		float((short(pos.y)/TILE_SIZE) * TILE_SIZE));
}

template<typename T>
constexpr std::ostream& operator<<(std::ostream& stream, const sf::Vector2<T>& vec) {
	return stream << "(" << vec.x << ", " << vec.y << ")";
}

template<typename T>
constexpr std::ostream& operator<<(std::ostream& stream, const sf::Rect<T>& rect) {
	return stream << "(" << rect.left << ", " << rect.top 
		<< " x " << rect.width << ", " << rect.height << ")";
}

template<typename T, typename R>
constexpr double sqrDistance(const sf::Vector2<T>& a, const sf::Vector2<R>& b) {
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

constexpr float centerX(const sf::FloatRect& bounds, const sf::FloatRect& rect) {
	return rect.left + (rect.width - bounds.width) / 2.;
}

constexpr float centerY(const sf::FloatRect& bounds, const sf::FloatRect& rect) {
	return rect.top + (rect.height - bounds.height) / 2.;
}

/** Given the bounding box of something, returns the coordinates
 *  which center that thing relatively to the given rectangle.
 *  bounds = object to center
 *  rect = container to center the object within (default: main game window)
 */
inline sf::Vector2f center(const sf::FloatRect& bounds, const sf::FloatRect& rect = 
		sf::FloatRect(Game::MAIN_WINDOW_SHIFT, 0, 
			Game::LEVEL_WIDTH * Game::TILE_SIZE,
			Game::LEVEL_HEIGHT * Game::TILE_SIZE))
{
	return sf::Vector2f(centerX(bounds, rect), centerY(bounds, rect));
}

inline bool startsWith(const std::string& haystack, const std::string& needle) {
	return haystack.substr(0, needle.size()) == needle;
}

void playMusic();
void stopMusic();
/** Plays a brief sound */
void testMusic();

/** If Game::options.showFPS == true, then display FPS on screen */
void maybeShowFPS(sf::RenderWindow& window);

namespace KeyUtils {
	short keyToNumber(sf::Keyboard::Key key);

	std::string keyToString(sf::Keyboard::Key key);
}

} // end namespace Game
