#pragma once

#include "../Utils/HMath.h"
class Entity;

class EntityList {
public:
	BUILD_ACCESS(this, uintptr_t, firstEntity, 0x70);
	BUILD_ACCESS(this, uintptr_t, lastEntity, 0x78);
	BUILD_ACCESS(this, uintptr_t, endAddr, 0x80);

	inline size_t getListSize() {
		return (lastEntity - firstEntity) / sizeof(uintptr_t);
	}

	inline Entity* get(size_t idx) {
		if (idx >= getListSize())
			return 0;
		return *reinterpret_cast<Entity**>(firstEntity + (idx * sizeof(uintptr_t)));
	}

	Entity* operator[](size_t idx) {
		return get(idx);
	}
};  //Size=0x0050

class MobEntityList {
public:
	BUILD_ACCESS(this, uintptr_t, firstEntity, 0x38);
	BUILD_ACCESS(this, uintptr_t, lastEntity, 0x40);
	BUILD_ACCESS(this, uintptr_t, endAddr, 0x48);

	inline size_t getListSize() {
		return (lastEntity - firstEntity) / sizeof(uintptr_t);
	}

	inline Entity* get(size_t idx) {
		if (idx >= getListSize())
			return 0;
		return *reinterpret_cast<Entity**>(firstEntity + (idx * sizeof(uintptr_t)));
	}

	Entity* operator[](size_t idx) {
		return get(idx);
	}
};  //Size=0x0050