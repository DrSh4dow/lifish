#pragma once

#include <array>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics.hpp>
#include "EntityGroup.hpp"
#include "CollisionDetector.hpp"
#include "LevelRenderer.hpp"
#include "LevelTime.hpp"
#include "game_values.hpp"

namespace Game {

class LevelLoader;
class SaveManager;
class Player;
class Bomb;
class Level;

/** The class that manages all the level's entities and events.
 *  In particular, its update() method updates all entities,
 *  the collisions and the game logic.
 */
class LevelManager final : public sf::Drawable, public Game::WithOrigin, private sf::NonCopyable {

	friend class Game::LevelLoader;
	friend class Game::LevelRenderer;
	friend class Game::SaveManager;

	/** The currently managed level */
	Game::Level *level = nullptr;
	Game::LevelRenderer renderer;
	Game::LevelTime levelTime;
	/** Whether hurry up has already been triggered or not */
	bool hurryUp = false;
	/** Whether EXTRA game was already triggered or not */
	bool extraGameTriggered = false;
	/** Whether we're currently in EXTRA game or not */
	bool extraGame = false;
	bool paused = false;

	Game::EntityGroup entities;
	Game::CollisionDetector cd;

	/** "Owned" pointers to players. 
	 *  The players' lifecycle is the following:
	 *  1) players are created via `createNewPlayers` and added to `entities`;
	 *  2) updates to players are managed by the EntityGroup;
	 *  3) on level change, the EntityGroup releases the ownership of all players to us (the LevelManager);
	 *  4) the new level is loaded: `entities` gets cleared;
	 *  5) the players are handed back to the EntityGroup.
	 */
	std::array<std::shared_ptr<Game::Player>, Game::MAX_PLAYERS> players;

	/** Unowned references to bombs currently deployed by players */
	Matrix<std::weak_ptr<Game::Bomb>, Game::MAX_PLAYERS, Game::Conf::Player::MAX_MAX_BOMBS> bombs;


	void _spawnBomb(Game::Bomb *b);
	void _checkResurrect();
	void _checkSpecialConditions();
	void _triggerHurryUp();
	void _triggerExtraGame();
	void _endExtraGame();
	bool _shouldTriggerExtraGame() const;

public:
	explicit LevelManager();

	/** Generates n players and returns them. If n > MAX_PLAYERS, only generate MAX_PLAYERS players. */
	auto createNewPlayers(unsigned short n = Game::MAX_PLAYERS) -> std::vector<Game::Player*>;

	bool isPlayer(const Game::Entity& e) const;
	/** Returns the id-th player (id starting from 1) */
	const Game::Player* getPlayer(unsigned short id) const;

	const Game::EntityGroup& getEntities() const { return entities; }
	Game::EntityGroup& getEntities() { return entities; }

	const Game::Level* getLevel() const { return level; }
	void setLevel(Game::Level& level);

	const Game::LevelTime& getLevelTime() const { return levelTime; }

	/** Updates all entities and collisions */
	void update();

	/** Adds the given entity to `entities` */
	void spawn(Game::Entity *e);

	bool isBombAt(const sf::Vector2i& tile) const;
	/** Returns the number of bombs currently deployed by id-th player */
	unsigned short bombsDeployedBy(unsigned short id) const;

	/** Pauses all Clock components of all entities */
	void pause();
	/** Resumes all Clock components of all entities */
	void resume();
	bool isPaused() const { return paused; }

	/** Clears `entities` and resets internal variables */
	void reset();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setOrigin(const sf::Vector2f& o) override;

};

}
