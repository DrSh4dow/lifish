#include "Killable.hpp"

using lif::Killable;

Killable::Killable(lif::Entity& owner)
	: lif::Component(owner)
{
	_declComponent<Killable>();
}

Killable::Killable(lif::Entity& owner, OnKillCallback callback)
	: Killable(owner)
{
	_declComponent<Killable>();
	onKill = callback;
}

Killable::Killable(lif::Entity& owner, OnKillCallback callback, CheckKillCallback checkKill)
	: Killable(owner, callback)
{
	_declComponent<Killable>();
	checkKillProgress = checkKill;
}

void Killable::kill() {
	if (!killed) {
		killed = true;
		deathClock.restart();
		if (onKill)
			onKill();
	}
}

bool Killable::isKillInProgress() const {
	if (!killed) return false;
	return checkKillProgress ? checkKillProgress() : false;
}
