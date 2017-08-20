#include "RegularEntityDeath.hpp"
#include "Sounded.hpp"
#include "Clock.hpp"
#include "Animated.hpp"
#include "Killable.hpp"
#include "MovingAnimator.hpp"
#include "AxisMoving.hpp"
#include "Collider.hpp"
#include "GameCache.hpp"
#include "core.hpp"
#include <exception>

using lif::RegularEntityDeath;

RegularEntityDeath::RegularEntityDeath(lif::Entity& owner, sf::Time deathTime)
	: lif::Component(owner)
	, deathTime(deathTime)
{
	_declComponent<RegularEntityDeath>();
}

lif::Entity* RegularEntityDeath::init() {
	killable = owner.get<lif::Killable>();
	if (killable == nullptr)
		throw std::invalid_argument("RegularEntityDeath's owner has no Killable component!");
	return this;
}

void RegularEntityDeath::kill() {
	// Stop all moving components
	auto moving = owner.getAllRecursive<lif::AxisMoving>();
	for (auto mv : moving) {
		origAutoRealign[mv] = mv->isAutoRealignEnabled();
		mv->setAutoRealign(false);
		mv->stop();
	}

	// Disable all MovingAnimators
	auto movingAnimators = owner.getAllRecursive<lif::MovingAnimator>();
	for (auto movingAnimator : movingAnimators)
		movingAnimator->setActive(false);

	// Switch to death animation
	auto animated = owner.getAllRecursive<lif::Animated>();
	for (auto anim : animated) {
		if (!anim->hasAnimation("death")) continue;
		auto& animSprite = anim->getSprite();
		anim->setAnimation("death");
		animSprite.play();
	}

	auto colliders = owner.getAllRecursive<lif::Collider>();
	for (auto cld : colliders)
		cld->reset();

	// Play death sound
	auto sounded = owner.get<lif::Sounded>();
	if (sounded != nullptr)
		lif::cache.playSound(sounded->getSoundFile("death"));
}

void RegularEntityDeath::resurrect() {
	// Reset all moving components
	auto moving = owner.getAllRecursive<lif::AxisMoving>();
	for (auto mv : moving) {
		auto ar = origAutoRealign.find(mv);
		if (ar != origAutoRealign.end())
			mv->setAutoRealign(ar->second);
	}

	// Reset all MovingAnimators
	auto movingAnimators = owner.getAllRecursive<lif::MovingAnimator>();
	for (auto movingAnimator : movingAnimators)
		movingAnimator->setActive(true);

	// Switch to idle animation
	auto animated = owner.getAllRecursive<lif::Animated>();
	for (auto anim : animated) {
		if (!anim->hasAnimation("death")) continue;
		auto& animSprite = anim->getSprite();
		anim->setAnimation("idle");
		animSprite.play();
	}
}

bool RegularEntityDeath::isKillInProgress() const {
	return killable->timeSinceDeath() < deathTime;
}
