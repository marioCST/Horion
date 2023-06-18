#include "Fly.h"

Fly::Fly() : IModule('F', Category::MOVEMENT, "Fly to the sky") {
	mode.addEntry(EnumEntry("Creative", 0))
	.addEntry(EnumEntry("CubeGlide", 1))
	.addEntry(EnumEntry("AirStuck", 2))
	.addEntry(EnumEntry("Jetpack", 3))
	.addEntry(EnumEntry("Jetpack2", 4))
	.addEntry(EnumEntry("Motion", 5))
	.addEntry(EnumEntry("Moonlight", 6))
	.addEntry(EnumEntry("Geyser", 7));
	registerEnumSetting("Mode", &mode, 0);
	registerFloatSetting("Horizontal Speed", &this->horizontalSpeed, this->horizontalSpeed, 0.1f, 10.f);
	registerFloatSetting("Vertical Speed", &this->verticalSpeed, this->verticalSpeed, 0.1f, 10.f);
}

Fly::~Fly() {
}

const char *Fly::getModuleName() {
	return ("Fly");
}

void Fly::onEnable() {
	switch (mode.selected) {
	case 5:
		if (Game.getLocalPlayer() != nullptr)
			Game.getLocalPlayer()->getMovementProxy()->setPos((*Game.getLocalPlayer()->getPos()).add(Vec3(0, 1, 0)));
		break;
	}
}

void Fly::onTick(GameMode *gm) {
	++gameTick;

	switch (mode.selected) {
	case 0:
		gm->player->canFly = true;
		break;
	case 1: {
		float calcYaw = (gm->player->getMovementProxy()->getRot().x + 90) * (PI / 180);

		gameTick++;

		Vec3 pos = *Game.getLocalPlayer()->getPos();
		pos.y += 1.3f;
		C_MovePlayerPacket a(Game.getLocalPlayer(), pos);
		Game.getClientInstance()->loopbackPacketSender->sendToServer(&a);
		pos.y -= 1.3f;
		C_MovePlayerPacket a2(Game.getLocalPlayer(), pos);
		Game.getClientInstance()->loopbackPacketSender->sendToServer(&a2);

		Vec3 moveVec;
		moveVec.x = cos(calcYaw) * horizontalSpeed;
		moveVec.z = sin(calcYaw) * horizontalSpeed;

		gm->player->lerpMotion(moveVec);

		if (gameTick >= 5) {
			gameTick = 0;
			float yaw = gm->player->getMovementProxy()->getRot().x * (PI / 180);
			float length = 4.f;

			float x = -sin(yaw) * length;
			float z = cos(yaw) * length;

			gm->player->getMovementProxy()->setPos(pos.add(Vec3(x, 0.5f, z)));
		}

		break;
	}
	case 2:
		gm->player->getMovementProxy()->setVelocity(Vec3(0, 0, 0));
		break;

	case 3: {
		float calcYaw = (gm->player->getMovementProxy()->getRot().x + 90) * (PI / 180);
		float calcPitch = (gm->player->getMovementProxy()->getRot().y) * -(PI / 180);

		Vec3 moveVec;
		moveVec.x = cos(calcYaw) * cos(calcPitch) * horizontalSpeed;
		moveVec.y = sin(calcPitch) * horizontalSpeed;
		moveVec.z = sin(calcYaw) * cos(calcPitch) * horizontalSpeed;

		gm->player->lerpMotion(moveVec);

		break;
	}
	case 4: {
		if (gameTick >= 5) {
			float calcYaw = (gm->player->getMovementProxy()->getRot().x + 90) * (PI / 180);
			float calcPitch = (gm->player->getMovementProxy()->getRot().y) * -(PI / 180);

			Vec3 pos = *Game.getLocalPlayer()->getPos();
			C_MovePlayerPacket a(Game.getLocalPlayer(), pos);
			Game.getClientInstance()->loopbackPacketSender->sendToServer(&a);
			pos.y += 0.35f;
			a = C_MovePlayerPacket(Game.getLocalPlayer(), pos);
			Game.getClientInstance()->loopbackPacketSender->sendToServer(&a);

			Vec3 vel = gm->player->getMovementProxy()->getVelocity();
			vel.y = 0.465f;
			gm->player->getMovementProxy()->setVelocity(vel);
			Vec3 moveVec;
			moveVec.x = cos(calcYaw) * cos(calcPitch) * horizontalSpeed;
			moveVec.z = sin(calcYaw) * cos(calcPitch) * horizontalSpeed;

			vel = gm->player->getMovementProxy()->getVelocity();
			vel.x = moveVec.x;
			vel.z = moveVec.z;
			gm->player->getMovementProxy()->setVelocity(vel);

			float teleportX = cos(calcYaw) * cos(calcPitch) * 0.00000005f;
			float teleportZ = sin(calcYaw) * cos(calcPitch) * 0.00000005f;

			pos = *gm->player->getPos();
			Game.getLocalPlayer()->getMovementProxy()->setPos(Vec3(pos.x + teleportX, pos.y - 0.15f, pos.z + teleportZ));

			vel = gm->player->getMovementProxy()->getVelocity();
			vel.y -= 0.15f;
			gm->player->getMovementProxy()->setVelocity(vel);
			gameTick = 0;
		}

		gm->player->getMovementProxy()->setVelocity(Vec3(0, 0, 0));

		break;
	}
	case 5:
	case 6:
		gm->player->getMovementProxy()->setVelocity(Vec3(0, 0, 0));
	}
}

void Fly::onDisable() {
	if (Game.getLocalPlayer() == nullptr)
		return;

	switch (mode.selected) {
	case 0:
		if (Game.getLocalPlayer()->gamemode != 1)
			Game.getLocalPlayer()->canFly = false;
		break;
	case 1:
	case 6:
		Game.getLocalPlayer()->getMovementProxy()->setVelocity(Vec3(0, 0, 0));
	}
}

void Fly::onMove(MoveInputHandler *input) {
	LocalPlayer *localPlayer = Game.getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	bool keyPressed = false;
	GameSettingsInput *inputf = Game.getClientInstance()->getGameSettingsInput();
	bool jumping = GameData::isKeyDown(*inputf->spaceBarKey);
	bool sneaking = GameData::isKeyDown(*inputf->sneakKey);

	float calcYaw = (localPlayer->getMovementProxy()->getRot().x + 90) * (PI / 180);
	float c = cos(calcYaw);
	float s = sin(calcYaw);

	Vec2 moveVec2D = {input->forwardMovement, -input->sideMovement};
	bool flag = moveVec2D.magnitude() > 0.f;

	moveVec2D = {moveVec2D.x * c - moveVec2D.y * s, moveVec2D.x * s + moveVec2D.y * c};
	Vec3 moveVec;

	switch (mode.selected) {
	case 5: {
		Vec3 *localPlayerPos = localPlayer->getPos();

		float yaw = localPlayer->getMovementProxy()->getRot().x;
		Vec2 moveVec2d = {input->forwardMovement, -input->sideMovement};
		bool pressed = moveVec2d.magnitude() > 0.01f;

		Vec3 vel = localPlayer->getMovementProxy()->getVelocity();

		if (input->isJumping) {
			vel.y += verticalSpeed;
			localPlayer->getMovementProxy()->setFallDistance(0.f);
		}

		if (input->isSneakDown) {
			vel.y -= verticalSpeed;
			localPlayer->getMovementProxy()->setFallDistance(0.f);
		}

		localPlayer->getMovementProxy()->setVelocity(vel);

		if (input->right) {
			yaw += 90.f;

			if (input->forward)
				yaw -= 45.f;
			else if (input->backward)
				yaw += 45.f;
		}

		if (input->left) {
			yaw -= 90.f;

			if (input->forward)
				yaw += 45.f;
			else if (input->backward)
				yaw -= 45.f;
		}

		if (input->backward && !input->left && !input->right)
			yaw += 180.f;

		if (pressed) {
			float calcYaw = (yaw + 90.f) * (PI / 180.f);
			Vec3 moveVec;
			moveVec.x = cos(calcYaw) * horizontalSpeed;
			moveVec.y = localPlayer->getMovementProxy()->getVelocity().y;
			moveVec.z = sin(calcYaw) * horizontalSpeed;
			localPlayer->lerpMotion(moveVec);
		}

		break;
	}
	case 6: {
		// Moonlight is an AntiCheat by disepi (Zephyr Developer)
		// This Fly/Glide worked on the Hive in the first half year of 2021
		// Idea from Weather Client (dead by now), TurakanFly from BadMan worked similar with less height loss

		if (!localPlayer->getMovementProxy()->isOnGround()) {
			localPlayer->getMovementProxy()->setVelocity(Vec3(localPlayer->getMovementProxy()->getVelocity().x, 0.f, localPlayer->getMovementProxy()->getVelocity().z));
		}

		GameSettingsInput *input = Game.getClientInstance()->getGameSettingsInput();

		if (input == nullptr)
			return;

		float yaw = localPlayer->getMovementProxy()->getRot().x;

		if (GameData::isKeyDown(*input->forwardKey) && GameData::isKeyDown(*input->backKey))
			return;
		else if (GameData::isKeyDown(*input->forwardKey) && GameData::isKeyDown(*input->rightKey) && !GameData::isKeyDown(*input->leftKey)) {
			yaw += 45.f;
			keyPressed = true;
		} else if (GameData::isKeyDown(*input->forwardKey) && GameData::isKeyDown(*input->leftKey) && !GameData::isKeyDown(*input->rightKey)) {
			yaw -= 45.f;
			keyPressed = true;
		} else if (GameData::isKeyDown(*input->backKey) && GameData::isKeyDown(*input->rightKey) && !GameData::isKeyDown(*input->leftKey)) {
			yaw += 135.f;
			keyPressed = true;
		} else if (GameData::isKeyDown(*input->backKey) && GameData::isKeyDown(*input->leftKey) && !GameData::isKeyDown(*input->rightKey)) {
			yaw -= 135.f;
			keyPressed = true;
		} else if (GameData::isKeyDown(*input->forwardKey)) {
			keyPressed = true;
		} else if (GameData::isKeyDown(*input->backKey)) {
			yaw += 180.f;
			keyPressed = true;
		} else if (GameData::isKeyDown(*input->rightKey) && !GameData::isKeyDown(*input->leftKey)) {
			yaw += 90.f;
			keyPressed = true;
		} else if (GameData::isKeyDown(*input->leftKey) && !GameData::isKeyDown(*input->rightKey)) {
			yaw -= 90.f;
			keyPressed = true;
		} else {
			Vec3 vel = localPlayer->getMovementProxy()->getVelocity();
			vel.x = 0.f;
			vel.z = 0.f;
			localPlayer->getMovementProxy()->setVelocity(vel);
			keyPressed = false;
		}

		if (yaw >= 180)
			yaw -= 360.f;

		float calcYaw = (yaw + 90) * (PI / 180);
		Vec3 moveVec;
		moveVec.x = cos(calcYaw) * horizontalSpeed;
		moveVec.y = localPlayer->getMovementProxy()->getVelocity().y;
		moveVec.z = sin(calcYaw) * horizontalSpeed;

		if (keyPressed) {
			localPlayer->lerpMotion(moveVec);
			keyPressed = false;
		}

		if (gameTick > 6) {
			if (!localPlayer->getMovementProxy()->isOnGround()) {
				Vec3 pos;
				pos.x = localPlayer->getPos()->x;
				pos.y = localPlayer->getPos()->y - 0.025f;
				pos.z = localPlayer->getPos()->z;

				localPlayer->getMovementProxy()->setPos(pos);
			}

			gameTick = 0;
		}
		break;
	}
	case 7:
		float motion = 0.0f;

		if (Game.canUseMoveKeys()) {
			if (jumping)
				motion += verticalSpeed;
			if (sneaking)
				motion -= verticalSpeed;
		}

		if (gameTick = 15 && !jumping && !sneaking) {
			motion = -0.04;
			gameTick = 0;
		}

		if (flag || jumping || sneaking) {
			moveVec.x = moveVec2D.x * horizontalSpeed;
			moveVec.y = motion;
			moveVec.z = moveVec2D.y * horizontalSpeed;

			localPlayer->lerpMotion(moveVec);
		}
		break;
	}
}
