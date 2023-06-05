#include "AutoGapple.h"
#include "../../../SDK/Attribute.h"

AutoGapple::AutoGapple() : IModule(0, Category::PLAYER, "Auto eat gapples if you're at low health.") {
	registerIntSetting("Health", &this->health, 10, 1, 20);
}

AutoGapple::~AutoGapple() {
}

const char* AutoGapple::getModuleName() {
	return ("AutoGapple");
}

int AutoGapple::findGapple() {
	auto player = Game.getLocalPlayer();

	if (player == nullptr) return -1;

	auto supplies = player->getSupplies();

	auto inv = supplies->inventory;

	for (int i = 0; i < 9; i++) {
		ItemStack* stack = inv->getItemStack(i);

		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 258) // Golden Apple
				return i;
		}
	}

	return -1;
}

void AutoGapple::onEnable() {
	auto player = Game.getLocalPlayer();

	if (player == nullptr) return;

	static HealthAttribute attribute = HealthAttribute();
	auto currentHealth = player->getAttribute(&attribute)->currentValue;

	this->lastHealth = currentHealth;

	int slot = this->findGapple();

	if (slot == -1) return;

	ItemStack* stack = Game.getLocalPlayer()->getSupplies()->inventory->getItemStack(slot);

	this->stackSize = stack->count;
}

void AutoGapple::onTick(GameMode* gm) {
	auto player = Game.getLocalPlayer();

	if (player == nullptr) return;

	static HealthAttribute attribute = HealthAttribute();
	auto currentHealth = player->getAttribute(&attribute)->currentValue;

	if (this->wait > 0 && this->lastHealth <= currentHealth) {
		this->lastHealth = currentHealth;
		this->wait--;
		return;
	}

	if (this->findGapple() == -1) return;

	// TODO: Effect check, functions must be implemented someday

	if (currentHealth < this->health) {
		auto supplies = player->getSupplies();

		auto inv = supplies->inventory;
		int lastSelected = supplies->selectedHotbarSlot;

		int gapple = this->findGapple();

		if (this->lastSlot != gapple) this->lastSlot = lastSelected;

		ItemStack* stack = inv->getItemStack(gapple);

		if (this->stackSize > stack->count) {
			this->stackSize = stack->count;
			supplies->selectedHotbarSlot = this->lastSlot;
			this->wait = 30;
			return;
		}

		supplies->selectedHotbarSlot = gapple;
		Game.getGameMode()->baseUseItem(*stack);
	}
}
