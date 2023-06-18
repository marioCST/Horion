#include "Jesus.h"

Jesus::Jesus() : IModule(0, Category::MOVEMENT, "Walk over water, like Jesus.") {
}

Jesus::~Jesus() {
}

const char* Jesus::getModuleName() {
	return "Jesus";
}

void Jesus::onTick(GameMode* gm) {
	if (gm->player->isSneaking()) return;

	if (gm->player->isInWater()) {
		Vec3 vel = gm->player->getMovementProxy()->getVelocity();
		vel.y = 0.06f;
		gm->player->getMovementProxy()->setVelocity(vel);
		gm->player->getMovementProxy()->setOnGround(true);
		wasInWater = true;
	} else if (gm->player->isInWater()) {
		Vec3 vel = gm->player->getMovementProxy()->getVelocity();
		vel.y = 0.1f;
		gm->player->getMovementProxy()->setVelocity(vel);
		gm->player->getMovementProxy()->setOnGround(true);
		wasInWater = true;
	} else {
		if (wasInWater) {
			wasInWater = false;
			Vec3 vel = gm->player->getMovementProxy()->getVelocity();
			vel.x *= 1.2f;
			vel.x *= 1.2f;
			gm->player->getMovementProxy()->setVelocity(vel);
		}
	}
}
