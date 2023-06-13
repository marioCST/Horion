#pragma once

#include "../Utils/Utils.h"

class Weather {
public:
	BUILD_ACCESS(this, bool, isRaining, 0x38);
	BUILD_ACCESS(this, bool, isLightning, 0x44);
	BUILD_ACCESS(this, float, lightningLevel, 0x48);
};