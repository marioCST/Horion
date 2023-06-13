#pragma once
#include "../Utils/HMath.h"
#include "Inventory.h"
#include "InventoryTransaction.h"

class ComplexInventoryTransaction {
public:
	BUILD_ACCESS(this, uintptr_t**, vTable, 0x0);
	BUILD_ACCESS(this, int, actionType, 0x8);
	BUILD_ACCESS(this, int, unknown, 0xC);
	BUILD_ACCESS(this, InventoryTransaction, transac, 0x10);

	ComplexInventoryTransaction();
	ComplexInventoryTransaction(InventoryTransaction& transac);
};

class ItemUseInventoryTransaction : public ComplexInventoryTransaction {
public:
	BUILD_ACCESS(this, Vec3i, blockPos, 0x6C);
	BUILD_ACCESS(this, int, runtimeBlockId, 0x78);
	BUILD_ACCESS(this, int, blockSide, 0x7C);
	BUILD_ACCESS(this, int, slot, 0x80);
	BUILD_ACCESS(this, ItemStack, item, 0x88);
	BUILD_ACCESS(this, Vec3, pos, 0x110);
	BUILD_ACCESS(this, Vec3, clickPos, 0x11C);

	ItemUseInventoryTransaction();
	ItemUseInventoryTransaction(int slot, ItemStack const* item, Vec3 const pos, int blockSide = 255, int runtimeBlockId = 0);
};

class ItemReleaseInventoryTransaction : public ComplexInventoryTransaction {
public:
	BUILD_ACCESS(this, int, slot, 0x6C);
	BUILD_ACCESS(this, ItemStack, item, 0x70);
	BUILD_ACCESS(this, Vec3, pos, 0xF8);
	BUILD_ACCESS(this, int, unknown, 0x104);

	ItemReleaseInventoryTransaction();
	ItemReleaseInventoryTransaction(int slot, ItemStack const* item, Vec3 const pos);
};
