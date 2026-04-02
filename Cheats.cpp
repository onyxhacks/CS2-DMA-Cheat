#include "Cheats.h"
#include "MenuConfig.hpp"

void Cheats::Run()
{
	// AimBot data
	float DistanceToSight = 0;
	float MaxAimDistance = 100000;
	Vec3  AimPos{ 0,0,0 };

	for (int i = 0; i < (int)EntityList.size(); i++)
	{
		CEntity Entity = EntityList[i];
		if (Entity.Pawn.Health <= 0)
			continue;

		if (MenuConfig::TeamCheck && Entity.Controller.TeamID == LocalEntityPlayer.Controller.TeamID)
			continue;

		if (!Entity.IsInScreen())
			continue;

		DistanceToSight = Entity.GetBone().BonePosList[BONEINDEX::head].ScreenPos.DistanceTo({ (float)gGame.View.Width / 2, (float)gGame.View.Height / 2 });

		if (DistanceToSight < MaxAimDistance)
		{
			MaxAimDistance = DistanceToSight;
			if (!MenuConfig::VisibleCheck ||
				Entity.Pawn.bSpottedByMask & (DWORD64(1) << (LocalEntityPlayer.LocalPlayerControllerIndex)) ||
				LocalEntityPlayer.Pawn.bSpottedByMask & (DWORD64(1) << (i)))
			{
				if (std::find(GunList::pistolsList.begin(), GunList::pistolsList.end(), LocalEntityPlayer.Pawn.WeaponName) != GunList::pistolsList.end()) {
					AimPos = Entity.GetBone().BonePosList[MenuConfig::AimPositionIndexPistol].Pos;
					if (MenuConfig::AimPositionIndexPistol == BONEINDEX::head)
						AimPos.z -= 1.f;
				} else if (std::find(GunList::snipersList.begin(), GunList::snipersList.end(), LocalEntityPlayer.Pawn.WeaponName) != GunList::snipersList.end()) {
					AimPos = Entity.GetBone().BonePosList[MenuConfig::AimPositionIndexSniper].Pos;
					if (MenuConfig::AimPositionIndexSniper == BONEINDEX::head)
						AimPos.z -= 1.f;
				}
				else {
					AimPos = Entity.GetBone().BonePosList[MenuConfig::AimPositionIndex].Pos;
					if (MenuConfig::AimPositionIndex == BONEINDEX::head)
						AimPos.z -= 1.f;
				}
			}
		}
	}

	// TriggerBot
	if (MenuConfig::TriggerMode == 0 && MenuConfig::TriggerBot && TriggerKey)
	{
		MenuConfig::Shoot = true;
		TriggerBot::Run(LocalEntityPlayer);
		MenuConfig::Shoot = false;
	}
	else if (MenuConfig::TriggerMode == 1 && MenuConfig::TriggerBot)
	{
		MenuConfig::Shoot = true;
		TriggerBot::Run(LocalEntityPlayer);
		MenuConfig::Shoot = false;
	}

	// AimBot
	if (MenuConfig::AimBot && AimKey)
	{
		if (AimPos != Vec3(0, 0, 0))
		{
			AimControl::AimBot(LocalEntityPlayer, LocalEntityPlayer.Pawn.CameraPos, AimPos);
		}
	}
}
