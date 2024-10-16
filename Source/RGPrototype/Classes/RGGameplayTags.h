#pragma once
#include "GameplayTagContainer.h"


/**
 * RGGameplayTags
 *
 * Singleton containing native Gameplay Tags (C++)
 */

struct FRGGameplayTags
{
private:
	static FRGGameplayTags GameplayTags;
public:
	static  const FRGGameplayTags &Get() {return GameplayTags;}
	static  void  InitializeNativeGameplayTags();

	//Towers
	FGameplayTag Building_Tower_Single_Arrow;
	FGameplayTag Building_Tower_Single_Sniper;
	FGameplayTag Building_Tower_AOE_Slowness;
	FGameplayTag Building_Tower_AOE_Damage;

	//Upgrades
	FGameplayTag Upgrade_Damage;
	FGameplayTag Upgrade_Range;
	FGameplayTag Upgrade_AttackSpeed;
	FGameplayTag Upgrade_Gold;
	FGameplayTag Upgrade_SpecialEffect;

	//Effects
	FGameplayTag Effect_Bleed;
	FGameplayTag Effect_Stun;
	FGameplayTag Effect_Slow;

	//State
	FGameplayTag State_Locked;
	FGameplayTag State_Unlockable;
	FGameplayTag State_Unlocked;

	


};