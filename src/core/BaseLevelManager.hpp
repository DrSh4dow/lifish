#pragma once

#include <SFML/System/NonCopyable.hpp>
#include "EntityGroup.hpp"
#include "SHCollisionDetector.hpp"
#ifdef MULTITHREADED
#	include <mutex>
#endif
#ifndef RELEASE
#	include "Stats.hpp"
#endif

namespace lif {

class BaseLevelManager : private sf::NonCopyable {
public:
	using GameLogicFunc = std::function<void(
			lif::Entity*,              // for-each loop variable
			lif::BaseLevelManager&,    // LevelManager calling this function
			std::vector<lif::Entity*>& // vector of entities to be spawned after calling game logic
			)>;

protected:
	lif::EntityGroup entities;
	lif::SHCollisionDetector cd;

	std::vector<GameLogicFunc> logicFunctions;

	bool paused = false;

#ifdef MULTITHREADED
	mutable std::mutex lvMutex;
#endif
	inline void _mtxLock() const {
#ifdef MULTITHREADED
		lvMutex.lock();
#endif
	}

	inline void _mtxUnlock() const {
#ifdef MULTITHREADED
		lvMutex.unlock();
#endif
	}

#ifndef RELEASE
	lif::debug::Stats dbgStats;
#endif
	
	virtual void _spawn(lif::Entity *e);
public:
	explicit BaseLevelManager(const sf::FloatRect& lvbounds);

	const lif::EntityGroup& getEntities() const { return entities; }
	lif::EntityGroup& getEntities() { return entities; }
	const lif::CollisionDetector& getCollisionDetector() const { return cd; }

	/** Pauses all Clock components of all entities */
	virtual void pause();
	/** Resumes all Clock components of all entities */
	virtual void resume();
	bool isPaused() const { return paused; }

	/** Updates all entities and collisions */
	virtual void update();

	/** Clears `entities` and resets internal variables */
	virtual void reset();

#ifndef RELEASE
	const lif::debug::Stats& getStats() const { return dbgStats; }
#endif
};

}