#pragma once

// SetLore func: 48 8B C4 48 89 58 ? 48 89 70 ? 55 57 41 54 41 56 41 57 48 8D 68 ? 48 81 EC ? ? ? ? ? ? 70 C8 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4C

#include "ICommand.h"

class SetLoreCommand : public IMCCommand {
public:
	SetLoreCommand();
	~SetLoreCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
