#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class FastStop : public IModule {
public:

	FastStop() : IModule(0x0, Category::MOVEMENT, "Stop. but fast!") {};
	~FastStop(){};

	virtual const char* getModuleName() override { return "FastStop"; }

	void onMove(MoveInputHandler* input) {
		if (Game.getLocalPlayer() != nullptr && Game.getLocalPlayer()->isAlive()) {
			LocalPlayer* player = Game.getLocalPlayer();

			if (!input->isPressed()) {
				player->velocity.setX(0);
				player->velocity.setZ(0);
			}
		}
	}
};