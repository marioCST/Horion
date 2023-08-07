#pragma once

#include <bitset>

#include "../Utils/HMath.h"
#include "../Utils/Utils.h"
#include "ActorFlags.h"
#include "Attribute.h"
#include "BlockLegacy.h"
#include "Components.h"
#include "Dimension.h"
#include "EntityList.h"
#include "Inventory.h"
#include "InventoryTransaction.h"
#include "PlayerMovementProxy.h"
#include "TextHolder.h"

class GameMode;

class Level {
	char pad_0x0[0xB30];
public:
	int rayHitType;
	int blockSide;
	Vec3i block;
	Vec3 rayHitVec;
	Entity *entityPtr;
	Entity *entityPtr2;
	uint64_t GamingEntityFinder;

	BUILD_ACCESS(this, int, levelTicks, 0x620);

public:
	bool hasEntity();     // to not go trough the entity list twice
	Entity *getEntity();  // returns the entity that the player is looking at
	int getLevelTicks() {
		// return levelTicks;
		return *reinterpret_cast<int *>(reinterpret_cast<__int64>(this) + 0x620);
	}

	class LoopbackPacketSender *getLoopbackPacketSender() {
		return *reinterpret_cast<class LoopbackPacketSender **>(reinterpret_cast<__int64>(this) + 0xA58);
	}

	void playSound(std::string sound, Vec3 const &position, float volume, float pitch) {
		static uintptr_t sig = 0x0;

		if (sig == 0)
			sig = FindSignature("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B 81 ? ? ? ? 33 FF 48 2B 81 ? ? ? ? 49 8B F0 48 C1 F8 ? 48 8B EA");

		using playSound_t = void(__fastcall *)(Level *, TextHolder *, Vec3 const &, float, float);
		static playSound_t func = reinterpret_cast<playSound_t>(sig);
		if (func != nullptr) func(this, &TextHolder(sound), position, volume, pitch);
	}

	// Credits to hacker hansen for this
	std::vector<Entity *> getMiscEntityList() { // Level::getRuntimeActorList
		static uintptr_t sig = 0x0;

		if (sig == 0)
			sig = FindSignature("48 89 5C 24 ? 56 57 41 56 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B F2 4C 8B F1 48 89 54 24 ? 33 C9");

		using entityList_t = std::int64_t *(__fastcall *)(Level *, void *);
		static entityList_t func = reinterpret_cast<entityList_t>(sig);
		if (func != nullptr) {
			std::unique_ptr<char[]> alloc = std::make_unique<char[]>(0x18);
			std::int64_t *listStart = func(this, alloc.get());
			std::size_t listSize = ((*reinterpret_cast<std::int64_t *>(reinterpret_cast<std::int64_t>(listStart) + 0x8)) - (*listStart)) / 0x8;
			Entity **entityList = reinterpret_cast<Entity **>(*listStart);
			std::vector<Entity *> res;
			res.reserve(listSize);
			if (listSize > 0 && listSize < 5000) {
				for (std::size_t i = 0; i < listSize; i++) res.push_back(entityList[i]);
			}
			return res;
		}
		return {};
	}
};

class Player;
class Dimension;
class MobEffect;
struct MobEffectInstance;
class Packet;
class BlockActor;

struct EntityLocation {
	Vec3 pos;
	Vec3 posPrev;
	Vec3 velocity;
};

enum class GameType : int {
	Survival = 0,
	Creative = 1,
	Adventure = 2,
	// SurvivalViewer = 3,
	// CreativeViewer = 4,
	Default = 5,
	Spectator = 6
};

#pragma pack(push, 4)
class Entity {
public:
	BUILD_ACCESS(this, __int64 **, entityRegistryBase, 0x8);
	BUILD_ACCESS(this, uint32_t, entityId, 0x10);
	BUILD_ACCESS(this, int, ticksAlive, 0x24C);
	BUILD_ACCESS(this, int, damageTime, 0x250);
	BUILD_ACCESS(this, BlockSource *, region, 0x2F8);
	BUILD_ACCESS(this, Dimension *, dimension, 0x300);
	BUILD_ACCESS(this, Level *, level, 0x310);
	BUILD_ACCESS(this, EntityLocation *, entityLocation, 0x350);
	BUILD_ACCESS(this, AABB*, aabb, 0x358);
	BUILD_ACCESS(this, int64_t, entityRuntimeId, 0x418);
	BUILD_ACCESS(this, int, deviceIdentifier, 0x848);
	BUILD_ACCESS(this, Vec3, eyePos0, 0xD90);
	BUILD_ACCESS(this, GameType, gamemode, 0x219C);

	virtual int getStatusFlag(ActorFlags);
	virtual void setStatusFlag(ActorFlags, bool);
	virtual int getLastHurtByEntity(void);
	virtual void setLastHurtByEntity(Entity *);
	virtual int getLastHurtByPlayer(void);
	virtual void setLastHurtByPlayer(Player *);
	virtual int getLastHurtEntity(void);
	virtual void setLastHurtEntity(Entity *);
	virtual void outOfWorld(void);
	virtual void reloadHardcoded(__int64, __int64 const &);
	virtual void reloadHardcodedClient(__int64, __int64 const &);
	virtual void initializeComponents(__int64, __int64 const &);
	virtual void reloadComponents(__int64, __int64 const &);
	virtual void _serverInitItemStackIds(void);
	virtual void _doInitialMove(void);
	virtual bool hasComponent(HashedString const &);

private:
	virtual void Destructor();

public:
	virtual void resetUserPos(bool);
	virtual int getDimensionId(void);
	virtual int getOwnerEntityType(void);
	virtual void remove(void);
	virtual void setPos(Vec3 const &);
	virtual bool isRuntimePredictedMovementEnabled(void);
	virtual int getPredictedMovementValues(void);
	virtual Vec3* getPos(void);
	virtual Vec3* getPosOld(void);
	virtual float getPosExtrapolated(float);
	virtual float getAttachPos(__int64, float);
	virtual int getFiringPos(void);
	virtual void move(Vec3 const &);
	virtual void move(__int64 &, Vec3 const &);
	virtual float getInterpolatedRidingPosition(float);
	virtual float getInterpolatedBodyRot(float);
	virtual float getInterpolatedHeadRot(float);
	virtual float getInterpolatedBodyYaw(float);
	virtual int getYawSpeedInDegreesPerSecond(void);
	virtual float getInterpolatedWalkAnimSpeed(float);
	virtual float getInterpolatedRidingOffset(float, int);
	virtual void resetInterpolated(void);
	virtual void updateEntityInside(AABB const &);
	virtual void updateEntityInside(void);
	virtual bool isFireImmune(void);
	virtual void breaksFallingBlocks(void);
	virtual void blockedByShield(__int64 const &, Entity &);
	virtual void canDisableShield(void);
	virtual void teleportTo(Vec3 const &, bool, int, int, bool);
	virtual void tryTeleportTo(Vec3 const &, bool, bool, int, int);
	virtual void chorusFruitTeleport(Vec3 const &);
	virtual void lerpMotion(Vec3 const &);
	virtual void tryCreateAddEntityPacket(void);
	virtual void normalTick(void);
	virtual void baseTick(void);
	virtual void passengerTick(void);
	virtual void positionPassenger(Entity &, float);
	virtual void startRiding(Entity &);
	virtual void addPassenger(Entity &);
	virtual void flagPassengerToRemove(Entity &);
	virtual int getExitTip(TextHolder const &, __int64);
	virtual int getEntityLocNameString(void);
	virtual void intersects(Vec3 const &, Vec3 const &);
	virtual bool isInWall(void);
	virtual bool isInvisible(void);
	virtual bool canShowNameTag(void);
	virtual void canExistInPeaceful(void);
	virtual void setNameTagVisible(bool);
	virtual TextHolder* getNameTag(void);
	virtual int getNameTagAsHash(void);
	virtual int getFormattedNameTag(void);
	virtual void filterFormattedNameTag(__int64 const &);
	virtual void setNameTag(TextHolder const &);
	virtual int getAlwaysShowNameTag(void);
	virtual void setScoreTag(TextHolder const &);
	virtual int getScoreTag(void);
	virtual bool isInWater(void);
	virtual bool hasEnteredWater(void);
	virtual bool isInLava(BlockSource const &);
	virtual bool isUnderLiquid(__int64);
	virtual bool isOverWater(void);
	virtual void setBlockMovementSlowdownMultiplier(BlockLegacy const &, Vec3 const &);
	virtual void resetBlockMovementSlowdownMultiplier(void);
	virtual int getCameraOffset(void);
	virtual int getShadowHeightOffs(void);
	virtual int getShadowRadius(void);
	virtual float getHeadLookVector(float);
	virtual bool canSeeInvisible(void);
	virtual bool canSee(Vec3 const &);
	virtual bool canSee(Entity const &);
	virtual void canInteractWithOtherEntitiesInGame(void);
	virtual bool isSkyLit(float);
	virtual float getBrightness(float);
	virtual void interactPreventDefault(void);
	virtual void playerTouch(Player &);
	virtual void onAboveBubbleColumn(bool);
	virtual void onInsideBubbleColumn(bool);
	virtual bool isImmobile(void);
	virtual bool isSilent(void);
	virtual bool isSilentObserver(void);
	virtual bool isPickable(void);
	virtual bool isFishable(void);
	virtual bool isSleeping(void);
	virtual void setSleeping(bool);
	virtual bool isShootable(void);
	virtual void setSneaking(bool);
	virtual bool isBlocking(void);
	virtual bool isDamageBlocked(__int64 const &);
	virtual bool isAlive(void);
	virtual bool isOnFire(void);
	virtual bool isOnHotBlock(void);
	virtual bool isCreativeModeAllowed(void);
	virtual bool isSurfaceEntity(void);
	virtual bool isTargetable(void);
	virtual bool isLocalPlayer(void);
	virtual bool isRemotePlayer(void);
	virtual bool isPlayer(void);
	virtual bool isAffectedByWaterBottle(void);
	virtual bool canAttack(Entity *, bool);
	virtual void setTarget(Entity *);
	virtual bool isValidTarget(Entity *);
	virtual void attack(Entity &, __int64 const &);
	virtual void performRangedAttack(Entity &, float);
	virtual int getEquipmentCount(void);
	virtual void setOwner(uint64_t);
	virtual void setSitting(bool);
	virtual void onTame(void);
	virtual void onFailedTame(void);
	virtual int getInventorySize(void);
	virtual int getEquipSlots(void);
	virtual int getChestSlots(void);
	virtual void setStanding(bool);
	virtual void canPowerJump(void);
	virtual void setCanPowerJump(bool);
	virtual bool isJumping(void);
	virtual bool isEnchanted(void);
	virtual void vehicleLanded(Vec3 const &, Vec3 const &);
	virtual void shouldRender(void);
	virtual void playAmbientSound(void);
	virtual int getAmbientSound(void);
	virtual bool isInvulnerableTo(__int64 const &);
	virtual int getBlockDamageCause(Block const &);
	virtual void animateHurt(void);
	virtual void doFireHurt(int);
	virtual void onLightningHit(void);
	virtual void onBounceStarted(Vec3i const &, Block const &);
	virtual void feed(int);
	virtual void handleEntityEvent(__int64, int);
	virtual int getPickRadius(void);
	virtual int getEntityRendererId(void);
	virtual void spawnAtLocation(int, int);
	virtual void spawnAtLocation(int, int, float);
	virtual void spawnAtLocation(Block const &, int);
	virtual void spawnAtLocation(Block const &, int, float);
	virtual void spawnAtLocation(ItemStack const &, float);
	virtual void despawn(void);
	virtual void killed(Entity &);
	virtual void awardKillScore(Entity &, int);
	virtual void setArmor(int, ItemStack const &);
	virtual ItemStack* getArmor(int);
	virtual int getAllArmor(void);
	virtual int getArmorMaterialTypeInSlot(int);
	virtual int getArmorMaterialTextureTypeInSlot(int);
	virtual int getArmorColorInSlot(int, int);
	virtual int getEquippedSlot(int);
	virtual void setEquippedSlot(int, ItemStack *);
	virtual void setCarriedItem(ItemStack *);
	virtual int getCarriedItem(void);
	virtual void setOffhandSlot(ItemStack *);
	virtual ItemStack* getEquippedTotem(void);
	virtual void consumeTotem(void);
	virtual void save(CompoundTag *);
	virtual void saveWithoutId(CompoundTag *);
	virtual void load(CompoundTag const &, __int64 &);
	virtual void loadLinks(CompoundTag const &, __int64 &, __int64 &);
	virtual int getEntityTypeId(void);
	virtual void queryEntityRenderer(void);
	virtual int getSourceUniqueID(void);
	virtual void thawFreezeEffect(void);
	virtual void canFreeze(void);
	virtual bool isWearingLeatherArmor(void);
	virtual int getLiquidAABB(__int64);
	virtual void handleInsidePortal(Vec3i const &);
	virtual int getPortalCooldown(void);
	virtual int getPortalWaitTime(void);
	virtual void canChangeDimensionsUsingPortal(void);
	virtual void changeDimension(int);
	virtual void changeDimension(__int64 const &);
	virtual int getControllingPlayer(void);
	virtual void checkFallDamage(float, bool);
	virtual void causeFallDamage(float, float, __int64);
	virtual void handleFallDistanceOnServer(float, float, bool);
	virtual void playSynchronizedSound(__int64, Vec3 const &, Block const &, bool);
	virtual void playSynchronizedSound(__int64, Vec3 const &, int, bool);
	virtual void onSynchedFlagUpdate(int, long, long);
	virtual void onSynchedDataUpdate(int);
	virtual void canAddPassenger(Entity &);
	virtual void canPickupItem(ItemStack const &);
	virtual void canBePulledIntoVehicle(void);
	virtual void inCaravan(void);
	virtual bool isLeashableType(void);
	virtual void tickLeash(void);
	virtual void sendMotionPacketIfNeeded(void);
	virtual void canSynchronizeNewEntity(void);
	virtual void stopRiding(bool, bool, bool);
	virtual void startSwimming(void);
	virtual void stopSwimming(void);
	virtual void buildDebugInfo(TextHolder &);
	virtual int getCommandPermissionLevel(void);
	virtual bool isClientSide(void);
	virtual class AttributeInstance *getMutableAttribute(class Attribute *Attribute);
	virtual class AttributeInstance *getAttribute(class Attribute *Attribute);
	virtual int getDeathTime(void);
	virtual void heal(int);
	virtual bool isInvertedHealAndHarm(void);
	virtual void canBeAffected(uint32_t);
	virtual void canBeAffectedByArrow(MobEffectInstance const &);
	virtual void onEffectAdded(MobEffectInstance &);
	virtual void onEffectUpdated(MobEffectInstance &);
	virtual void onEffectRemoved(MobEffectInstance &);
	virtual void canObstructSpawningAndBlockPlacement(void);
	virtual int getAnimationComponent(void);
	virtual void openContainerComponent(Player &);
	virtual void swing(void);
	virtual void useItem(ItemStack &, __int64, bool);
	virtual void hasOutputSignal(uint8_t);
	virtual int getOutputSignal(void);
	virtual int getDebugText(std::vector<TextHolder> &);
	virtual int getMapDecorationRotation(void);
	virtual int getPassengerYRotation(Entity const &);
	virtual bool isWorldBuilder(void);
	virtual bool isCreative(void);
	virtual bool isAdventure(void);
	virtual bool isSurvival(void);
	virtual bool isSpectator(void);
	virtual bool isAttackableGamemode(void);
	virtual void add(ItemStack &);
	virtual void drop(ItemStack const &, bool);
	virtual int getInteraction(Player &, __int64 &, Vec3 const &);
	virtual void canDestroyBlock(Block const &);
	virtual void setAuxValue(int);
	virtual void setSize(float, float);
	virtual void onOrphan(void);
	virtual void wobble(void);
	virtual void wasHurt(void);
	virtual void startSpinAttack(void);
	virtual void stopSpinAttack(void);
	virtual void setDamageNearbyEntitys(bool);
	virtual void renderDebugServerState(__int64 const &);
	virtual void reloadLootTable(void);
	virtual void reloadLootTable(__int64 const &);
	virtual int getDeletionDelayTimeSeconds(void);
	virtual void kill(void);
	virtual void die(__int64 const &);
	virtual void shouldDropDeathLoot(void);
	virtual void shouldTick(void);
	virtual void applySnapshot(__int64 const &, __int64 const &);
	virtual float getNextStep(float);
	virtual int getLootTable(void);
	virtual void interpolatorTick(void);
	virtual void onPush(Entity &);
	virtual int getLastDeathPos(void);
	virtual int getLastDeathDimension(void);
	virtual void hasDiedBefore(void);
	virtual void doWaterSplashEffect(void);
	virtual void _shouldProvideFeedbackOnHandContainerItemSet(int, ItemStack const &);
	virtual void _shouldProvideFeedbackOnArmorSet(int, ItemStack const &);
	virtual void updateEntitySpecificMolangVariables(__int64 &);
	virtual void shouldTryMakeStepSound(void);
	virtual void canMakeStepSound(void);
	virtual void _hurt(__int64 const &, float, bool, bool);
	virtual void markHurt(void);
	virtual void _getAnimationComponent(__int64 &, __int64);
	virtual void readAdditionalSaveData(CompoundTag const &, __int64 &);
	virtual void addAdditionalSaveData(CompoundTag &);
	virtual void _playStepSound(Vec3i const &, Block const &);
	virtual void _playFlySound(Vec3i const &, Block const &);
	virtual void _makeFlySound(void);
	virtual void checkInsideBlocks(float);
	virtual void pushOutOfBlocks(Vec3 const &);
	virtual void spawnTrailBubbles(void);
	virtual void updateInsideBlock(void);
	virtual void _removePassenger(uint64_t const &, bool, bool, bool);
	virtual void _onSizeUpdated(void);
	virtual void _doAutoAttackOnTouch(Entity &);
	virtual void knockback(Entity *, int, float, float, float, float, float);
	virtual void spawnAnim(void);
	virtual void setSprinting(bool);
	virtual int getHurtSound(void);
	virtual int getDeathSound(void);
	virtual __int64 getSpeed(void);
	virtual void setSpeed(float);
	virtual int getSprintSpeedIncrease(void);
	virtual void hurtEffects(__int64 const &, float, bool, bool);
	virtual int getMeleeWeaponDamageBonus(Entity);
	virtual int getMeleeKnockbackBonus(void);
	virtual void travel(float, float, float);
	virtual void applyFinalFriction(float, bool);
	virtual void aiStep(void);
	virtual void aiStep(__int64 &);
	virtual void pushEntitys(void);
	virtual void lookAt(Entity *, float, float);
	virtual bool isLookingAtAnEntity(void);
	virtual void checkSpawnRules(bool);
	virtual void checkSpawnObstruction(void);
	virtual float getAttackAnim(float);
	virtual int getItemUseDuration(void);
	virtual int getItemUseStartupProgress(void);
	virtual int getItemUseIntervalProgress(void);
	virtual int getItemUseIntervalAxis(void);
	virtual int getTimeAlongSwing(void);
	virtual void ate(void);
	virtual int getMaxHeadXRot(void);
	virtual bool isAlliedTo(Entity *);
	virtual void doHurtTarget(Entity *, __int64 const &);
	virtual void canBeControlledByPassenger(void);
	virtual void leaveCaravan(void);
	virtual void joinCaravan(Entity);
	virtual void hasCaravanTail(void);
	virtual int getCaravanHead(void);
	virtual int getArmorValue(void);
	virtual int getArmorCoverPercentage(void);
	virtual int getToughnessValue(void);
	virtual void hurtints(__int64 const &, int, std::bitset<4ul>);
	virtual void setDamagedArmor(int, ItemStack const &);
	virtual void sendArmorDamage(std::bitset<4ul>);
	virtual void sendArmor(std::bitset<4ul>);
	virtual void containerChanged(int);
	virtual void updateEquipment(void);
	virtual void clearEquipment(void);
	virtual int getAllArmorID(void);
	virtual int getAllHand(void);
	virtual int getAllEquipment(void);
	virtual int getArmorTypeHash(void);
	virtual void dropEquipmentOnDeath(__int64 const &, int);
	virtual void dropEquipmentOnDeath(void);
	virtual void clearVanishEnchantedItemsOnDeath(void);
	virtual void sendInventory(bool);
	virtual float getDamageAfterEnchantReduction(__int64 const &, float);
	virtual float getDamageAfterArmorReduction(__int64 const &, float);
	virtual float getDamageAfterResistanceEffect(__int64 const &, float);
	virtual void createAIGoals(void);
	virtual void onBorn(Entity &, Entity &);
	virtual void setItemSlot(int, ItemStack const &);
	virtual void setTransitioningSitting(bool);
	virtual void attackAnimation(Entity *, float);
	virtual int getAttackTime(void);
	virtual void _getWalkTargetValue(Vec3i const &);
	virtual void canExistWhenDisallowEntity(void);
	virtual void useNewAi(void);
	virtual void ascendLadder(void);
	virtual void ascendBlockByJumping(void);
	virtual void descendBlockByCrouching(void);
	virtual void dropContainer(void);
	virtual void initBodyControl(void);
	virtual void jumpFromGround(void);
	virtual void jumpFromGround(__int64 &);
	virtual void updateAi(void);
	virtual void newServerAiStep(void);
	virtual void _serverAiEntityStep(void);
	virtual void dropBags(void);
	virtual void tickDeath(void);
	virtual void updateGliding(void);
	virtual void _allowAscendingScaffolding(void);
	virtual void _getAdjustedAABBForSpawnCheck(AABB const &, Vec3 const &);

public:
	InventoryTransactionManager *getTransactionManager();

	AABB *getAABB() {
		return this->aabb;
	}

	__int64 *getUniqueId() {
		using getUniqueId_t = __int64*(__thiscall *)(Entity*);
		static auto getUniqueIdFunc = reinterpret_cast<getUniqueId_t>(FindSignature("40 53 48 83 EC 20 48 8D 99 ? ? ? ? 48 83 3B"));
		return getUniqueIdFunc(this);
	}

	float getRealSpeed() {
		return *reinterpret_cast<float *>(this->getSpeed() + 0x84);
	}

	float getTicksPerSecond() {
		Vec3 targetPos = *this->getPos();
		Vec3 targetPosOld = *this->getPosOld();
		float hVel = sqrtf(((targetPos.x - targetPosOld.x) * (targetPos.x - targetPosOld.x)) + ((targetPos.z - targetPosOld.z) * (targetPos.z - targetPosOld.z)));
		return hVel;
	}

	float getBlocksPerSecond();

	int getTicksUsingItem() {
		return *reinterpret_cast<int *>(this + 0x1238);
	}

	bool isSneaking() {
		return getStatusFlag(SNEAKING);
	}

	bool isSprinting() {
		return getStatusFlag(SPRINTING);
	}

	Dimension *getDimension() {
		return this->dimension;
	}

	void setIsInWater(bool is) {
		*(bool *)((uintptr_t)(this) + 0x1FE) = is;
	}

	void SetFieldOfView(float num) {
		*(float *)((uintptr_t)(this) + 0x1458) = num;
	}

	float GetFieldOfView() {
		return *reinterpret_cast<float *>(this + 0x1458);
	}

	class Level *getLevel() {
		return this->level;
	}

	PlayerMovementProxy *getMovementProxy() {
		return getActorMovementProxyComponent()->movementProxy;
	}

	bool isOnGround() {
		return getMovementProxy()->isOnGround();
	}

	void setOnGround(bool onGround) {
		getMovementProxy()->setOnGround(onGround);
	}

	void lerpTo(Vec3 const &pos, Vec2 const &rot, int steps);

	ActorRotationComponent *getActorRotationComponent() {
		using getActorRotationComponent = ActorRotationComponent *(__cdecl *)(__int64 *, uint32_t *);
		static auto func = reinterpret_cast<getActorRotationComponent>(FindSignature("40 53 48 83 EC ? 48 8B DA BA CE 21 1E DC"));
		uint32_t id = this->entityId;
		return func(*this->entityRegistryBase, &id);
	}

	MobBodyRotationComponent *getMobBodyRotationComponent() {
		using getMobBodyRotationComponent = MobBodyRotationComponent *(__cdecl *)(__int64 *, uint32_t *);
		static auto func = reinterpret_cast<getMobBodyRotationComponent>(FindSignature("40 53 48 83 EC ? 48 8B DA BA 2F B8 31 03"));
		uint32_t id = this->entityId;
		return func(*this->entityRegistryBase, &id);
	}

	ActorHeadRotationComponent *getActorHeadRotationComponent() {
		using getActorHeadRotationComponent = ActorHeadRotationComponent *(__cdecl *)(__int64 *, uint32_t *);
		static auto func = reinterpret_cast<getActorHeadRotationComponent>(FindSignature("40 53 48 83 EC ? 48 8B DA BA 1C 58 40 E9"));
		uint32_t id = this->entityId;
		return func(*this->entityRegistryBase, &id);
	}

	MaxAutoStepComponent *getMaxAutoStepComponent() {
		using getMaxAutoStep = MaxAutoStepComponent *(__cdecl *)(__int64 *, uint32_t *);
		static auto func = reinterpret_cast<getMaxAutoStep>(FindSignature("40 53 48 83 EC ? 48 8B DA BA 16 72 6F 0E"));
		uint32_t id = this->entityId;
		return func(*this->entityRegistryBase, &id);
	}

	FallDistanceComponent *getFallDistanceComponent() {
		using getFallDistanceComponent = FallDistanceComponent *(__cdecl *)(__int64 *, uint32_t *);
		static auto func = reinterpret_cast<getFallDistanceComponent>(FindSignature("40 53 48 83 EC ? 48 8B DA BA B5 5C 12 81"));
		uint32_t id = this->entityId;
		return func(*this->entityRegistryBase, &id);
	}

	ActorMovementProxyComponent *getActorMovementProxyComponent() {
		using getActorMovementProxyComponent = ActorMovementProxyComponent *(__cdecl *)(__int64 *, uint32_t *);
		static auto func = reinterpret_cast<getActorMovementProxyComponent>(FindSignature("40 53 48 83 EC ? 48 8B DA BA 18 0C BD EC"));
		uint32_t id = this->entityId;
		return func(*this->entityRegistryBase, &id);
	}
};
#pragma pack(pop)

class MobEffect {
public:
	int id; // 0x8
	bool isHarmful; // 0xC

private:
	char pad_0xD[0x13]; // 0xD

public:
	TextHolder* internalName; // 0x20

private:
	char pad_0x28[0x28]; // 0x28

public:
	TextHolder name; // 0x50

private:
	char pad_0x70[0x30]; // 0x70

public:
	TextHolder* minecraftName; // 0xA0

private:
	char pad_0xA8[0xD0];  // 0xA8

private:
	virtual void destructor();

public:
	virtual void applyEffects(Entity *, int, int);
	virtual void removeEffects(Entity *);
	virtual void applyInstantaneousEffect(Entity *, Entity *, Entity *, int, float);
	virtual void parse(void *, __int64 const &, __int64 const &, int, std::string &);
	virtual int getAttributeModifierValue(int, __int64 const &);
};

class ServerPlayer;

class Player : public Entity {
public:
	PlayerInventoryProxy *getSupplies();

	int getDeviceID();

	const char* getDeviceName() {
		int deviceID = this->getDeviceID();

		switch (deviceID) {
		case -1:
			return "Unknown";
		case 1:
			return "Android";
		case 2:
			return "iOS";
		case 3:
			return "OSX";
		case 4:
			return "Amazon";
		case 5:
			return "GearVR";
		case 6:
			return "Hololens";
		case 7:
			return "Win10";
		case 8:
			return "WIN32";
		case 9:
			return "Dedicated";
		case 10:
			return "TVOS";
		case 11:
			return "PlayStation";
		case 12:
			return "Nintendo";
		case 13:
			return "Xbox";
		case 14:
			return "WindowsPhone";
		case 15:
			return "Linux";
		default:
			return "Unknown";
		}
	}

	ItemStack *getSelectedItem() {
		auto supplies = this->getSupplies();
		return supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
	}

	int getSelectedItemId() {
		auto item = getSelectedItem();
		if (item == nullptr || item->item == nullptr)
			return 0;
		if (item->getItem() == nullptr)
			return 0;
		return item->getItem()->itemId;
	}

	virtual void prepareRegion(__int64 &);
	virtual void destroyRegion(void);
	virtual void suspendRegion(void);
	virtual void resendAllChunks(void);
	virtual void _fireWillChangeDimension(void);
	virtual void _fireDimensionChanged(void);
	virtual void changeDimensionWithCredits(int);
	virtual void tickWorld(__int64 const &);
	virtual void frameUpdate(__int64 &);
	virtual int getTickingOffsets(void);
	virtual void moveView(void);
	virtual void moveSpawnView(Vec3 const &, int);
	virtual void setName(TextHolder const &);
	virtual int getTravelledMethod(void);
	virtual void checkMovementStats(Vec3 const &);
	virtual int getCurrentStructureFeature(void);
	virtual bool isAutoJumpEnabled(void);
	virtual void respawn(void);
	virtual void resetRot(void);
	virtual bool isInTrialMode(void);
	virtual void hasResource(int);
	virtual void completeUsingItem(void);
	virtual void startDestroying(void);
	virtual void stopDestroying(void);
	virtual void openPortfolio(void);
	virtual void openBook(int, bool, int, BlockActor *);
	virtual void openTrading(uint64_t const &, bool);
	virtual bool canOpenContainerScreen(void);
	virtual void openChalkboard(__int64 &, bool);
	virtual void openNpcInteractScreen(__int64);
	virtual void openInventory(void);
	virtual void displayChatMessage(TextHolder const &, TextHolder const &);
	virtual void displayClientMessage(TextHolder *);
	virtual void displayTextObjectMessage(__int64 const &, TextHolder const &, TextHolder const &);
	virtual void displayTextObjectWhisperMessage(__int64 const &, TextHolder const &, TextHolder const &);
	virtual void displayTextObjectWhisperMessage(TextHolder const &, TextHolder const &, TextHolder const &);
	virtual void displayWhisperMessage(TextHolder const &, TextHolder const &, TextHolder const &, TextHolder const &);
	virtual void startSleepInBed(Vec3i const &);
	virtual void stopSleepInBed(bool, bool);
	virtual void canStartSleepInBed(void);
	virtual int getSleepTimer(void);
	virtual int getPreviousTickSleepTimer(void);
	virtual void openSign(Vec3i const &);
	virtual void playEmote(TextHolder const &);
	virtual bool isHostingPlayer(void);
	virtual bool isLoading(void);
	virtual bool isPlayerInitialized(void);
	virtual void stopLoading(void);
	virtual void registerTrackedBoss(uint64_t);
	virtual void unRegisterTrackedBoss(uint64_t);
	virtual void setPlayerGameType(GameType);
	virtual void initHUDContainerManager(void);
	virtual void _crit(Entity &);
	virtual int getEventing(void);
	virtual int getUserId(void);
	virtual void sendEventPacket(__int64 &);
	virtual void addExperience(int);
	virtual void addLevels(int);
	virtual void setContainerData(__int64 &, int, int);
	virtual void slotChanged(__int64 &, __int64 &, int, ItemStack const &, ItemStack const &, bool);
	virtual void inventoryChanged(__int64 &, int, ItemStack const &, ItemStack const &, bool);
	virtual void refreshContainer(__int64 &);
	virtual void deleteContainerManager(void);
	virtual void setFieldOfViewModifier(float);
	virtual bool isEntityRelevant(Entity const &);
	virtual bool isTeacher(void);
	virtual void onSuspension(void);
	virtual void onLinkedSlotsChanged(void);
	virtual void startCooldown(Item const *, bool);
	virtual int getItemCooldownLeft(HashedString const &);
	virtual int getItemCooldownLeft(uint64_t);
	virtual int getMaxItemCooldownLeft(void);
	virtual bool isItemOnCooldown(HashedString const &);
	virtual void sendInventoryTransaction(InventoryTransaction const &);
	virtual void sendComplexInventoryTransaction(__int64);
	virtual void sendNetworkPacket(Packet &);
	virtual int getPlayerEventCoordinator(void);
	virtual void tryGetMoveInputHandler(void);
	virtual int getInputMode(void);
	virtual int getPlayMode(void);
	virtual void reportMovementTelemetry(__int64);
	virtual bool isSimulated(void);
	virtual int getXuid(void);
	virtual int getMovementSettings(void);
	virtual void addSavedChunk(__int64 const &);
	virtual void onMovePlayerPacketNormal(Vec3 const &, Vec2 const &, float);
	virtual void _createChunkSource(__int64 &);
	virtual void setAbilities(__int64 const &);
	virtual int getEditorPlayer(void);
	virtual void destroyEditorPlayer(void);
	virtual void _getSpawnChunkLimit(void);
	virtual void _updateChunkPublisherView(Vec3 const &, float);
};

class ServerPlayer : public Player {
};

class LocalPlayer : public Player {
public:
	void unlockAchievements();

	const char* getDeviceNameLocal() {
		int deviceID = this->deviceIdentifier;

		switch (deviceID) {
		case -1:
			return "Unknown";
		case 0:
			return "Win10";
		case 1:
			return "WindowsPhone";
		case 2:
			return "Xbox";
		default:
			return "Unknown";
		}
	}

	void swingArm() {
		// using SwingArm = void(__thiscall*)(void*);
		// static SwingArm swingFunc = reinterpret_cast<SwingArm>(FindSignature("40 57 48 83 EC ?? 48 C7 44 24 ?? FE FF FF FF 48 89 5C 24 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 48 8B D9 80 B9"));
		// swingFunc(this);
		this->swing();
	}
	void localPlayerTurn(Vec2 *viewAngles) {
		using Turn = void(__thiscall *)(void *, Vec2 *);
		static Turn TurnFunc = reinterpret_cast<Turn>(FindSignature("48 8B 58 68 48 ?? ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? 89 ?? ?? ?? ?? ?? ?? ?? 48 8B 03 48 8B CB FF 50 08 90 48 85 DB ?? 09 48 8B ?? 48 8B CB ?? ?? ?? ?? ?? ?? ?? 48 8B D3 48 8B CE E8 0D BB 1D FF 90 48 85 DB 74 09 48 8B 03 48 8B ?? ?? ?? ?? 48 83 C7 10"));
		TurnFunc(this, viewAngles);
	}
	GameMode *getGameMode() {
		static unsigned int offset = 0;
		if (offset == NULL)
			offset = *reinterpret_cast<int *>(FindSignature("48 8B BE ? ? ? ? 48 8B 8E ? ? ? ? 48 89 6C") + 3);
		return *reinterpret_cast<GameMode **>((uintptr_t)(this) + offset);
	}
	void applyTurnDelta(Vec2 *viewAngleDelta);
};