#include "DeviceIDCommand.h"

DeviceIDCommand::DeviceIDCommand() : IMCCommand("deviceid", "Displays the device id from you or another player", "[playername]") {
}

DeviceIDCommand::~DeviceIDCommand() {
}

bool DeviceIDCommand::execute(std::vector<std::string>* args) {
	if (args->size() < 2) {
		auto player = Game.getLocalPlayer();

		clientMessageF("%sYour device: %s%s", GREEN, GRAY, player->getDeviceName());
	}
	else {
		/*std::string nameOfPlayer = args->at(1);
		assertTrue(!nameOfPlayer.empty());
		std::string nameOfPlayerLower = std::string(nameOfPlayer);
		std::transform(nameOfPlayerLower.begin(), nameOfPlayerLower.end(), nameOfPlayerLower.begin(), ::tolower);
		nameOfPlayerLower = Utils::sanitize(nameOfPlayerLower);

		EntityList* entList = Game.getEntityList();
		size_t listSize = entList->getListSize();
		Vec3 pos;

		if (listSize > 5000) {
			return true;
		}

		Player* tar = nullptr;

		// Loop through all our players and retrieve their information
		for (size_t i = 0; i < listSize; i++) {
			Entity* currentEntity = entList->get(i);

			std::string name(currentEntity->getNameTag()->getText());

			std::transform(name.begin(), name.end(), name.begin(), ::tolower);

			if (currentEntity == 0)
				break;

			if (currentEntity == Game.getLocalPlayer())  // Skip Local player
				continue;

			if (name.find(nameOfPlayerLower) == std::string::npos)
				continue;

			tar = static_cast<Player*>(currentEntity);
			break;
		}

		if (tar == nullptr) {
			clientMessageF("[%sHorion%s] %sCouldn't find player: %s!", GOLD, WHITE, RED, nameOfPlayer.c_str());
			return true;
		}

		clientMessageF("%sDevice from %s%s%s: %s%s", GREEN, GRAY, tar->getNameTag()->getText(), GREEN, GRAY, tar->getDeviceName());*/
		clientMessageF("%sCurrently crashing", RED);
	}

	return true;
}
