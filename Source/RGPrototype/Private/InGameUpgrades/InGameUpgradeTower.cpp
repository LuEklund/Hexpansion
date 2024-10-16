// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUpgrades/InGameUpgradeTower.h"

#include "Buildings/Tower.h"
#include "Game/RGGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Waves/EnemySpawnSystem.h"

void	UInGameUpgradeTower::Execute(FInGameUpgradeRow *Upgrade)
{
	Super::Execute(Upgrade);
	if (!GetWorld()) return;
	ARGGameMode *RGGamemode = GetWorld()->GetAuthGameMode<ARGGameMode>();
	const FRGGameplayTags& RGameplayTags = FRGGameplayTags::Get();

	TArray<AActor*>	AllTowers;
	UGameplayStatics::GetAllActorsWithTag(this, BuildingType.GetTagName(), AllTowers);
	
	if (UpgradeType.MatchesTagExact(RGameplayTags.Upgrade_Damage))
	{
		for (AActor *Actor : AllTowers)
		{
			UE_LOG(LogTemp, Error, TEXT("%s Increased attack damage"), *Actor->GetName());
			Cast<ATower>(Actor)->BuildingInfo.AttackDamage += IncreaseValue;
		}
		RGGamemode->BuildingData->GetBuildingByTag(BuildingType).AttackDamage += IncreaseValue;
	}
	else if (UpgradeType.MatchesTagExact(RGameplayTags.Upgrade_Range))
	{
		for (AActor *Actor : AllTowers)
		{
			UE_LOG(LogTemp, Error, TEXT("%s Increased attack Range"), *Actor->GetName());
			Cast<ATower>(Actor)->BuildingInfo.AttackRange += IncreaseValue;
		}
		RGGamemode->BuildingData->GetBuildingByTag(BuildingType).AttackRange += IncreaseValue;
	}
	else if (UpgradeType.MatchesTagExact(RGameplayTags.Upgrade_AttackSpeed))
	{
		for (AActor *Actor : AllTowers)
		{
			UE_LOG(LogTemp, Error, TEXT("%s Increased attack Speed"), *Actor->GetName());
			ATower *Tower = Cast<ATower>(Actor);
			float TowerCurrentAttackSpeed = Tower->GetTowerAttackSpeed();
			Tower->SetTowerAttackSpeed(TowerCurrentAttackSpeed + IncreaseValue);
	
			// Cast<ATower>(Actor)->TowerDamage += IncreaseFlatDamage;
		}
		RGGamemode->BuildingData->GetBuildingByTag(BuildingType).AttackSpeed += IncreaseValue;
	}
}
