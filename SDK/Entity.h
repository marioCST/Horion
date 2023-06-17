#pragma once

#include <bitset>

#include "../Utils/HMath.h"
#include "../Utils/Utils.h"
#include "BlockLegacy.h"
#include "Dimension.h"
#include "EntityList.h"
#include "Inventory.h"
#include "InventoryTransaction.h"
#include "TextHolder.h"

class GameMode;

class Level {
private:
	char pad_0x0[0xBF0];  // 0x0
public:
	int rayHitType;  // 0xBF0
	int blockSide;   // 0xBF4
	Vec3i block;     // 0xBF8
	Vec3 rayHitVec;  // 0xC04
	Entity *entityPtr;
	Entity *entityPtr2;
	uint64_t GamingEntityFinder;

public:
	bool hasEntity();     // to not go trough the entity list twice
	Entity *getEntity();  // returns the entity that the player is looking at
	int getLevelTicks() {
		// return levelTicks;
		return *reinterpret_cast<int *>(reinterpret_cast<__int64>(this) + 0x8A0);
	}

	class LoopbackPacketSender *getLoopbackPacketSender() {
		return *reinterpret_cast<class LoopbackPacketSender **>(reinterpret_cast<__int64>(this) + 0xBD0);
	}

	void playSound(std::string sound, Vec3 const &position, float volume, float pitch) {
		using playSound_t = void(__fastcall *)(Level *, TextHolder *, Vec3 const &, float, float);
		static playSound_t func = reinterpret_cast<playSound_t>(Utils::getBase() + 0x2DD9860);  // 48 89 5C ? ? 48 89 6C ? ? 48 89 74 ? ? 57 48 83 EC ? 48 8B 41 ? 33 FF
		if (func != nullptr) func(this, &TextHolder(sound), position, volume, pitch);
	}

	// Credits to hacker hansen for this
	std::vector<Entity *> getMiscEntityList() { // 1.20 Vtable index: 304
		using entityList_t = std::int64_t *(__fastcall *)(Level *, void *);
		static entityList_t func = reinterpret_cast<entityList_t>(Utils::getBase() + 0x2E1CE60);  // 40 53 48 83 EC ? 48 81 C1 ? ? ? ? 48 8B DA E8 ? ? ? ? 48 8B C3 48 83 C4 ? 5B C3 CC CC 48 8B 81
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
struct MobEffectInstance;

#pragma pack(push, 4)
class Entity {
	uint64_t ptrToEntityList;  // 0x8
private:
	char pad_0x10[0x128];  // 0x10
public:
	union {
		struct {
			float pitch;  // 0x0138
			float yaw;    // 0x013C
		};
		Vec2 viewAngles;
	};
	union {
		struct {
			float pitch2;  // 0x0138
			float yaw2;    // 0x013C
		};
		Vec2 viewAngles2;
	};

private:
	char pad_0148[16];  // 0x0148
public:
	Vec3 eyePos0;  // 0x0158
private:
	char pad_0164[112];  // 0x0164
public:
	float fallDistance;  // 0x01D4
	bool onGround;       // 0x01D8
private:
	char pad_01D9[95];  // 0x01D9
public:
	float stepHeight;  // 0x0238
private:
	char pad_023C[16];  // 0x023C
public:
	Vec3 slowdownFactor;  // 0x024C
private:
	char pad_0258[5];  // 0x0258
public:
	bool didEnterWaterBool;  // 0x025D
private:
	char pad_025E[1];  // 0x025E
public:
	bool hasEnteredWaterBool;  // 0x025F
private:
	char pad_0260[72];  // 0x0260
public:
	//int32_t ticksAlive;  // 0x02A8
	char pad_2A8[0x4];

private:
	char pad_02AC[172];  // 0x02AC
public:
	//class BlockSource *region;  // 0x0358
	char pad_0x358[8];

private:
	char pad_0360[8];  // 0x0360
public:
	//Level *level;  // 0x0368
	char pad_368[8];

private:
	char pad_0370[328];  // 0x0370
public:
	//AABB aabb;        // 0x04B8
	char pad_4B8[8];
	float width;      // 0x04D0
	float height;     // 0x04D4
	Vec3 currentPos;  // 0x04D8
	Vec3 oldPos;      // 0x04E4
	Vec3 velocity;    // 0x04F0
	Vec3 velocity2;   // 0x04FC
private:
	char pad_0508[80];  // 0x0508
public:
	int64_t entityRuntimeId;  // 0x0558
private:
	char pad_0560[0x1C8];  // 0x0560
public:
	float bodyYaw;                    // 0x0728
	float oldBodyYaw;                 // 0xtoolazytoupdatethesecommentsxd
	float yawUnused1;                 // 0x0748
	float yawUnused2;                 // 0x074C
	int32_t damageTime;               // 0x0750
	int32_t damageAnimationDuration;  // 0x0754
private:
	char pad_0758[136];  // 0x0758
public:
	int32_t timeSinceDeath;  // 0x07E0
private:
	char pad_07E4[244];  // 0x07E4
public:
	class TextHolder playerName;  // 0x08D8
private:
	char pad_08F8[140];  // 0x08F8
public:
	bool canFly;  // 0x0984
private:
	char pad_0985[1667];  // 0x0985
public:
	int32_t ticksUsingItem;  // 0x1008
private:
	char pad_100C[20];  // 0x100C
public:
	int16_t itemData;  // 0x1020
	int16_t itemId;    // 0x1022
private:
	char pad_1024[508];  // 0x1024
public:
	class InventoryTransactionManager transac;  // 0x1220
private:
	char pad_1280[2828];  // 0x1280
public:
	int gamemode;  // 0x1D7C

	BUILD_ACCESS(this, __int64*, entityContext, 0x8);
	BUILD_ACCESS(this, uint32_t, entityIdentifier, 0x10);
	BUILD_ACCESS(this, int32_t, ticksAlive, 0x200);
	BUILD_ACCESS(this, Level *, level, 0x260);
	BUILD_ACCESS(this, AABB *, aabb, 0x2A8);
	BUILD_ACCESS(this, Vec3, eyePosPrev, 0x814);
	BUILD_ACCESS(this, Vec3, eyePos, 0x824);
	BUILD_ACCESS(this, BlockSource *, region, 0xB98);
	BUILD_ACCESS(this, bool, whatIsThis, 0xBA0);

	virtual int getStatusFlag(__int64);
	virtual void setStatusFlag(__int64, bool);
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
	virtual void hasComponent(HashedString const &);
	virtual void Destructor();
	//virtual void ~Player();
	virtual void resetUserPos(bool);
	virtual int getOwnerEntityType(void);
	virtual void remove(void);
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
	virtual int getExitTip(std::string const &, __int64, __int64);
	virtual int getEntityLocNameString(void);
	virtual void intersects(Vec3 const &, Vec3 const &);
	virtual bool isInWall(void);
	virtual bool isInvisible(void);
	virtual bool canShowNameTag(void);
	virtual void canExistInPeaceful(void);
	virtual void setNameTagVisible(bool);
	virtual TextHolder* getNameTag(void);
	virtual int getNameTagAsHash(void);
	virtual TextHolder* getFormattedNameTag(void);
	virtual void filterFormattedNameTag(__int64 const &);
	virtual void setNameTag(std::string const &);
	virtual int getAlwaysShowNameTag(void);
	virtual void setScoreTag(std::string const &);
	virtual int getScoreTag(void);
	virtual bool isInWater(void);
	virtual bool isUnderLiquid(__int64);
	virtual bool isOverWater(void);
	virtual void setBlockMovementSlowdownMultiplier(BlockLegacy const &, Vec3 const &);
	virtual void resetBlockMovementSlowdownMultiplier(void);
	virtual int getCameraOffset(void);
	virtual int getShadowHeightOffs(void);
	virtual int getShadowRadius(void);
	virtual float getHeadLookVector(float);
	virtual void canSeeInvisible(void);
	virtual void canSee(Entity const &);
	virtual void canSee(Vec3 const &);
	virtual void canInteractWithOtherEntitiesInGame(void);
	virtual bool isSkyLit(float);
	virtual float getBrightness(float, __int64 const &);
	virtual void interactPreventDefault(void);
	virtual void playerTouch(Player &);
	virtual void onAboveBubbleColumn(bool);
	virtual void onInsideBubbleColumn(bool);
	virtual bool isImEntityile(void);
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
	virtual void spawnAtLocation(ItemStack const &, float);
	virtual void despawn(void);
	virtual void killed(Entity &);
	virtual void awardKillScore(Entity &, int);
	virtual void setArmor(int, ItemStack const &);
	virtual ItemStack *getArmor(int);
	virtual int getAllArmor(void);
	virtual int getArmorMaterialTypeInSlot(int);
	virtual int getArmorMaterialTextureTypeInSlot(int);
	virtual int getArmorColorInSlot(int, int);
	virtual int getEquippedSlot(int);
	virtual void setEquippedSlot(int, ItemStack const &);
	virtual void setCarriedItem(ItemStack const *);
	virtual int getCarriedItem(void);
	virtual void setOffhandSlot(ItemStack const *);
	virtual ItemStack* getEquippedTotem(void);
	virtual void consumeTotem(void);
	virtual void save(CompoundTag*);
	virtual void saveWithoutId(CompoundTag &);
	virtual void load(CompoundTag const &, __int64 &);
	virtual void loadLinks(CompoundTag const &, std::vector<__int64> &, __int64 &);
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
	virtual void changeDimension(Dimension*, int);
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
	virtual void buildDebugInfo(std::string &);
	virtual int getCommandPermissionLevel(void);
	virtual bool isClientSide(void);
	virtual int getMutableAttribute(class Attribute const &);
	virtual int getAttribute(Attribute const &);
	virtual int getDeathTime(void);
	virtual void heal(int);
	virtual bool isInvertedHealAndHarm(void);
	virtual void canBeAffected(int);
	virtual void canBeAffectedByArrow(MobEffectInstance const &);
	virtual void onEffectAdded(MobEffectInstance &);
	virtual void onEffectUpdated(MobEffectInstance &);
	virtual void onEffectRemoved(MobEffectInstance &);
	virtual void canObstructSpawningAndBlockPlacement(void);
	virtual int getAnimationComponent(void);
	virtual void openContainerComponent(Player &);
	virtual void swing(void);
	virtual void useItem(ItemStack &, __int64, bool);
	virtual void hasOutputSignal(char);
	virtual int getOutputSignal(void);
	virtual int getDebugText(std::vector<std::string> &);
	virtual int getMapDecorationRotation(void);
	virtual int getPassengerYRotation(Entity const &);
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
	virtual void hurtEffects(__int64 const &, float, bool, bool);
	virtual int getMeleeWeaponDamageBonus(Entity);
	virtual int getMeleeKnockbackBonus(void);
	virtual void travel(float, float, float, bool);
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
	virtual bool isAlliedTo(Entity*);
	virtual void doHurtTarget(Entity *, __int64 const &);
	virtual void canBeControlledByPassenger(void);
	virtual void leaveCaravan(void);
	virtual void joinCaravan(Entity);
	virtual void hasCaravanTail(void);
	virtual int getCaravanHead(void);
	virtual int getArmorValue(void);
	virtual int getArmorCoverPercentage(void);
	virtual int getToughnessValue(void);
	virtual void hurtArmorSlots(__int64 const &, int, std::bitset<4ul>);
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
	virtual void ascendLadder(void);
	virtual void ascendBlockByJumping(void);
	virtual void descendBlockByCrouching(void);
	virtual void dropContainer(void);
	virtual void initBodyControl(void);
	virtual void jumpFromGround(); //these may be wrong but idk
	virtual void jumpFromGround(__int64 &, __int64 const &);
	virtual void newServerAiStep(void);
	virtual void _serverAiEntityStep(void);
	virtual void dropBags(void);
	virtual void tickDeath(void);
	virtual void updateGliding(void);
	virtual void _allowAscendingScaffolding(void);
	virtual void _getAdjustedAABBForSpawnCheck(AABB const &, Vec3 const &);
	virtual void prepareRegion(__int64 &);
	virtual void destroyRegion(void);
	virtual void suspendRegion(void);
	virtual void resendAllChunks(void);
	virtual void _fireWillChangeDimension(void);
	virtual void _fireDimensionChanged(void);
	virtual void changeDimensionWithCredits(Dimension, int);
	virtual void tickWorld(__int64 const &);

private:
	virtual void TryroFunc354();

public:
	virtual int getTickingOffsets(void);
	virtual void moveView(void);
	virtual void moveSpawnView(Vec3 const &, Dimension, int);
	virtual void setName(std::string const &);
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
	virtual void openNpcInteractScreen(std::shared_ptr<__int64>);
	virtual void openInventory(void);
	virtual void displayChatMessage(std::string const &, std::string const &);
	virtual void displayClientMessage(TextHolder *);
	virtual void displayTextObjectMessage(__int64 const &, std::string const &, std::string const &);
	virtual void displayTextObjectWhisperMessage(__int64 const &, std::string const &, std::string const &);
	virtual void displayTextObjectWhisperMessage(std::string const &, std::string const &, std::string const &);
	virtual void displayWhisperMessage(std::string const &, std::string const &, std::string const &, std::string const &);
	virtual void startSleepInBed(Vec3i const &);
	virtual void stopSleepInBed(bool, bool);
	virtual void canStartSleepInBed(void);
	virtual int getSleepTimer(void);
	virtual int getPreviousTickSleepTimer(void);
	virtual void openSign(Vec3i const &, bool);
	virtual void playEmote(std::string const &, bool);
	virtual bool isHostingPlayer(void);
	virtual bool isLoading(void);
	virtual bool isPlayerInitialized(void);
	virtual void stopLoading(void);
	virtual void registerTrackedBoss(uint64_t);
	virtual void unRegisterTrackedBoss(uint64_t);
	virtual void setPlayerGameType(int);
	virtual void initHUDContainerManager(void);
	virtual void _crit(Entity &);
	virtual int getEventing(void);
	virtual int getUserId(void);
	virtual void sendEventPacket(__int64 &);
	virtual void addExperience(int);
	virtual void addLevels(int);

private:
	virtual void TryroFunc404();
	virtual void TryroFunc405();

public:
	virtual void inventoryChanged(Container &, int, ItemStack const &, ItemStack const &, bool);

private:
	virtual void TryroFunc407();

public:
	virtual void deleteContainerManager(void);
	virtual bool isEntityRelevant(Entity const &);

private:
	virtual void TryroFunc410();

public:
	virtual void onSuspension(void);
	virtual void onLinkedSlotsChanged(void);
	virtual void startCooldown(Item const *, bool);
	virtual int getItemCooldownLeft(HashedString const &);
	virtual int getItemCooldownLeft(long);
	virtual int getMaxItemCooldownLeft(void);
	virtual bool isItemOnCooldown(HashedString const &);

private:
	virtual void TryroFunc418();
	virtual void TryroFunc419();

public:
	virtual void sendNetworkPacket(class Packet &);

private:
	virtual void TryroFunc421();

public:
	virtual void reportMovementTelemetry(__int64);
	virtual bool isSimulated(void);
	virtual int getXuid(void);
	virtual int getMovementSettings(void);
	virtual void addSavedChunk(__int64 const &);
	virtual int getMaxChunkBuildRadius(void);
	virtual void onMovePlayerPacketNormal(Vec3 const &, Vec2 const &, float);
	virtual void _createChunkSource(__int64 &);
	virtual void setAbilities(__int64 const &);

public:
	InventoryTransactionManager *getTransactionManager();

	AABB *getAABB() {
		return this->aabb;
	}

	__int64 *getUniqueId() {
		__int64 *v1;  // rbx
		char v3;      // [rsp+30h] [rbp+8h]

		v1 = (__int64 *)((char *)this + 256);
		if (*((__int64 *)this + 32) == -1i64)
			*v1 = *(__int64 *)(*(__int64(__fastcall **)(__int64, char *))(**((__int64 **)this + 110) + 1960i64))(
				*((__int64 *)this + 110),
				&v3);
		return (__int64 *)v1;
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
		return *reinterpret_cast<int *>(this + 0xFF8);
	}

	bool isSneaking() {
		return false;  // TODO
	}

	bool isSprinting() {
		return false;  // TODO
	}

	Dimension *getDimension() {
		return reinterpret_cast<class Dimension *>(reinterpret_cast<__int64>(this) + 0x250);
	}

	void SetFieldOfView(float num) {
		*(float *)((uintptr_t)(this) + 0x1040) = num;
	}

	float GetFieldOfView() {
		return *reinterpret_cast<float *>(this + 0x1040);
	}

	class Level *getlevel() {
		return *reinterpret_cast<class Level **>(reinterpret_cast<__int64>(this) + 0x260);
	}

	void lerpTo(Vec3 const &pos, Vec2 const &a2, int a3);

	void setPos(Vec3 const& pos) {
		using setPos = void(__thiscall *)(Entity*, Vec3*);
		static setPos setPosFunc = reinterpret_cast<setPos>(FindSignature("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B DA 48 8B F9 48 8B CB 48 8D 15"));
		setPosFunc(this, &Vec3(pos));
	}

	void setRot(Vec2 const& rot) {
		/*using setRot = void(__thiscall *)(Entity *, Vec2 *);
		static setRot setRotFunc = reinterpret_cast<setRot>(FindSignature("48 83 EC ? 48 8B 41 ? 48 89 54 ? ? 48 85 C0"));
		setRotFunc(this, &Vec2(rot));*/

		float *ptr = reinterpret_cast<float *>(this + 0x2B0);

		*ptr = rot.x;
		*(ptr + 0x4) = rot.y;
	}

	void setRotPrev(Vec2 const &rot) {
		float *ptr = reinterpret_cast<float *>(this + 0x2B0);

		*(ptr + 0x8) = rot.x;
		*(ptr + 0xC) = rot.y;
	}

	Vec2 getRot() {
		using getRot = Vec2(__thiscall *)(Entity *);
		static getRot getRotFunc = reinterpret_cast<getRot>(FindSignature("48 83 EC 28 48 8B 81 B0 02 00 00 48 85 C0 74 05"));
		return getRotFunc(this);
	}

	Vec2 getRotPrev() {
		float *ptr = reinterpret_cast<float *>(this + 0x2B0);

		Vec2 vec;

		vec.x = *(ptr + 0x8);
		vec.y = *(ptr + 0xC);

		return vec;
	}

	Vec2 getAABBDim() {
		using getAABBDim = Vec2(__thiscall *)(Entity *);
		static getAABBDim getAABBDimFunc = reinterpret_cast<getAABBDim>(FindSignature("48 83 EC 28 48 8B 81 ? ? ? ? 48 85 C0 74 09 48 83 C0 18"));
		return getAABBDimFunc(this);
	}

	bool isOnGround() {
		using ActorCollision_isOnGround = bool(__thiscall *)(__int64*);
		static ActorCollision_isOnGround ActorCollision_isOnGroundFunc = reinterpret_cast<ActorCollision_isOnGround>(FindSignature("40 53 48 83 EC ? 48 8B 01 48 8B D9 BA E1 2D 1F 21"));
		return ActorCollision_isOnGroundFunc(this->entityContext);
	}

	int64_t getRuntimeId() {
		uint32_t id = this->entityIdentifier;

		using getRuntimeId = bool(__thiscall *)(__int64 *, uint32_t*);
		static getRuntimeId getRuntimeIdFunc = reinterpret_cast<getRuntimeId>(FindSignature("40 53 48 83 EC ? 48 8B DA BA 14 14 A1 3C"));
		return getRuntimeIdFunc(this->entityContext, &id);
	}

	float getYHeadYaw() {
		using getYHeadYaw = float(__thiscall *)(Entity*);
		static getYHeadYaw getYHeadYawFunc = reinterpret_cast<getYHeadYaw>(FindSignature("48 83 EC 28 8B 41 10 48 8D 54 24 ? 48 8B 49 08 89 44 24 30 48 8B 09 E8 ? ? ? ? 48 85 C0 74 09 F3 0F 10 00 48 83 C4 28 C3 0F 57 C0"));
		return getYHeadYawFunc(this);
	}

	float getYHeadRotationsNewOld() {
		using getYHeadRotationsNewOld = float(__thiscall *)(Entity *);
		static getYHeadRotationsNewOld getYHeadRotationsNewOldFunc = reinterpret_cast<getYHeadRotationsNewOld>(FindSignature("40 53 48 83 EC 20 48 8B 41 08 4C 8B C1"));
		return getYHeadRotationsNewOldFunc(this);
	}
};
#pragma pack(pop)

class ServerPlayer;

class Player : public Entity {
public:
	PlayerInventoryProxy *getSupplies();

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
};

class ServerPlayer : public Player {
};

class LocalPlayer : public Player {
public:
	void unlockAchievements();

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
			offset = *reinterpret_cast<int *>(FindSignature("48 8B 86 ? ? ? ? 48 89 44 24 ? 88 54 24") + 3);
		return *reinterpret_cast<GameMode **>((uintptr_t)(this) + offset);
	}
	void applyTurnDelta(Vec2 *viewAngleDelta);
	void setGameModeType(int gma);
};