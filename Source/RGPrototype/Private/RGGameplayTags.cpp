#include "RGGameplayTags.h"

#include "GameplayTagsManager.h"


FRGGameplayTags FRGGameplayTags::GameplayTags;

void FRGGameplayTags::InitializeNativeGameplayTags()
{
	//Towers
	GameplayTags.Building_Tower_Single_Arrow = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Building.Tower.Single.Arrow"),
		FString("Projectile Tower"));
	GameplayTags.Building_Tower_Single_Sniper = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Building.Tower.Single.Sniper"),
		FString("Sniper Tower"));
	GameplayTags.Building_Tower_AOE_Slowness = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Building.Tower.AOE.Slowness"),
		FString("slows enemies"));
	GameplayTags.Building_Tower_AOE_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Building.Tower.AOE.Damage"),
			FString("Deal AoE Damage"));
	
	//Upgrades
	GameplayTags.Upgrade_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Upgrade.Damage"),
		FString("upgrades the damage for tower"));
	GameplayTags.Upgrade_Range = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Upgrade.Range"),
		FString("upgrades the Range for tower"));
	GameplayTags.Upgrade_AttackSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Upgrade.AttackSpeed"),
		FString("upgrades the AttackSpeed for tower"));
	GameplayTags.Upgrade_Gold = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Upgrade.Gold"),
	FString("Increase gold"));
	GameplayTags.Upgrade_SpecialEffect = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Upgrade.SpecialEffect"),
	FString("Gives player a Special Effect"));
	

	//Effects
	GameplayTags.Effect_Bleed = UGameplayTagsManager::Get().AddNativeGameplayTag(
    	FName("Effect.Bleed"),
    	FString("Bleeding Effect"));
	GameplayTags.Effect_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effect.Stun"),
		FString("Stunning Effect"));
	GameplayTags.Effect_Slow = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effect.Slow"),
		FString("Slowing Effect"));
	

	
	
	//State
	GameplayTags.State_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("State.Locked"),
	FString("Locked"));
	GameplayTags.State_Unlockable = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("State.Unlockable"),
	FString("Unlockable"));
	GameplayTags.State_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("State.Unlocked"),
	FString("Unlocked"));
	// GameplayTags.State_Available = UGameplayTagsManager::Get().AddNativeGameplayTag(
	// FName("State.Available"),
	// FString("Available"));
	// GameplayTags.State_Used = UGameplayTagsManager::Get().AddNativeGameplayTag(
	// FName("State.Used"),
	// FString("Used"));
}
