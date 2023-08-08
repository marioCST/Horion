#pragma once

#include "../Utils/HMath.h"
#include "../Utils/Utils.h"
#include "PlayerMovementProxy.h"
#include "TextHolder.h"
//#include "Tag.h"

class Material {
public:
	BUILD_ACCESS(this, int, type, 0x0);
	BUILD_ACCESS(this, bool, isFlammable, 0x4);
	BUILD_ACCESS(this, bool, isNeverBuildable, 0x5);
	BUILD_ACCESS(this, bool, isLiquid, 0x6);
	BUILD_ACCESS(this, bool, isBlockingMotion, 0xC);
	BUILD_ACCESS(this, bool, isSuperHot, 0xF);
};

class Entity;
class Block;
class BlockSource;

class MCColor {
public:
	union {
		struct {
			float red;
			float green;
			float blue;
			float alpha;
		};
		float color[4];
	};
};

class BlockLegacy {
public:
	BUILD_ACCESS(this, uintptr_t**, Vtable, 0x0);
	BUILD_ACCESS(this, TextHolder, tileName, 0x28);
	BUILD_ACCESS(this, TextHolder, name, 0x78);
	BUILD_ACCESS(this, Material*, material, 0xD8);
	BUILD_ACCESS(this, float, translucency, 0x11C);
	BUILD_ACCESS(this, bool, isSolid, 0x144);
	BUILD_ACCESS(this, short, blockId, 0x154);
	BUILD_ACCESS(this, float, thickness, 0x218);
	BUILD_ACCESS(this, MCColor, color, 0x440);

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

private:
	virtual void Destructor();

public:
	virtual int getRenderLayer();
};

class CompoundTag;

class BlockActor {
private:
	virtual void Destructor();
	virtual __int64 load(__int64&, CompoundTag*, __int64&);

public:
	virtual bool save(CompoundTag*);
};

class BlockSource {
private:
	virtual void Destructor();

public:
	virtual Block* getBlock(Vec3i const&, uint32_t);
	virtual Block* getBlock(Vec3i const&);
	virtual Block* getBlock(int, int, int);
	virtual BlockActor* getBlockEntity(Vec3i const&);
	virtual Block* getExtraBlock(Vec3i const&);
	virtual Block* getLiquidBlock(Vec3i const&);
	virtual bool hasBlock(Vec3i const&);
	virtual void containsAnyLiquid(AABB const&);
	virtual bool containsMaterial(AABB const&, struct MaterialType);
	virtual Material* getMaterial(int, int, int);
	virtual Material* getMaterial(Vec3i const&);
	virtual bool hasChunksAt(AABB const&, bool);
	virtual bool hasChunksAt(Vec3i const&, int, bool);
	virtual bool hasChunksAt(struct Bounds const&, bool);
	virtual int getDimensionId(void);
	virtual std::vector<AABB>* fetchAABBs(std::vector<AABB> &, bool);
	virtual std::vector<AABB>* fetchCollisionShapes(std::vector<AABB> &, float*, bool, __int64);
	virtual AABB* getTallestCollisionShape(AABB const&, float*, bool, __int64);
	virtual __int64 getWeakRef(void);
	virtual void addListener(struct BlockSourceListener&);
	virtual void removeListener(BlockSourceListener&);
	virtual class EntityList* fetchEntities(__int64, AABB const&, Entity const*);
	virtual class EntityList* fetchEntities(Entity const*, AABB const&, bool);
	virtual void setBlock(Vec3i const&, Block const&, int, class ActorBlockSyncMessage const*, Entity*);
	virtual int getMaxHeight(void);
	virtual int getMinHeight(void);
	virtual class Dimension* getDimension(void);
	virtual class Dimension* getDimensionConst(void);
	virtual class Dimension* getDimension2(void);
	virtual int getChunkAt(Vec3i const&);
	virtual __int64 getILevel(void);
	virtual std::vector<AABB>* fetchAABBs(AABB const&, bool);
	virtual std::vector<AABB>* fetchCollisionShapes(AABB const&, float*, bool, PlayerMovementProxy*);
	virtual __int64 getChunkSource(void);
	virtual bool isSolidBlockingBlock(Vec3i const&);
	virtual bool isSolidBlockingBlock(int, int, int);
};