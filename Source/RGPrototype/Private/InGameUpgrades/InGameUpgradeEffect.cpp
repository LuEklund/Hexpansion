// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUpgrades/InGameUpgradeEffect.h"

#include "RGGameplayTags.h"
#include "Buildings/Tower.h"
#include "Game/RGGameMode.h"
#include "Kismet/GameplayStatics.h"


void UInGameUpgradeEffect::Execute(FInGameUpgradeRow* Upgrade)
{
	Super::Execute(Upgrade);
	if (!GetWorld()) return;
	ARGGameMode *RGGamemode = GetWorld()->GetAuthGameMode<ARGGameMode>();
	const FRGGameplayTags& RGameplayTags = FRGGameplayTags::Get();
	
	TArray<AActor*>	AllTowers;
	UGameplayStatics::GetAllActorsWithTag(this, TowerTypeTag.GetTagName(), AllTowers);
	
	if (EffectTypeTag.MatchesTagExact(RGameplayTags.Effect_Bleed))
	{
		for (AActor* Actor : AllTowers)
		{
			if (ATower *Tower = Cast<ATower>(Actor))
			{
				Tower->BuildingInfo.Bleed += Upgrade->FlatValueIncrease;
			}
		}
		RGGamemode->BuildingData->GetBuildingByTag(TowerTypeTag).Bleed += Upgrade->FlatValueIncrease;
	}
}
