#include "SetLoreCommand.h"

SetLoreCommand::SetLoreCommand() : IMCCommand("setlore", "Sets the lore of an item", "<lore>") {
}

SetLoreCommand::~SetLoreCommand() {
}

void setLore(ItemStack* stack, std::vector<std::string> lore) {
	using constructor_t = void(__fastcall*)(ItemStack*, std::vector<std::string>);
	static constructor_t func = reinterpret_cast<constructor_t>(FindSignature("48 8B C4 48 89 58 ? 48 89 70 ? 55 57 41 54 41 56 41 57 48 8D 68 ? 48 81 EC ? ? ? ? ? ? 70 C8 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4C"));
	func(stack, lore);
}

bool SetLoreCommand::execute(std::vector<std::string>* args) {
	assertTrue(args->size() >= 2);

	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();

	auto transactionManager = Game.getLocalPlayer()->getTransactionManager();

	Inventory* inv = supplies->inventory;

	int selectedSlot = supplies->selectedHotbarSlot;

	ItemStack* item = inv->getItemStack(selectedSlot);

	if (item == nullptr) {
		clientMessageF("Please hold an item!");
		return false;
	}

	std::string lore;

	for (int i = 1; i < args->size(); i++) {
		lore += args->at(i) + " ";
	}

	std::vector<std::string> loreList;
	loreList.push_back(lore);

	setLore(item, loreList);

	clientMessageF("Set lore to '%s'!", lore);
	return true;
}
