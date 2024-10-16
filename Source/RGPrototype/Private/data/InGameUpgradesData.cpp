// Fill out your copyright notice in the Description page of Project Settings.


#include "data/InGameUpgradesData.h"


// TArray<FInGameUpgrade> UInGameUpgradesData::GetUnlockedUpgrades()
// {
// 	TArray<FInGameUpgrade> UnlockedUpgrades;
// 	for (auto UpgradeContainer : InGameUpgradeCategories)
// 	{
// 		for (const FInGameUpgrade Upgrade : UpgradeContainer.Value.Upgrades)
// 		{
// 			if (Upgrade.bUnlocked)
// 			{
// 				UnlockedUpgrades.Add(Upgrade);
// 			}
// 		}
// 	}
// 	return UnlockedUpgrades;
// }

// TArray<FInGameUpgrade> UInGameUpgradesData::GetUnlockedUpgrades()
// {
// 	TArray<FInGameUpgrade> Upgrades;
// 	//TODO:
// 	
//
// 	for (auto UpgradeContainer : InGameUpgradeCategories)
// 	{
// 		// for (const FInGameUpgrade Upgrade : UpgradeContainer.Value.Upgrades)
// 		// {
// 		// 	UE_LOG(LogTemp, Error, TEXT("Status: %s"), *Upgrade.State.GetTagName().ToString());
// 		// 	if (Upgrade.State.MatchesTag(FRGGameplayTags::Get().State_Unlocked))
// 		// 	{
// 		// 		Upgrades.Add(Upgrade);
// 		// 	}
// 		// }
// 	}
// 	return Upgrades;
// }

TMap<FName, FPermaUpgradeCategory>& UInGameUpgradesData::GetInGameUpgradeCategories()
{
	return (InGameUpgradeCategories);
}

TArray<FInGameUpgradeRow> UInGameUpgradesData::GetUpgradesForCategory(FName CategoryName)
{
	TArray<FInGameUpgradeRow> Upgrades;
	FPermaUpgradeCategory *Category = InGameUpgradeCategories.Find(CategoryName);
	if (!Category) return Upgrades;
	for (auto UpgradeDataTable : Category->Upgrades)
	{
		for (auto TableRow : UpgradeDataTable->GetRowMap())
		{
			Upgrades.Add(*reinterpret_cast<FInGameUpgradeRow*>(TableRow.Value));
		}
	}
	return Upgrades;
}

TArray<FInGameUpgradeRow> UInGameUpgradesData::GetUpgrades()
{
	TArray<FInGameUpgradeRow> Upgrades;
	

	for (auto UpgradeContainer : InGameUpgradeCategories)
	{
		for (auto DataTable : UpgradeContainer.Value.Upgrades)
		{
			for (auto TableRow : DataTable->GetRowMap())
			{
				Upgrades.Add(*reinterpret_cast<FInGameUpgradeRow*>(TableRow.Value));
			}
		}

	}
	return Upgrades;
}

FInGameUpgradeRow* UInGameUpgradesData::GetUpgradeByName(FName Name)
{
	for (auto UpgradeContainer : InGameUpgradeCategories)
	{
		for (auto DataTable : UpgradeContainer.Value.Upgrades)
		{
			for (auto TableRow : DataTable->GetRowMap())
			{
				if (Name.IsEqual(reinterpret_cast<FInGameUpgradeRow*>(TableRow.Value)->Name))
				{
					return (reinterpret_cast<FInGameUpgradeRow*>(TableRow.Value));
				}
			}
		}
	}
	return nullptr;
}

FInGameUpgradeRow UInGameUpgradesData::FindUpgradeForTag(const FGameplayTag Type, bool bLogNotFound) const
{
	// for (const FInGameUpgrade& InGameUpgrade : InGameUpgrades)
	// {
	// 	if (InGameUpgrade.BuildingType == Type)
	// 	{
	// 		return InGameUpgrade;
	// 	}
	// }
	UE_LOG(LogTemp, Error, TEXT("CALLED: FInGameUpgrade UInGameUpgardesData::FindUpgradeForTag(const FGameplayTag Type, bool bLogNotFound) const"))
	return FInGameUpgradeRow();
}
