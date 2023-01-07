#include "NoFall.h"

NoFall::NoFall() : IModule(VK_NUMPAD3, Category::PLAYER, "Prevents you from taking falldamage") {
	mode = (*new SettingEnum(this))
		.addEntry(EnumEntry("GroundSpoof", 0))
		.addEntry(EnumEntry("CubeCraft", 1))
		.addEntry(EnumEntry("Nukkit", 2));
		//.addEntry(EnumEntry("Vanilla", 3))
	registerEnumSetting("Mode", &mode, 0);
}

NoFall::~NoFall() {
}

const char* NoFall::getModuleName() {
	return ("NoFall");
}

void NoFall::onSendPacket(Packet* packet) {
	LocalPlayer* localPlayer = Game.getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	if (localPlayer->fallDistance > 2.f && mode.selected == 0) {
		if (packet->isInstanceOf<C_MovePlayerPacket>()) {
			C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			movePacket->onGround = true;
		}
	}
}

void NoFall::onTick(GameMode* gm) {
	LocalPlayer* localPlayer = Game.getLocalPlayer();

	if (localPlayer->fallDistance > 2.f) {
		switch (mode.selected) {
		/*case 3:
			PlayerActionPacket actionPacket;
			actionPacket.action = 7; //Respawn
			actionPacket.entityRuntimeId = localPlayer->entityRuntimeId;
			Game.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);
			break;*/
		case 1:
			localPlayer->velocity.y = 0.f;
			localPlayer->setPos((*localPlayer->getPos()).add(0, 0.2, 0));
			break;
		case 2:
			PlayerActionPacket actionPacket;
			actionPacket.action = 15;  //Open Elytra, I have a fix for that server side
			actionPacket.entityRuntimeId = localPlayer->entityRuntimeId;
			Game.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);
			break;
		}
	}
}
