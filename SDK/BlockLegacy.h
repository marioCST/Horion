#pragma once

#include "../Utils/HMath.h"
#include "../Utils/Utils.h"
#include "TextHolder.h"
//#include "Tag.h"

class Material {
public:
	BUILD_ACCESS(this, int, type, 0x0);
	BUILD_ACCESS(this, bool, isFlammable, 0x4);
	BUILD_ACCESS(this, bool, isNeverBuildable, 0x5);
	BUILD_ACCESS(this, bool, isReplaceable, 0x6);
	BUILD_ACCESS(this, bool, isLiquid, 0x8);
	BUILD_ACCESS(this, bool, isSolid, 0xC);
	BUILD_ACCESS(this, bool, isBlockingMotion, 0xD);
	BUILD_ACCESS(this, bool, isSuperHot, 0xF);

	bool isType(int materialType) {
		using isType = bool(__thiscall *)(Material*, int);
		static isType isTypeFunc = reinterpret_cast<isType>(FindSignature("83 FA 35 74 07"));
		return isTypeFunc(this, materialType);
	}
};

class Entity;
class Block;
class BlockSource;

class BlockLegacy {
public:
	BUILD_ACCESS(this, uintptr_t**, Vtable, 0x0);
	BUILD_ACCESS(this, TextHolder, tileName, 0x8);
	BUILD_ACCESS(this, TextHolder, name, 0x30);
	BUILD_ACCESS(this, Material*, material, 0x108);
	BUILD_ACCESS(this, float, translucency, 0x14C);
	BUILD_ACCESS(this, short, blockId, 0x186);

	int liquidGetDepth(BlockSource*, const Vec3i* pos);
	void liquidGetFlow(Vec3* flowOut, BlockSource*, const Vec3i* pos);
	bool getCollisionShape(AABB* collShapeOut, Block* block, BlockSource* blockSource, const Vec3i* pos, Entity* actor);
	bool hasWater(BlockSource*, const Vec3i& pos);
};

class Block {
public:
	BUILD_ACCESS(this, uint8_t, data, 0x8);
	BUILD_ACCESS(this, BlockLegacy*, blockLegacy, 0x10);

	inline BlockLegacy* toLegacy() { return blockLegacy; }

	virtual ~Block();
	virtual int getRenderLayer();
};

class CompoundTag;

class BlockActor {
private:
	virtual void destructor();
	virtual __int64 load(__int64&, CompoundTag*, __int64&);

public:
	virtual bool save(CompoundTag*);
};

class BlockSource {
public:
	virtual void Destructor();
	virtual Block* getBlock(Vec3i const &, uint32_t);
	virtual Block* getBlock(Vec3i const &);
	virtual Block* getBlock(int, int, int);
	virtual BlockActor* getBlockEntity(Vec3i const &);
	virtual Block* getExtraBlock(Vec3i const &);
	virtual Block* getLiquidBlock(Vec3i const &);
	virtual bool hasBlock(Vec3i const &);
	virtual bool containsAnyLiquid(__int64 const &, AABB const &);
	virtual bool containsMaterial(AABB const &, struct MaterialType);
	virtual Material* getMaterial(int, int, int);
	virtual Material* getMaterial(Vec3i const &);
	virtual bool hasChunksAt(AABB const &, bool);
	virtual bool hasChunksAt(Vec3i const &, int, bool);
	virtual bool hasChunksAt(__int64 const &, bool);
	virtual int getDimensionId(void);
	virtual std::vector<AABB>* fetchAABBs(std::vector<AABB, std::allocator<AABB>> &, AABB const &, bool);
	virtual std::vector<AABB>* fetchCollisionShapes(std::vector<AABB, std::allocator<AABB>> &, AABB const &, float *, bool, __int64);
	virtual float getTallestCollisionShape(AABB const &, float *, bool, __int64);
	virtual int getBrightness(Vec3i const &);
	virtual void fetchAABBs(AABB const &, bool);
	virtual void fetchCollisionShapes(AABB const &, float *, bool, __int64 *);
	virtual __int64 getWeakRef(void);
	virtual void addListener(__int64 &);
	virtual void removeListener(__int64 &);
	virtual void fetchEntities(int, AABB const &, Entity const *, __int64);
	virtual void fetchEntities(Entity const *, AABB const &, bool, bool);
	virtual void setBlock(Vec3i const &, Block const &, int, __int64 const *, Entity *);
	virtual int16_t getMaxHeight(void);
	virtual int16_t getMinHeight(void);

private:
	virtual void TryroFunc_FlagVil29();  // 29
	virtual void TryroFunc_FlagVil30();  // 30
	virtual void TryroFunc_FlagVil31();  // 31

public:
	virtual __int64 getChunkAt(Vec3i const &);
	virtual Entity* getKilledEntity(void);

private:
	virtual void TryroFunc_FlagVil34();  // 34

public:
	virtual bool isSolidBlockingBlock(int, int, int);
	virtual bool isSolidBlockingBlock(Vec3i const &);
	virtual bool areChunksFullyLoaded(Vec3i const &, int);
	virtual bool canDoBlockDrops(void);
	virtual bool canDoContainedItemDrops(void);
};
