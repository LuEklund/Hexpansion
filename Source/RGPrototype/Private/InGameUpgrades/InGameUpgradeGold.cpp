// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUpgrades/InGameUpgradeGold.h"

#include "Game/RGGameMode.h"
#include "Waves/EnemySpawnSystem.h"

void UInGameUpgradeGold::Execute(FInGameUpgradeRow* Upgrade)
{
	Super::Execute(Upgrade);
	ARGGameMode *GameMode = GetWorld()->GetAuthGameMode<ARGGameMode>();

	switch (Type)
	{
	case EGoldUpgradeType::FlatInstantGold:// gain gold * waveID
			GameMode->GainGold(IncreaseValue);
		break;
	case EGoldUpgradeType::PercentInstantGold:
			GameMode->GainGold(GameMode->PlayerGold * IncreaseValue);// gain currentgold * waveID
		break;
	case EGoldUpgradeType::PercentagePerGoldReceive: //Increase gold by enemykilled by % * waveID
			GameMode->PlayerGoldMultiplier += IncreaseValue;
		break;
	}

}
