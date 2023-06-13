#pragma once
class HashedString;
class Weather;

class Dimension {
public:
	BUILD_ACCESS(this, __int16, maxHeight, 0x5A);
	BUILD_ACCESS(this, class BrightnessRamp *, brightnessRamp, 0xB8);
	BUILD_ACCESS(this, int, dimensionId, 0xE0);
	BUILD_ACCESS(this, bool, hasCeiling, 0xE5);
	BUILD_ACCESS(this, int, timeThingy, 0xE8);
	BUILD_ACCESS(this, Weather *, weather, 0x120);
};

class Weather {
public:
	BUILD_ACCESS(this, float, fogLevel, 0x4C);

	virtual ~Weather();
	virtual void onSourceCreated(BlockSource &);
	virtual void onSourceDestroyed(BlockSource &);
	virtual void onAreaChanged(BlockSource &, Vec3i const &, Vec3i const &);
	virtual void onBlockChanged(BlockSource &, Vec3i const &, unsigned int, Block const &, Block const &, int, class ActorBlockSyncMessage const *, class BlockChangedEventTarget);
	virtual void onBrightnessChanged(BlockSource &, Vec3i const &);
	virtual void onBlockEntityChanged(BlockSource &, class BlockActor &);
	virtual void onBlockEntityAboutToBeRemoved(BlockSource &, std::shared_ptr<BlockActor>);
	virtual void onEntityChanged(BlockSource &, Entity &);
	virtual void onBlockEvent(BlockSource &, int, int, int, int, int);
	virtual void allChanged(void);
	virtual void addParticle(class ParticleType, Vec3 const &, Vec3 const &, int, CompoundTag const *, bool);
	virtual void sendServerLegacyParticle(ParticleType, Vec3 const &, Vec3 const &, int);
	virtual void addParticleEffect(HashedString const &, Vec3 const &, class MolangVariableMap const &);
	virtual void addParticleEffect2(HashedString const &, Entity const &, HashedString const &, Vec3 const &, MolangVariableMap const &);
	virtual void addTerrainParticleEffect(Vec3i const &, Block const &, Vec3 const &, float, float, float);
	virtual void addTerrainSlideEffect(Vec3i const &, Block const &, Vec3 const &, float, float, float);
	virtual void addBreakingItemParticleEffect(Vec3 const &, ParticleType, struct TextureUVCoordinateSet const &, bool);
	virtual void playMusic(std::string const &, Vec3 const &, float, float);
	virtual void playStreamingMusic(std::string const &, int, int, int);
	virtual void onEntityAdded(Entity &);
	virtual void onEntityRemoved(Entity &);
	virtual void onChunkLoaded(class ChunkSource &, class LevelChunk &);
	virtual void onChunkReloaded(ChunkSource &, LevelChunk &);
	virtual void onSubChunkLoaded(ChunkSource &, LevelChunk &, short);
	virtual void onChunkUnloaded(LevelChunk &);
	virtual void onLevelDestruction(std::string const &);
	virtual void levelEvent(class LevelEvent, Vec3 const &, int);
	virtual void levelEvent(LevelEvent, CompoundTag const &);
	virtual void levelSoundEvent(class LevelSoundEvent, Vec3 const &, int, class ActorDefinitionIdentifier const &, bool, bool);
	virtual void levelSoundEvent(std::string const &, Vec3 const &, float, float);
	virtual void stopSoundEvent(std::string const &);
	virtual void stopAllSounds(void);
	virtual void takePicture(__int64 &, Entity *, Entity *, struct ScreenshotOptions &);
	virtual void playerListChanged(void);
};