// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UpgradeManager.h"

#include "Algo/RandomShuffle.h"
#include "Game/RGGameMode.h"
#include "InGameUpgrades/InGameUpgradeBase.h"




TArray<FInGameUpgradeRow> UUpgradeManager::GetRandomUpgrades()
{
	Algo::RandomShuffle(UnlockedInGameUpgrades);
	FGameplayTag UpgradeType;
	switch (RGGamemode->PreviousUpgradeType)
	{
		case EUpgradeType::GOLD:
			UpgradeType = FRGGameplayTags::Get().Upgrade_Gold;
			break;
		case EUpgradeType::UTILITY_ATTACK_RANGE:
			UpgradeType = FRGGameplayTags::Get().Upgrade_Range;
			break;
		case EUpgradeType::UTILITY_ATTACK_SPEED:
			UpgradeType = FRGGameplayTags::Get().Upgrade_AttackSpeed;
			break;
		case EUpgradeType::DAMAGE:
			UpgradeType = FRGGameplayTags::Get().Upgrade_Damage;
			break;
		case EUpgradeType::BOSS:
			UpgradeType = FRGGameplayTags::Get().Upgrade_Gold;
			break;
		case EUpgradeType::SPECIAL_EFFECT:
			UpgradeType = FRGGameplayTags::Get().Upgrade_SpecialEffect;
			break;
		default:
			UpgradeType = FRGGameplayTags::Get().Upgrade_Gold;
			break;
	}
	TArray<FInGameUpgradeRow> RightCategory;
	for (FInGameUpgradeRow Upgrade : UnlockedInGameUpgrades)
	{
		if (Upgrade.UpgradeType == UpgradeType)
		{
			RightCategory.Add(Upgrade);
		}
	}
	return (RightCategory);
}

FInGameUpgradeRow *UUpgradeManager::GetUpgradeByName(FName Name)
{
	for (auto &Upgrade : UnlockedInGameUpgrades)
	{
		if (Upgrade.Name.IsEqual(Name))
		{
			return &Upgrade;
		}
	}
	return nullptr;
}

void UUpgradeManager::Upgrade(FInGameUpgradeRow &UpgradeInfo)
{
	FInGameUpgradeRow *Upgrade = GetUpgradeByName(UpgradeInfo.Name);
	if (!Upgrade) return;

	if (Upgrade->UpgradeClass)
	{
		UInGameUpgradeBase *UpgradeBase = NewObject<UInGameUpgradeBase>(this, Upgrade->UpgradeClass);
		UpgradeBase->Execute(Upgrade);
	}
	
	return;
	
	// for (int32 Index = 0; Index < UnlockedInGameUpgrades.Num(); ++Index)
	// {
	// 	if (UnlockedInGameUpgrades[Index].Name.IsEqual(UpgradeInfo.Name))
	// 	{
	// 		UnlockedInGameUpgrades.RemoveAt(Index);
	// 		return; // Assuming each name is unique, we can exit early after removal
	// 	}
	// }
}
