#include "Phase.h"

Phase::Phase() : IModule('I', Category::MOVEMENT, "Walk straight through walls.") {
}

Phase::~Phase() {
}

const char* Phase::getModuleName() {
	return ("Phase");
}

void Phase::onTick(GameMode* gm) {
	gm->player->aabb.upper.setY(gm->player->aabb.lower.y);
}

void Phase::onDisable() {
	if (Game.getLocalPlayer() != nullptr) {
		AABB aabb = Game.getLocalPlayer()->aabb;
		aabb.upper.y += 1.8f;
		Game.getLocalPlayer()->setaabb(aabb);
	}
}
