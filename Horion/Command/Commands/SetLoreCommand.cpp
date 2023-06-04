#include "SetLoreCommand.h"

SetLoreCommand::SetLoreCommand() : IMCCommand("setlore", "Sets the lore of an item", "<lore>") {
}

SetLoreCommand::~SetLoreCommand() {
}

bool SetLoreCommand::execute(std::vector<std::string>* args) {
	assertTrue(args->size() >= 2);

	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();

	auto transactionManager = Game.getLocalPlayer()->getTransactionManager();

	Inventory* inv = supplies->inventory;

	int selectedSlot = supplies->selectedHotbarSlot;

	ItemStack* item = inv->getItemStack(selectedSlot);

	if (item == nullptr || item->item == nullptr) {
		clientMessageF("Please hold an item!");
		return false;
	}

	std::string lore;

	for (int i = 1; i < args->size(); i++) {
		lore += args->at(i) + " ";
	}

	transactionManager->addInventoryAction(InventoryAction(selectedSlot, item, nullptr));
	transactionManager->addInventoryAction(InventoryAction(0, nullptr, item, 507, 99999));

	item->setLore(lore);

	transactionManager->addInventoryAction(InventoryAction(0, item, nullptr, 507, 99999));
	transactionManager->addInventoryAction(InventoryAction(selectedSlot, nullptr, item));
	transactionManager->addInventoryAction(InventoryAction(selectedSlot, nullptr, item));

	clientMessageF("Set lore to '%s'!", lore);
	return true;
}
