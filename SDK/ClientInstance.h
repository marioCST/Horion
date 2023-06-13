#pragma once

#include "Entity.h"
#include "GameSettingsInput.h"
#include "LoopbackPacketSender.h"
#include "MinecraftUIRenderContext.h"
#include "TextHolder.h"

class Minecraft {
public:
	BUILD_ACCESS(this, float*, timer, 0xD8);
	BUILD_ACCESS(this, float*, otherTimer, 0xE0);

	void setTimerSpeed(float tps) {
		*this->timer = tps;
		*this->otherTimer = tps;
	}
};

class Tessellator;
class Block;

class BlockTessellator;
class ResourceLocation {
private:
	BUILD_ACCESS(this, __int64, hashCode, 0x28);
};

class HashedString {
private:
	unsigned __int64 hash;
	TextHolder text;  // 0x0008

public:
	HashedString(const std::string& text) {
		this->text.setText(text);

		this->computeHash();
	}

	void computeHash() {
		hash = 0xCBF29CE484222325i64;
		if (this->text.getTextLength() <= 0)
			return;
		char* textP = this->text.getText();
		auto c = *textP;

		do {
			hash = hash * 0x100000001B3i64 ^ c;
			c = *++textP;
		} while (*textP);
	}

	unsigned __int64 getHash() {
		return this->hash;
	}
};

namespace mce {
class TextureGroup;
class MaterialPtr;
class Mesh {
public:
	void renderMesh(__int64 screenContext, mce::MaterialPtr* material, size_t numTextures, __int64** textureArray);

	template <size_t numTextures>
	void renderMesh(__int64 screenContext, mce::MaterialPtr* material, std::array<__int64*, numTextures> textures) {
		this->renderMesh(screenContext, material, numTextures, &textures[0]);
	}
};
class TexturePtr {
private:
	BUILD_ACCESS(this, __int64*, clientTexture, 0x0);
	BUILD_ACCESS(this, ResourceLocation, resourceLocation, 0x10);

public:
	__int64* getClientTexture() {
		return this->clientTexture;
	}
};
class MaterialPtr {
private:
	std::shared_ptr<void> materialPtr;

public:
	MaterialPtr(HashedString materialName, bool switchable = false);
};
}  // namespace mce

class LevelRendererPlayer {
public:
	BUILD_ACCESS(this, Vec3, cameraPos, 0x514);
};

class LevelRenderer {
public:
	BUILD_ACCESS(this, LevelRendererPlayer*, levelRendererPlayer, 0x2F8);
	Vec3& getOrigin() {
		return levelRendererPlayer->cameraPos;
	}
};

class HitDetectSystem;

struct FontRepository_FontList_FontEntry {
public:
	Font* font;

private:
	void* sharedFontPtr;
};

struct FontRepository_FontList {
public:
	FontRepository_FontList_FontEntry fontEntries[9];
};

struct FontRepository {
private:
	BUILD_ACCESS(this, uintptr_t*, font_repository_vtable, 0x0);
	BUILD_ACCESS(this, void*, ptrToSelf, 0x10);
	BUILD_ACCESS(this, void*, ptrToSelfSharedPtr, 0x18);
public:
	BUILD_ACCESS(this, FontRepository_FontList*, fontList, 0x28);
										  //FontRepository_FontList* fontList1;  //0x0040
};

class MinecraftGame {
public:
	BUILD_ACCESS(this, FontRepository*, fontRepository, 0xEC8);
	BUILD_ACCESS(this, Font*, mcFont, 0xED8);
	BUILD_ACCESS(this, bool, canUseKeys, 0x130);

	Font* getTheGoodFontThankYou() {
		return fontRepository->fontList->fontEntries[7].font;
	};

	Font* getTheRandomFontPlease() {
		return fontRepository->fontList->fontEntries[4].font;
	};

	Font* getOldFont() {
		return fontRepository->fontList->fontEntries[7].font;
	};

	Font* getTheBetterFontYes() {
		return fontRepository->fontList->fontEntries[3].font;
	}

	Font* getMCFont() {
		return mcFont;
	}

	const bool canUseKeybinds() {
		return canUseKeys;
	};

	__int64 getServerEntries();
};

class GuiData {
private:
	char pad_0x0000[0x18];  //0x0000
public:
	union {
		struct {
			float widthReal;   //0x0018
			float heightReal;  //0x001C
		};
		Vec2 windowSizeReal;  //0x0018
	};

	float widthReal2;   //0x0020
	float heightReal2;  //0x0024
	union {
		struct {
			float widthGame;   //0x0028
			float heightGame;  //0x002C
		};
		Vec2 windowSize;  //0x0028
	};

	void displayClientMessageVA(const char* fmt, va_list lis);

	void displayClientMessageF(const char* fmt, ...);
	void displayClientMessageNoSendF(const char* fmt, ...);
	void displayClientMessage(std::string* a2);
};

struct PtrToGameSettings3 {
public:
	BUILD_ACCESS(this, GameSettingsInput*, settingsInput, 0x18);
};

struct PtrToGameSettings2 {
public:
	BUILD_ACCESS(this, PtrToGameSettings3*, ptr, 0x148);
};

struct PtrToGameSettings1 {
public:
	BUILD_ACCESS(this, PtrToGameSettings2*, ptr, 0x18);
};

class MoveInputHandler;
class CameraManager;

class ClientInstance {
public:
	BUILD_ACCESS(this, MinecraftGame*, minecraftGame, 0xC8);
	BUILD_ACCESS(this, Minecraft*, minecraft, 0xD0);
	BUILD_ACCESS(this, LevelRenderer*, levelRenderer, 0xE0);
	BUILD_ACCESS(this, LoopbackPacketSender*, loopbackPacketSender, 0xF0);
	BUILD_ACCESS(this, PtrToGameSettings1*, ptr, 0x110);
	BUILD_ACCESS(this, HitDetectSystem*, hitDetectSystem, 0x120);
	BUILD_ACCESS(this, class ItemInHandRenderer*, itemInHandRenderer, 0x500);

	glmatrixf* getRefDef() {
		return reinterpret_cast<glmatrixf*>((uintptr_t)(this) + 0x300);
	};

	Vec2* getMousePos() {
		return reinterpret_cast<Vec2*>((uintptr_t)(this) + 0x468);
	}

	Vec2 getFov() {
		Vec2 fov;
		fov.x = *reinterpret_cast<float*>((uintptr_t)(this) + 0x624);
		fov.y = *reinterpret_cast<float*>((uintptr_t)(this) + 0x638);
		return fov;
	}

	Font* getFont() {
		/*using getFont_t = Font*(__fastcall*)(ClientInstance*);
		static getFont_t getFontFunc = reinterpret_cast<getFont_t>(FindSignature("40 53 48 83 EC 30 48 8B DA 48 8B 89 ? ? ? ? 48 8B 01 48 8D 54 24 ? FF 90 ? ? ? ? 90 48 8B C8 E8 ? ? ? ? 48 8B D0 48 8B CB E8 ? ? ? ? 90 48 8D 4C 24 ? E8 ? ? ? ? 48 8B C3 48 83 C4 30 5B C3 CC CC CC CC CC CC CC CC CC CC 48 89 5C 24 ?"));
		return getFontFunc(this);*/
		return reinterpret_cast<Font*>((uintptr_t)(this) + 0xED8);
	}

	Font* getRuneFont() {
		using getRuneFont_t = Font*(__fastcall*)(ClientInstance*);
		static getRuneFont_t getRuneFontFunc = reinterpret_cast<getRuneFont_t>(FindSignature("40 53 48 83 EC 30 48 8B DA 48 8B 89 ? ? ? ? 48 8B 01 48 8D 54 24 ? FF 90 ? ? ? ? 90 48 8B C8 E8 ? ? ? ? 48 8B D0 48 8B CB E8 ? ? ? ? 90 48 8D 4C 24 ? E8 ? ? ? ? 48 8B C3 48 83 C4 30 5B C3 CC CC CC CC CC CC CC CC CC CC 40 53 48 83 EC 30"));
		return getRuneFontFunc(this);
	}

	Font* getUnicodeFont() {
		using getUnicodeFont_t = Font*(__fastcall*)(ClientInstance*);
		static getUnicodeFont_t getUnicodeFontFunc = reinterpret_cast<getUnicodeFont_t>(FindSignature("40 53 48 83 EC 30 48 8B DA 48 8B 89 ? ? ? ? 48 8B 01 48 8D 54 24 ? FF 90 ? ? ? ? 90 48 8B C8 E8 ? ? ? ? 48 8B D0 48 8B CB E8 ? ? ? ? 90 48 8D 4C 24 ? E8 ? ? ? ? 48 8B C3 48 83 C4 30 5B C3 CC CC CC CC CC CC CC CC CC CC 40 53 48 83 EC 20"));
		return getUnicodeFontFunc(this);
	}

	Font* _getFont() {
		return this->getFont();
	}

	Font* _getRuneFont() {
		return this->getRuneFont();
	}

	Font* _getUnicodeFont() {
		return this->getUnicodeFont();
	}

	MoveInputHandler* getMoveTurnInput() {
		using GetMoveInputHandlerFunc = MoveInputHandler*(__fastcall*)(ClientInstance*);
		static GetMoveInputHandlerFunc getMoveInputHandlerFunc = reinterpret_cast<GetMoveInputHandlerFunc>(FindSignature("40 53 48 83 EC ? 48 8B 01 8B DA 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 48 85 C0"));
		return getMoveInputHandlerFunc(this);
	}

	void grabMouse() {
		return Utils::CallVFunc<284, void>(this);
	}

	void releaseMouse() {
		return Utils::CallVFunc<285, void>(this);
	}

	GuiData* getGuiData() {
		return (GuiData*)*reinterpret_cast<__int64*>(reinterpret_cast<GuiData*>(reinterpret_cast<__int64>(this) + 0x510));
	};

	BlockTessellator* getBlockTesselator() {
		return (BlockTessellator*)*reinterpret_cast<__int64*>(reinterpret_cast<BlockTessellator*>(reinterpret_cast<__int64>(this) + 0x4D8));
	};

	class LocalPlayer* getCILocalPlayer() {
		return Utils::CallVFunc<27, LocalPlayer*>(this);
	}

	inline GameSettingsInput* getGameSettingsInput() { return this->ptr->ptr->ptr->settingsInput; };
};