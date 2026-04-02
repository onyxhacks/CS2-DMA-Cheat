#pragma once

#include "Game.h"
#include "Entity.h"
#include "AimBot.hpp"
#include "TriggerBot.h"

namespace Cheats
{
	void Run();
	inline CEntity LocalEntityPlayer;

	inline std::vector<CEntity> EntityList;

	inline bool TriggerKey = false;
	inline bool AimKey = false;
}
