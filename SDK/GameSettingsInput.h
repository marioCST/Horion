#pragma once

class GameSettingsInput {
public:
	BUILD_ACCESS(this, int*, spaceBarKey, 0x3E0);
	BUILD_ACCESS(this, int*, sneakKey, 0x420);
	BUILD_ACCESS(this, int*, leftKey, 0x4A0);
	BUILD_ACCESS(this, int*, rightKey, 0x4E0);
	BUILD_ACCESS(this, int*, backKey, 0x520);
	BUILD_ACCESS(this, int*, forwardKey, 0x560);
};
