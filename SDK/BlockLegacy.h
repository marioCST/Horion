#pragma once

#include "../Utils/HMath.h"
#include "../Utils/Utils.h"
#include "TextHolder.h"
//#include "Tag.h"

class Material {
public:
	BUILD_ACCESS(this, int, type, 0x0); // 5 for water, 6 for lava
	BUILD_ACCESS(this, bool, isFlammable, 0x4);
	BUILD_ACCESS(this, bool, isNeverBuildable, 0x5);
	BUILD_ACCESS(this, bool, isAlwaysDestroyable, 0x6);
	BUILD_ACCESS(this, bool, isReplaceable, 0x7);
	BUILD_ACCESS(this, bool, isLiquid, 0x8);
	BUILD_ACCESS(this, float, translucency, 0xC);
	BUILD_ACCESS(this, bool, isBlockingMotion, 0x10);
	BUILD_ACCESS(this, bool, isBlockingPrecipitation, 0x11);
	BUILD_ACCESS(this, bool, isSolid, 0x12);
	BUILD_ACCESS(this, bool, isSuperHot, 0x13);
	BUILD_ACCESS(this, float, color1, 0x14);
	BUILD_ACCESS(this, float, color2, 0x18);
	BUILD_ACCESS(this, float, color3, 0x1C);
	BUILD_ACCESS(this, float, color4, 0x20);
};

class Entity;
class Block;
class BlockSource;

class BlockLegacy {
public:
	BUILD_ACCESS(this, uintptr_t**, Vtable, 0x0);
	BUILD_ACCESS(this, class TextHolder, tileName, 0x08);
	BUILD_ACCESS(this, class TextHolder, name, 0x30);
	BUILD_ACCESS(this, class Material*, material, 0xD8);
	BUILD_ACCESS(this, short, blockId, 0x14C);

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
	virtual Block* getBlock(int, int, int);
	virtual Block* getBlock(Vec3i const&);
	virtual Block* getBlock(Vec3i const&, int);
	virtual BlockActor* getBlockEntity(Vec3i const&);
	virtual Block* getExtraBlock(Vec3i const&);
	virtual Block* getLiquidBlock(Vec3i const&);
	virtual bool hasBlock(Vec3i const&);
	virtual bool containsMaterial(AABB const&, struct MaterialType);
	virtual Material* getMaterial(Vec3i const&);
	virtual Material* getMaterial(int, int, int);
	virtual bool hasChunksAt(struct Bounds const&);
	virtual bool hasChunksAt(Vec3i const&, int);
	virtual bool hasChunksAt(AABB const&);
	virtual int getDimensionId(void);
	virtual __int64 getWeakRef(void);
	virtual void addListener(struct BlockSourceListener&);
	virtual void removeListener(BlockSourceListener&);
	virtual class EntityList* fetchEntities(Entity const*, AABB const&, bool);
	virtual class EntityList* fetchEntities(__int64, AABB const&, Entity const*);
	virtual void setBlock(Vec3i const&, Block const&, int, class ActorBlockSyncMessage const*, Entity*);
	virtual bool containsAnyLiquid(AABB const&);
	virtual int getMinHeight(void);
	virtual class Dimension* getDimension(void);
	virtual class Dimension* getDimensionConst(void);
	virtual __int64 getILevel(void);
	virtual std::vector<AABB>* fetchAABBs(AABB const&, bool);
	virtual std::vector<AABB>* fetchCollisionShapes(AABB const&, float*, bool, class IActorMovementProxy*);
	virtual AABB* getTallestCollisionShape(AABB const&, float*, bool, IActorMovementProxy*);
	virtual __int64 getChunkSource(void);
	virtual bool isSolidBlockingBlock(Vec3i const&);
};
