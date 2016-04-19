#include "Level.hpp"
#include "Game.hpp"
#include "GameCache.hpp"
#include "LevelSet.hpp"
#include "utils.hpp"
#include <iostream>
#include <sstream>

using Game::Level;
using Game::pwd;
using Game::DIRSEP;
using Game::LEVEL_WIDTH;
using Game::LEVEL_HEIGHT;
using Game::TILE_SIZE;
using Game::EntityType;

Level::Level(const Game::LevelSet *const _levelSet) 
	: levelSet(_levelSet) 
	, Game::Entity()
{}

bool Level::init() {
	if (initialized) return true;
	
	if (!_setTilemap(levelInfo.tilemap))
		return false;

	music = addComponent(new Game::Music(this, levelInfo.track));
	_loadTextures();

	levelnumtext = addComponent(new Game::LevelNumText(this, levelInfo.levelnum));

	return initialized = true;
}

void Level::_loadTextures() {
	std::stringstream ss;
	ss << "bg" << levelInfo.tileIDs.bg << ".png";
	// Load background texture
	bgTexture = addComponent(new Game::Texture(this, Game::getAsset("graphics", ss.str())));
	// Load borderTexture
	borderTexture = addComponent(new Game::Texture(this, Game::getAsset("graphics", "border.png")));
	_loadTiles();
}

// FIXME: move to texture components?
void Level::_loadTiles() {
	bgTiles[TILE_REGULAR].setTexture(*bgTexture->getTexture());
	bgTexture->getTexture()->setRepeated(true);
	bgTexture->getTexture()->setSmooth(true);
	bgTiles[TILE_REGULAR].setTextureRect(sf::IntRect(0, 0, TILE_SIZE * LEVEL_WIDTH, TILE_SIZE * LEVEL_HEIGHT));

	for (unsigned short i = 1; i < bgTiles.size(); ++i)
		bgTiles[i].setTexture(*borderTexture->getTexture());

	const unsigned short b = (levelInfo.tileIDs.border-1) * TILE_SIZE;

	bgTiles[TILE_UPPER].setTextureRect(sf::IntRect(0, b, TILE_SIZE, TILE_SIZE));
	// TILE_UPPER y-mirrored
	bgTiles[TILE_LOWER].setTextureRect(sf::IntRect(0, b + TILE_SIZE, TILE_SIZE, -TILE_SIZE));
	bgTiles[TILE_LEFT].setTextureRect(sf::IntRect(TILE_SIZE, b, TILE_SIZE, TILE_SIZE));
	// TILE_LEFT x-mirrored
	bgTiles[TILE_RIGHT].setTextureRect(sf::IntRect(TILE_SIZE + TILE_SIZE, b, -TILE_SIZE, TILE_SIZE));
	bgTiles[TILE_UPPER_LEFT].setTextureRect(sf::IntRect(2 * TILE_SIZE, b, TILE_SIZE, TILE_SIZE));
	// TILE_UPPER_LEFT x-mirrored
	bgTiles[TILE_UPPER_RIGHT].setTextureRect(sf::IntRect(2 * TILE_SIZE + TILE_SIZE, b, -TILE_SIZE, TILE_SIZE));
	bgTiles[TILE_LOWER_RIGHT].setTextureRect(sf::IntRect(3 * TILE_SIZE, b, TILE_SIZE, TILE_SIZE));
	// TILE_LOWER_RIGHT x-mirrored
	bgTiles[TILE_LOWER_LEFT].setTextureRect(sf::IntRect(3 * TILE_SIZE + TILE_SIZE, b, -TILE_SIZE, TILE_SIZE));

	// Set angles' position once and for all
	bgTiles[TILE_UPPER_LEFT].setPosition(0, 0);
	bgTiles[TILE_UPPER_RIGHT].setPosition(TILE_SIZE * (LEVEL_WIDTH+1), 0);
	bgTiles[TILE_LOWER_RIGHT].setPosition(TILE_SIZE * (LEVEL_WIDTH+1), TILE_SIZE * (LEVEL_HEIGHT+1));
	bgTiles[TILE_LOWER_LEFT].setPosition(0, TILE_SIZE * (LEVEL_HEIGHT+1));
}

void Level::setOrigin(const sf::Vector2f& offset) {
	for (unsigned short i = 0; i < bgTiles.size(); ++i)
		bgTiles[i].setOrigin(offset);
	levelnumtext->setOrigin(offset);
}

void Level::draw(sf::RenderTarget& window, sf::RenderStates states) const {
	// Border tiles number on x axis
	const unsigned short btn_x = LEVEL_WIDTH + 1;
	// Border tiles number on y axis
	const unsigned short btn_y = LEVEL_HEIGHT + 1;

	// Draw the level background
	window.draw(bgTiles[TILE_REGULAR], states);

	// Draw the borders
	window.draw(bgTiles[TILE_UPPER_LEFT], states);
	window.draw(bgTiles[TILE_UPPER], states);
	window.draw(bgTiles[TILE_LEFT], states);
	window.draw(bgTiles[TILE_UPPER_RIGHT], states);
	window.draw(bgTiles[TILE_RIGHT], states);
	window.draw(bgTiles[TILE_LOWER_RIGHT], states);
	window.draw(bgTiles[TILE_LOWER], states);
	window.draw(bgTiles[TILE_LOWER_LEFT], states);

	// Draw the level number
	if (levelnumtext != nullptr)
		window.draw(*levelnumtext, states);
}

EntityType Level::getTile(unsigned short left, unsigned short top) const {
	if (left >= LEVEL_WIDTH || top >= LEVEL_HEIGHT) 
		return EntityType::UNKNOWN;
	return tiles[top][left];
}

bool Level::_setTilemap(const std::string& tilemap) {
	unsigned short x = 0, y = 0;
	bool player_set[] = { false, false };
	for (unsigned int i = 0; i < tilemap.length(); ++i) {
		EntityType et = Game::entityFromLetter(tilemap[i]);
		if (et == EntityType::UNKNOWN) return false;
		if (et == EntityType::PLAYER1) {
			if (player_set[0]) return false;
			player_set[0] = true;
		}
		if (et == EntityType::PLAYER2) {
			if (player_set[1]) return false;
			player_set[1] = true;
		}
		tiles[y][x] = et;
		if (++x == LEVEL_WIDTH) {
			x = 0;
			if (++y == LEVEL_HEIGHT)
				break;
		}
	}
	return true;
}

std::string Level::getTilemap() const {
	static bool called = false;
	static std::stringstream ss;

	if (called)
		return ss.str();

	for (unsigned short i = 0; i < LEVEL_HEIGHT; ++i) {
		for (unsigned short j = 0; j < LEVEL_WIDTH; ++j) {
			ss << tiles[i][j] << " ";
		}
		ss << std::endl;
	}

	called = true;
	return ss.str();
}

std::string Level::toString() const {
	static bool called = false;
	static std::stringstream ss;

	if (called)
		return ss.str();

	ss << "Level Info:\r\n" 
	   << "-----------\r\n"
	   << "Level " << levelInfo.levelnum << "\r\n"
	   << "Time: " << levelInfo.time << " s\r\n"
	   << "Tiles: {\r\n"
	   << "    bg: " << levelInfo.tileIDs.bg << "\r\n"
	   << "    border: " << levelInfo.tileIDs.border << "\r\n"
	   << "    fixed: " << levelInfo.tileIDs.fixed << "\r\n"
	   << "    breakable: " << levelInfo.tileIDs.breakable << "\r\n}\r\n"
	   << "Music: " << music->getTrack().name << "\r\n";
	if (levelSet != nullptr) {
		ss << "Belongs to: >>>\r\n";
		ss << levelSet->toString();
		ss << "<<<\r\n";
	}
	ss << "Tilemap:\r\n";
	ss << getTilemap();

	called = true;
	return ss.str();
}
