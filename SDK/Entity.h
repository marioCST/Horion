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
	char firstpad[0x688];  // 0x8
public:
	int levelTicks;  // 0x690
private:
	char secondpad[0x41C];  // 0x694
public:
	int rayHitType;  // 0xAB0
	int blockSide;   // 0xAB4
	Vec3i block;     // 0xAB8
	Vec3 rayHitVec;  // 0xAC4
private:
	Entity *entityPtr;            // 0xAD0
	Entity *entityPtr2;           // 0xAD8
	uint64_t GamingEntityFinder;  // 0xAE0

	virtual void DONTREMOVEPLS();

public:
	bool hasEntity();     // to not go trough the entity list twice
	Entity *getEntity();  // returns the entity that the player is looking at
	int getLevelTicks() {
		// return levelTicks;
		return *reinterpret_cast<int *>(reinterpret_cast<__int64>(this) + 0x690);
	}

	class LoopbackPacketSender *getLoopbackPacketSender() {
		return *reinterpret_cast<class LoopbackPacketSender **>(reinterpret_cast<__int64>(this) + 0xA90);
	}

	void playSound(std::string sound, Vec3 const &position, float volume, float pitch) {
		using playSound_t = void(__fastcall *)(Level *, TextHolder *, Vec3 const &, float, float);
		static playSound_t func = reinterpret_cast<playSound_t>(Utils::getBase() + 0x2DD9860);  // 48 89 5C ? ? 48 89 6C ? ? 48 89 74 ? ? 57 48 83 EC ? 48 8B 41 ? 33 FF
		if (func != nullptr) func(this, &TextHolder(sound), position, volume, pitch);
	}

	// Credits to hacker hansen for this
	std::vector<Entity *> getMiscEntityList() {
		using entityList_t = std::int64_t *(__fastcall *)(Level *, void *);
		static entityList_t func = reinterpret_cast<entityList_t>(Utils::getBase() + 0x23E78C0);  // 48 89 5C 24 ? 56 57 41 56 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B F2 4C 8B F1 48 89 54 24 ? 33 C9
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
	int32_t ticksAlive;  // 0x02A8
private:
	char pad_02AC[172];  // 0x02AC
public:
	class BlockSource *region;  // 0x0358
private:
	char pad_0360[8];  // 0x0360
public:
	Level *level;  // 0x0368
private:
	char pad_0370[328];  // 0x0370
public:
	AABB aabb;        // 0x04B8
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

	virtual bool hasComponent();																// 0
	virtual __int64 getLastHurtByMob(void);														// 1
	virtual __int64 setLastHurtByMob(Entity *);													// 2
	virtual __int64 getLastHurtByPlayer(void);													// 3
	virtual __int64 setLastHurtByPlayer(Player *);												// 4
	virtual __int64 getLastHurtMob(void);														// 5
	virtual __int64 setLastHurtMob(Entity *);													// 6
	virtual __int64 outOfWorld(void);															// 7
	virtual __int64 reloadHardcoded();															// 8
	virtual __int64 reloadHardcodedClient();													// 9
	virtual __int64 initializeComponents();														// 10
	virtual __int64 reloadComponents();															// 11
	virtual __int64 _serverInitItemStackIds(void);												// 12
	virtual __int64 _doInitialMove(void);														// 13
	virtual void destructor();																	// 14
	virtual __int64 reset(void);																// 15
	virtual __int64 getOwnerEntityType(void);													// 16
	virtual __int64 remove(void);																// 17
	virtual __int64 setPos(Vec3 const &);														// 18
	virtual bool isRuntimePredictedMovementEnabled(void);										// 19
	virtual __int64 getPredictedMovementValues(void);											// 20
	virtual Vec3 *getPos(void);																	// 21
	virtual Vec3 *getPosOld(void);																// 22
	virtual Vec3 *getPosExtrapolated(float);													// 23
	virtual Vec3 *getAttachPos(__int64, float);													// 24
	virtual Vec3 *getFiringPos(void);															// 25
	virtual __int64 setRot(Vec2 const &);														// 26
	virtual __int64 move(Vec3 const &);															// 27
	virtual __int64 move(__int64, Vec3 const &);												// 28
	virtual __int64 getInterpolatedRidingPosition(float);										// 29
	virtual __int64 getInterpolatedBodyRot(float);												// 30
	virtual __int64 getShadowHeightOffs1(void);													// 31
	virtual __int64 getShadowHeightOffs2(void);													// 32
	virtual __int64 getYawSpeedInDegreesPerSecond(void);										// 33
	virtual __int64 getInterpolatedWalkAnimSpeed(float);										// 34
	virtual __int64 getWorldPosition(float);													// 35
	virtual __int64 resetInterpolated(void);													// 36
	virtual __int64 updateEntityInside(void);													// 37
	virtual __int64 updateEntityInside(AABB const &);											// 38
	virtual bool isFireImmune(void);															// 39
	virtual __int64 breaksFallingBlocks(void);													// 40
	virtual __int64 blockedByShield(__int64 const &, Entity *);									// 41
	virtual __int64 canDisableShield(void);														// 42
	virtual __int64 teleportTo(Vec3 const &, bool, int, int);									// 43
	virtual __int64 tryTeleportTo(Vec3 const &, bool, bool, int, int);							// 44
	virtual __int64 chorusFruitTeleport(Vec3 const &);											// 45
	virtual __int64 lerpMotion(Vec3 const &);													// 46
	virtual __int64 tryCreateAddActorPacket(void);												// 47
	virtual __int64 normalTick(void);															// 48
	virtual __int64 baseTick(void);																// 49
	virtual __int64 passengerTick(void);														// 50
	virtual __int64 positionPassenger(Entity *, float);											// 51
	virtual __int64 startRiding(Entity *);														// 53
	virtual __int64 addPassenger(Entity *);														// 54
	virtual __int64 flagPassengerToRemove(Entity *);											// 55
	virtual __int64 getExitTip(TextHolder *, int);												// 56
	virtual __int64 intersects(Vec3 const &, Vec3 const &);										// 57
	virtual bool isInWall(void);																// 58
	virtual bool isInvisible(void);																// 59
	virtual bool canShowNameTag(void);															// 60
	virtual bool canExistInPeaceful(void);														// 61
	virtual __int64 setNameTagVisible(bool);													// 62
	virtual TextHolder *getNameTag(void);														// 63
	virtual __int64 getGameEventDispatcher(void);												// 64
	virtual TextHolder *getFormattedNameTag(void);												// 65
	virtual __int64 filterFormattedNameTag(__int64);											// 66
	virtual __int64 setNameTag(TextHolder *);													// 67
	virtual __int64 getAlwaysShowNameTag(void);													// 68
	virtual __int64 setScoreTag(TextHolder *);													// 69
	virtual __int64 getScoreTag(void);															// 70
	virtual bool isInWater(void);																// 71
	virtual bool hasEnteredWater(void);															// 72
	virtual bool isInLava(void);																// 73
	virtual bool isUnderLiquid(__int64);														// 74
	virtual bool isOverWater(void);																// 75
	virtual __int64 setBlockMovementSlowdownMultiplier(Vec3 const &);							// 76
	virtual __int64 resetBlockMovementSlowdownMultiplier(void);									// 77
	virtual __int64 getShadowHeightOffs3(void);													// 78
	virtual __int64 getShadowHeightOffs4(void);													// 79
	virtual __int64 getShadowRadius(void);														// 80
	virtual __int64 getHeadLookVector(float);													// 81
	virtual bool canSeeInvisible(void);															// 82
	virtual bool canSee(Vec3 const &);															// 83
	virtual bool canSee(Entity const &);														// 84
	virtual bool cahReceiveMobEffectsForGameplay(void);											// 85
	virtual bool isSkyLit(float);																// 86
	virtual __int64 getBrightness(float);														// 87
	virtual __int64 interactPreventDefault(void);												// 88
	virtual __int64 PlayerTouch(Player &);														// 89
	virtual __int64 onAboveBubbleColumn(bool);													// 90
	virtual __int64 onInsideBubbleColumn(bool);													// 91
	virtual bool isImmobile(void);																// 92
	virtual bool isSilent(void);																// 93
	virtual bool isPickable(void);																// 94
	virtual bool isFishable(void);																// 95
	virtual bool isSleeping(void);																// 96
	virtual __int64 setSleeping(bool);															// 97
	virtual bool isShootable(void);																// 98
	virtual __int64 setSneaking(bool);															// 99
	virtual bool isBlocking(void);																// 100
	virtual bool isDamageBlocked(__int64 const &);												// 101
	virtual bool isAlive(void);																	// 102
	virtual bool isOnFire(void);																// 103
	virtual bool isReadyForInGameCommands(void);												// 104
	virtual bool isCreativeModeAllowed(void);													// 105
	virtual bool isSurfaceMob(void);															// 106
	virtual bool isReferenceType(void);															// 107
	virtual bool getReturnPolicy(void);															// 108
	virtual bool isPlayer(void);																// 109
	virtual bool isReferenceType2(void);														// 110
	virtual bool isAffectedByWaterBottle(void);													// 111
	virtual bool canAttack(Entity *, bool);														// 112
	virtual __int64 setTarget(Entity *);														// 113
	virtual __int64 findAttackTarget(void);														// 114
	virtual __int64 attack(Entity *, __int64 const &);											// 115
	virtual __int64 performRangedAttack(Entity *, float);										// 116
	virtual __int64 getEquipmentCount(void);													// 117
	virtual __int64 setOwner(__int64);															// 118
	virtual __int64 setSitting(bool);															// 119
	virtual __int64 onTame(void);																// 120
	virtual __int64 onFailedTame(void);															// 121
	virtual __int64 getInventorySize(void);														// 122
	virtual __int64 getEquipSlots(void);														// 123
	virtual __int64 getChestSlots(void);														// 124
	virtual __int64 setStanding(bool);															// 125
	virtual bool canPowerJump(void);															// 126
	virtual __int64 setCanPowerJump(bool);														// 127
	virtual bool isJumping(void);																// 128
	virtual bool isEnchanted(void);																// 129
	virtual __int64 vehicleLanded(Vec3 const &, Vec3 const &);									// 130
	virtual __int64 shouldRender(void);															// 131
	virtual __int64 playAmbientSound(void);														// 132
	virtual __int64 getAmbientSound(void);														// 133
	virtual bool isInvulnerableTo(__int64 const &);												// 134
	virtual __int64 getBlockDamageCause(Block const &);											// 135
	virtual __int64 actuallyHurt(int, __int64 const &, bool);									// 136
	virtual __int64 doFireHurt(int);															// 138
	virtual __int64 onLightningHit(void);														// 139
	virtual __int64 onBounceStarted(Vec3i const &, Block const &);								// 140
	virtual __int64 feed(int);																	// 141
	virtual __int64 handleEntityEvent(__int64, int);											// 142
	virtual __int64 getPickRadius(void);														// 143
	virtual __int64 getActorRendererId(void);													// 144
	virtual __int64 spawnAtLocation(ItemStack const &, float);									// 145
	virtual __int64 spawnAtLocation(Block const &, int, float);									// 146
	virtual __int64 spawnAtLocation(Block const &, int);										// 147
	virtual __int64 spawnAtLocation(int, int, float);											// 148
	virtual __int64 spawnAtLocation(int, int);													// 149
	virtual __int64 despawn(void);																// 150
	virtual __int64 killed(Entity *);															// 151
	virtual __int64 awardKillScore(Entity *, int);												// 152
	virtual __int64 setArmor(int, ItemStack const &);											// 153
	virtual ItemStack *getArmor(int);															// 154
	virtual __int64 getAllArmor(void);															// 155
	virtual __int64 getEnchantSlot1(int);														// 156
	virtual __int64 getEnchantSlot2(int);														// 157
	virtual __int64 getModelScale(void);														// 158
	virtual __int64 getEquippedSlot(int);														// 159
	virtual __int64 setEquippedSlot(int, ItemStack const &);									// 160
	virtual __int64 getCarriedItem(void);														// 161
	virtual __int64 setCarriedItem(ItemStack const *);											// 162
	virtual __int64 setOffhandSlot(ItemStack const *);											// 163
	virtual ItemStack *getEquippedTotem(void);													// 164
	virtual __int64 consumeTotem(void);															// 165
	virtual __int64 save(CompoundTag *);														// 166
	virtual __int64 saveWithoutId(CompoundTag &);												// 167
	virtual __int64 load(CompoundTag const &, __int64);											// 168
	virtual __int64 loadLinks(CompoundTag, __int64, __int64);									// 169
	virtual int getEntityTypeId(void);															// 170
	virtual __int64 getRawName(void);															// 171
	virtual __int64 getSourceUniqueID(void);													// 172
	virtual __int64 thawFreezeEffect(void);														// 173
	virtual bool canFreeze(void);																// 174
	virtual bool isWearingLeatherArmor(void);													// 175
	virtual __int64 getLiquidAABB(__int64);														// 176
	virtual __int64 handleInsidePortal(Vec3i const &);											// 177
	virtual __int64 getPortalCooldown(void);													// 178
	virtual __int64 getPortalWaitTime(void);													// 179
	virtual int getDimensionId(int *);															// 180
	virtual bool canChangeDimensions(void);														// 181
	virtual __int64 changeDimension(int, bool);													// 182
	virtual __int64 changeDimension2(__int64 const &);											// 183
	virtual __int64 getSourceUniqueID2(void);													// 184
	virtual __int64 checkFallDamage(float, bool);												// 185
	virtual __int64 causeFallDamage(float, float, __int64);										// 186
	virtual __int64 handleFallDistanceOnServer(float, float, bool);								// 187
	virtual __int64 playSynchronizedSound(__int64, Vec3 const &, int, bool);					// 188
	virtual __int64 playSynchronizedSound2(__int64, Vec3 const &, Block const &, bool);			// 189
	virtual __int64 onSynchedFlagUpdate(int, long, long);										// 190
	virtual __int64 onSynchedDataUpdate(int);													// 191
	virtual bool canAddPassenger(Entity *);														// 192
	virtual bool canPickupItem(ItemStack const &);												// 193
	virtual bool canBePulledIntoVehicle(void);													// 194
	virtual __int64 inCaravan(void);															// 195
	virtual bool isLeashableType(void);															// 196
	virtual __int64 tickLeash(void);															// 197
	virtual __int64 sendMotionPacketIfNeeded(void);												// 198
	virtual bool canSynchronizeNewEntity(void);													// 199
	virtual __int64 stopRiding(bool, bool, bool);												// 200
	virtual __int64 startSwimming(void);														// 201
	virtual __int64 stopSwimming(void);															// 202
	virtual __int64 buildDebugInfo(std::string &);												// 203
	virtual __int64 getCommandPermissionLevel(void);											// 204
	virtual bool isClientSide(void);															// 205
	virtual class AttributeInstance *getMutableAttribute(class Attribute *Attribute);			// 206
	virtual class AttributeInstance *getAttribute(class Attribute *Attribute);					// 207
	virtual __int64 getDeathTime(void);															// 208
	virtual __int64 heal(int);																	// 209
	virtual bool isInvertedHealAndHarm(void);													// 210
	virtual bool canBeAffected(MobEffectInstance const &);										// 211
	virtual bool canBeAffected(int);															// 212
	virtual bool canBeAffectedByArrow(MobEffectInstance const &);								// 213
	virtual __int64 onEffectAdded(MobEffectInstance &);											// 214
	virtual __int64 onEffectUpdated(MobEffectInstance &);										// 215
	virtual __int64 onEffectRemoved(MobEffectInstance &);										// 216
	virtual __int64 onObstructSpawningAndBlockPlacement(void);									// 217
	virtual __int64 openContainerComponent(Player &);											// 218
	virtual __int64 swing(void);																// 219
	virtual __int64 useItem(ItemStack &, int, bool);											// 220
	virtual bool hasOutputSignal(unsigned char);												// 221
	virtual __int64 getOutputSignal(void);														// 222
	virtual __int64 getDebugText(__int64);														// 223
	virtual __int64 getMapDecorationRotation(void);												// 224
	virtual __int64 getPassengerYRotation(Entity *);											// 225
	virtual __int64 getYHeadRot(void);															// 226
	virtual __int64 setYHeadRot(float);															// 227
	virtual __int64 getInterpolatedBodyYaw(float);												// 228
	virtual __int64 setYHeadRot0(float);														// 229
	virtual bool isWorldBuilder(void);															// 230
	virtual bool isCreative(void);																// 231
	virtual bool isAdventure(void);																// 232
	virtual bool isSurvival(void);																// 233
	virtual bool isSpectator(void);																// 234
	virtual __int64 add(ItemStack &);															// 235
	virtual __int64 drop(ItemStack const &, bool);												// 236
	virtual __int64 getInteraction(Player &, __int64 &, Vec3 const &);							// 237
	virtual bool canDestroyBlock(Block const &);												// 238
	virtual __int64 setAuxValue(int);															// 239
	virtual __int64 setSize(float, float);														// 240
	virtual __int64 onOrphan(void);																// 241
	virtual __int64 wobble(void);																// 242
	virtual bool wasHurt(void);																	// 243
	virtual __int64 startSpinAttack(void);														// 244
	virtual __int64 stopSpinAttack(void);														// 245
	virtual __int64 setDamageNearbyMobs(bool);													// 246
	virtual __int64 renderDebugServerState(__int64);											// 247
	virtual __int64 reloadLootTable(__int64);													// 248
	virtual __int64 reloadLootTable(void);														// 249
	virtual __int64 getShadowHeightOffs7(void);													// 250
	virtual __int64 kill(void);																	// 251
	virtual __int64 die(__int64 const &);														// 252
	virtual __int64 shouldDropDeathLoot(void);													// 253
	virtual __int64 shouldTick(void);															// 254
	virtual __int64 extractSnapshot(__int64 &);													// 255
	virtual __int64 applySnapshot(__int64, __int64);											// 256
	virtual __int64 getNextStep(float);															// 257
	virtual __int64 getLootTable(void);															// 258
	virtual __int64 interpolatorTick(void);														// 259
	virtual __int64 updateEntitySpecificMolangVariables(__int64);								// 260
	virtual __int64 tryGetPlayingSoundAttributes(__int64);										// 261
	virtual __int64 getOperationMode(void);														// 262
	virtual bool canMakeStepSound(void);														// 263
	virtual __int64 doWaterSplashEffect(void);													// 264
	virtual __int64 _shouldProvideFeedbackOnHandContainerItemSet(__int64, ItemStack const &);	// 265
	virtual __int64 _shouldProvideFeedbackOnArmorSet(__int64, ItemStack const &);				// 266
	virtual __int64 unknown7(void);																// 267
	virtual __int64 shouldTryMakeStepSound(void);												// 268
	virtual __int64 unknown8(void);																// 269
	virtual __int64 _hurt(__int64 const &, int, bool, bool);									// 270
	virtual __int64 markHurt(void);																// 271
	virtual __int64 _getAnimationComponent(__int64, __int64);									// 272
	virtual __int64 readAdditionalSaveData(CompoundTag const &, __int64);						// 273
	virtual __int64 addAdditionalSaveData(CompoundTag &);										// 274
	virtual __int64 _playStepSound(Vec3i const &, Block const &);								// 275
	virtual __int64 _playFlySound(Vec3i const &, Block const &);								// 276
	virtual __int64 _makeFlySound(void);														// 277
	virtual __int64 checkInsideBlocks(float);													// 278
	virtual __int64 pushOutOfBlocks(Vec3 const &);												// 279
	virtual __int64 spawnTrailBubbles(void);													// 280
	virtual __int64 updateInsideBlock(void);													// 281
	virtual __int64 _removePassenger(__int64 const &, bool, bool, bool);						// 282
	virtual __int64 _onSizeUpdated(void);														// 283
	virtual __int64 _doAutoAttackOnTouch(Entity *);												// 284
	virtual __int64 knockback(Entity *, int, float, float, float, float, float);				// 285
	virtual __int64 spawnAnim(void);															// 286
	virtual __int64 setSprinting(bool);															// 287
	virtual __int64 getHurtSound(void);															// 288
	virtual __int64 getDeathSound(void);														// 289
	virtual __int64 getItemInHandIcon(ItemStack const &, int);									// 290
	virtual __int64 getSpeed(void);																// 291
	virtual __int64 setSpeed(float);															// 292
	virtual __int64 getJumpPower(void);															// 293
	virtual __int64 hurtEffects(__int64 const &, int, bool, bool);								// 294
	virtual __int64 getMeleeWeaponDamageBonus(Entity *);										// 295
	virtual __int64 getMeleeKnockbackBonus(void);												// 296
	virtual __int64 travel(float, float, float);												// 297
	virtual __int64 travel(__int64, float, float, float);										// 298
	virtual __int64 applyFinalFriction(float, bool);											// 299
	// virtual __int64 updateWalkAnim(void);                                                    // 300?                                                                                                                                      // 290
	virtual __int64 aiStep(void);                                                    // 301
	virtual __int64 aiStep(__int64);                                                 // 302
	virtual __int64 pushActors(void);                                                // 303
	virtual __int64 lookAt(Entity *, float, float);                                  // 304
	virtual bool isLookingAtAnEntity(void);                                          // 305
	virtual __int64 checkSpawnRules(bool);                                           // 306
	virtual __int64 checkSpawnObstruction(void);                                     // 307
	virtual __int64 getAttackAnim(float);                                            // 308
	virtual __int64 getItemUseDuration(void);                                        // 309
	virtual __int64 getItemUseStartupProgress(void);                                 // 310
	virtual __int64 getItemUseIntervalProgress(void);                                // 311
	virtual __int64 getItemUseIntervalAxis(void);                                    // 312
	virtual __int64 getTimeAlongSwing(void);                                         // 313
	virtual __int64 ate(void);                                                       // 314
	virtual __int64 getMaxHeadXRot(void);                                            // 315
	virtual bool isAlliedTo(Entity *);                                               // 316
	virtual __int64 doHurtTarget(Entity *, __int64 const &);                         // 317
	virtual bool canBeControlledByPassenger(void);                                   // 318
	virtual __int64 leaveCaravan(void);                                              // 319
	virtual __int64 joinCaravan(Entity *);                                           // 320
	virtual bool hasCaravanTail(void);                                               // 321
	virtual __int64 getCaravanHead(void);                                            // 322
	virtual __int64 getArmorValue(void);                                             // 323
	virtual __int64 getArmorCoverPercentage(void);                                   // 324
	virtual __int64 hurtArmorSlots(__int64 const &, int, std::bitset<4ul> const &);  // 325
	virtual __int64 setDamagedArmor(int, ItemStack const &);                         // 326
	virtual __int64 sendArmorDamage(std::bitset<4ul> const &);                       // 327
	virtual __int64 sendArmor(std::bitset<4ul> const &);                             // 328
	virtual __int64 containerChanged(int);                                           // 329
	virtual __int64 updateEquipment(void);                                           // 330
	virtual __int64 clearEquipment(void);                                            // 331
	virtual __int64 getAllArmorID(void);                                             // 332
	virtual __int64 getAllHand(void);                                                // 333
	virtual __int64 getAllEquipment(void);                                           // 334
	virtual __int64 getArmorTypeHash(void);                                          // 335
	virtual __int64 dropEquipmentOnDeath(__int64 const &, int);                      // 336
	virtual __int64 dropEquipmentOnDeath(void);                                      // 337
	virtual __int64 clearVanishEnchantedItemsOnDeath(void);                          // 338
	virtual __int64 sendInventory(bool);                                             // 339
	virtual __int64 getDamageAfterMagicAbsorb(__int64 const &, int);                 // 340
	virtual __int64 createAIGoals(void);                                             // 341
	virtual __int64 onBorn(Entity *, Entity *);                                      // 342
	virtual __int64 setItemSlot(int, ItemStack const &);                             // 343
	virtual __int64 setTransitioningSitting(bool);                                   // 344
	virtual __int64 attackAnimation(Entity *, float);                                // 345
	virtual __int64 getAttackTime(void);                                             // 346
	virtual __int64 _getWalkTargetValue(Vec3i const &);                              // 347
	virtual bool canExistWhenDisallowMob(void);                                      // 348
	virtual __int64 useNewAi(void);                                                  // 349
	virtual __int64 ascendLadder(void);                                              // 350
	virtual __int64 ascendBlockByJumping(void);                                      // 351
	virtual __int64 descendBlockByCrouching(void);                                   // 352
	virtual __int64 dropContainer(void);                                             // 353
	virtual __int64 initBodyControl(void);                                           // 354
	virtual __int64 jumpFromGround(void);                                            // 355
	virtual __int64 jumpFromGround(int);                                             // 356
	virtual __int64 updateAi(void);                                                  // 357
	virtual __int64 newServerAiStep(void);                                           // 358
	virtual __int64 _serverAiMobStep(void);                                          // 359
	virtual __int64 getDamageAfterEnchantReduction(__int64 const &, int);            // 360
	virtual __int64 getDamageAfterArmorAbsorb(__int64 const &, int);                 // 361
	virtual __int64 dropBags(void);                                                  // 362
	virtual __int64 tickDeath(void);                                                 // 363
	virtual __int64 updateGliding(void);                                             // 364
	virtual __int64 _allowAscendingScaffolding(void);                                // 365
	virtual __int64 _getAdjustedAABBForSpawnCheck(AABB const &, Vec3 const &);       // 366

public:
	InventoryTransactionManager *getTransactionManager();

	AABB *getAABB() {
		return &this->aabb;
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
		return *reinterpret_cast<class Dimension **>(reinterpret_cast<__int64>(this) + 0x360);
	}

	void SetFieldOfView(float num) {
		*(float *)((uintptr_t)(this) + 0x1040) = num;
	}

	float GetFieldOfView() {
		return *reinterpret_cast<float *>(this + 0x1040);
	}

	class Level *getlevel() {
		return *reinterpret_cast<class Level **>(reinterpret_cast<__int64>(this) + 0x368);
	}

	void lerpTo(Vec3 const &pos, Vec2 const &a2, int a3);
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

	virtual __int64 prepareRegion(__int64);                                                          // 357
	virtual __int64 destroyRegion(void);                                                             // 358
	virtual __int64 suspendRegion(void);                                                             // 359
	virtual __int64 resendAllChunks(void);                                                           // 360
	virtual __int64 _fireWillChangeDimension(void);                                                  // 361
	virtual __int64 _fireDimensionChanged(void);                                                     // 362
	virtual __int64 changeDimensionWithCredits(int);                                                 // 363
	virtual __int64 tickWorld(int const &);                                                          // 364
	virtual __int64 frameUpdate(__int64);                                                            // 365
	virtual __int64 getTickingOffsets(void);                                                         // 366
	virtual __int64 moveView(void);                                                                  // 367
	virtual __int64 setName(TextHolder *);                                                           // 368
	virtual __int64 checkMovementStats(Vec3 const &);                                                // 369
	virtual __int64 getCurrentStructureFeature(void);                                                // 370
	virtual bool isAutoJumpEnabled(void);                                                            // 371
	virtual __int64 respawn(void);                                                                   // 372
	virtual __int64 resetRot(void);                                                                  // 373
	virtual __int64 resetPos(bool);                                                                  // 374
	virtual bool isInTrialMode(void);                                                                // 375
	virtual bool hasResource(int);                                                                   // 376
	virtual __int64 completeUsingItem(void);                                                         // 377
	virtual __int64 setPermissions(void);                                                            // 378
	virtual __int64 startDestroying(void);                                                           // 379
	virtual __int64 stopDestroying(void);                                                            // 380
	virtual __int64 openPortfolio(void);                                                             // 381
	virtual __int64 openBook(int, bool, int, __int64 *);                                             // 382
	virtual __int64 openTrading(__int64 const &, bool);                                              // 383
	virtual bool canOpenContainerScreen(void);                                                       // 384
	virtual __int64 openChalkboard(__int64 *, bool);                                                 // 385
	virtual __int64 openNpcInteractScreen(std::shared_ptr<__int64>);                                 // 386
	virtual __int64 openInventory(void);                                                             // 387
	virtual __int64 displayChatMessage(TextHolder *, TextHolder *);                                  // 388
	virtual __int64 displayClientMessage(TextHolder *);                                              // 389
	virtual __int64 displayTextObjectMessage(__int64 const &, TextHolder *, TextHolder *);           // 390
	virtual __int64 displayTextObjectWhisperMessage(__int64 const &, TextHolder *, TextHolder *);    // 391
	virtual __int64 displayWhisperMessage(TextHolder *, TextHolder *, TextHolder *, TextHolder *);   // 392
	virtual __int64 startSleepInBed(Vec3i const &);                                                  // 393
	virtual __int64 stopSleepInBed(bool, bool);                                                      // 394
	virtual bool canStartSleepInBed(void);                                                           // 395
	virtual __int64 getSleepTimer(void);                                                             // 396
	virtual __int64 getPreviousTickSleepTimer(void);                                                 // 397
	virtual __int64 openSign(Vec3i const &);                                                         // 398
	virtual void playEmote(std::string);                                                             // 399
	virtual bool isHostingPlayer(void);                                                              // 400
	virtual bool isLoading(void);                                                                    // 401
	virtual bool isPlayerInitialized(void);                                                          // 402
	virtual __int64 stopLoading(void);                                                               // 403
	virtual __int64 registerTrackedBoss(__int64);                                                    // 404
	virtual __int64 unRegisterTrackedBoss(__int64);                                                  // 405
	virtual __int64 setPlayerGameType(int);                                                          // 406
	virtual __int64 initHUDContainerManager(void);                                                   // 407
	virtual __int64 _crit(Entity *);                                                                 // 408
	virtual __int64 getEventing(void);                                                               // 409
	virtual __int64 getUserId(void);                                                                 // 410
	virtual __int64 sendEventPacket(__int64);                                                        // 411
	virtual __int64 addExperience(int);                                                              // 412
	virtual __int64 addLevels(int);                                                                  // 413
	virtual __int64 setContainerData(__int64, int, int);                                             // 414
	virtual __int64 slotChanged(__int64, __int64, int, ItemStack const &, ItemStack const &, bool);  // 415
	virtual __int64 inventoryChanged(__int64, int, ItemStack const &, ItemStack const &, bool);      // 416
	virtual __int64 refreshContainer(__int64);                                                       // 417
	virtual __int64 deleteContainerManager(void);                                                    // 418
	virtual __int64 setFieldOfViewModifier(float);                                                   // 419
	virtual bool is2DPositionRelevant(int, Vec3i const &);                                           // 420
	virtual bool isActorRelevant(Entity *);                                                          // 421
	virtual bool isTeacher(void);                                                                    // 422
	virtual __int64 onSuspension(void);                                                              // 423
	virtual __int64 onLinkedSlotsChanged(void);                                                      // 424
	virtual __int64 startCooldown(Item const *);                                                     // 425
	virtual __int64 getItemCooldownLeft();                                                           // 426
	virtual __int64 getItemCooldownLeft(unsigned long);                                              // 427
	virtual __int64 getMaxItemCooldownLeft(void);                                                    // 428
	virtual bool isItemInCooldown(HashedString const &);                                             // 429
	virtual __int64 sendInventoryTransaction(InventoryTransaction const &);                          // 430
	virtual __int64 sendComplexInventoryTransaction(__int64);                                        // 431
	virtual __int64 sendNetworkPacket(class Packet &);                                               // 432
	virtual __int64 getPlayerEventCoordinator(void);                                                 // 433
	virtual __int64 getMoveInputHandler(void);                                                       // 434
	virtual __int64 getInputMode(void);                                                              // 435
	virtual __int64 getPlayMode(void);                                                               // 436
	virtual __int64 reportMovementTelemetry(int);                                                    // 437
	virtual bool isSimulated(void);                                                                  // 438
	virtual __int64 getXuid(void);                                                                   // 439
	virtual __int64 getMovementSettings(void);                                                       // 440
	virtual __int64 onMovePlayerPacketNormal(Vec3 const &, Vec2 const &, float);                     // 441
	virtual __int64 _createChunkSource();                                                            // 442
	virtual __int64 _getSpawnChunkLimit(void);                                                       // 443
	virtual __int64 _updateChunkPublisherView(Vec3 const &, float);
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