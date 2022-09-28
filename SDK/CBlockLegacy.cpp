#include "CBlockLegacy.h"

#include "../Memory/GameData.h"
#include "../Utils/Logger.h"
#include "../Utils/Utils.h"
C_Block* C_BlockSource::getBlock(const vec3_ti& block) {
	using getBlock_t = C_Block*(__fastcall*)(C_BlockSource*, const vec3_ti&);
	static getBlock_t getBlock = reinterpret_cast<getBlock_t>(FindSignature("48 89 5C ? ? 48 89 74 ? ? 57 48 83 EC ? 80 79 50 ? 48"));
	return getBlock(this, block);
}
C_BlockActor* C_BlockSource::getBlockEntity(const vec3_ti& block) {
	using getBlockEntity_t = C_BlockActor*(__fastcall*)(C_BlockSource*, const vec3_ti&);
	static getBlockEntity_t getBlockEntity = reinterpret_cast<getBlockEntity_t>(FindSignature("48 89 5C ? ? 48 89 74 ? ? 57 48 83 EC ? 48 8B 01 48 8B FA 48 8B D9 48"));
	return getBlockEntity(this, block);
}
C_Block* C_BlockSource::getLiquidBlock(const vec3_ti& block) {
	using getLiquidBlock_t = C_Block*(__fastcall*)(C_BlockSource*, const vec3_ti&);  //E8 41 7D 6A 01 48 8B C8 48 8B 46 10
	static getLiquidBlock_t getLiquidBlock = Utils::FuncFromSigOffset<getLiquidBlock_t>(FindSignature("E8 ?? ?? ?? ?? 65 48 8B 0C 25 ?? ?? ?? ??"), 1);
	return getLiquidBlock(this, block);  //E8 41 7D 6A ?? ?? ?? ?? ?? ?? ?? ??
}
bool C_BlockLegacy::getCollisionShape(AABB* collShapeOut, C_Block* block, C_BlockSource* blockSource, const vec3_ti* pos, C_Entity* actor) {
	return Utils::CallVFunc<5, bool, AABB*, C_Block*, C_BlockSource*, const vec3_ti*, C_Entity*>(this, collShapeOut, block, blockSource, pos, 0);
}
void C_BlockLegacy::liquidGetFlow(vec3_t* flowOut, C_BlockSource* reg, const vec3_ti* pos) {
	using liquid_getFlow_t = int(__fastcall*)(C_BlockLegacy*, vec3_t*, C_BlockSource*, const vec3_ti*);
	static liquid_getFlow_t liquid_getDepth = Utils::FuncFromSigOffset<liquid_getFlow_t>(FindSignature("E8 ? ? ? ? 48 8B 5C ? ? 48 8B C7 48 83 C4 ? 5F C3 CC CC CC CC CC CC CC 48 8D 81"), 1);

	liquid_getDepth(this, flowOut, reg, pos);
}
bool C_BlockLegacy::hasWater(C_BlockSource* reg, const vec3_ti& pos) {
	if (this->material->isLiquid) {
		return !this->material->isSuperHot;
	}

	auto liquidBlock = reg->getLiquidBlock(pos)->toLegacy();
	return this != liquidBlock && liquidBlock->material->isLiquid && !liquidBlock->material->isSuperHot;
}