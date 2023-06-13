#pragma once
//#include <string>
#include <vector>

#include "../Memory/GameData.h"
#include "../resource.h"

struct SkinData {
public:
	BUILD_ACCESS(this, unsigned int, SkinWidth, 0x4);
	BUILD_ACCESS(this, unsigned int, SkinHeight, 0x8);
	BUILD_ACCESS(this, void*, skinData, 0x10);
	BUILD_ACCESS(this, size_t, skinSize, 0x18);
};

namespace MeshStructs {
struct face {
	struct facePart {
		int vertIndex = -1, normalIndex = -1, uvIndex = -1;
	} indices[8];
	int facesPresent = 4;
};

/*
using namespace nlohmann;
class json;
void to_json(json& j, const face& f);*/

struct meshData {
	std::vector<std::array<float, 3>> vertices;
	std::vector<std::array<float, 3>> normals;
	std::vector<std::array<float, 2>> uvs;
	std::vector<face> faces;
};
}  // namespace MeshStructs

class SkinUtil {
public:
	static void importGeo(std::wstring filePath);
	static std::string modGeometry(const char* oldGeoStr, MeshStructs::meshData mesh);
	static MeshStructs::meshData objToMesh(const char* str, bool convertTrisToQuads = true);
};
