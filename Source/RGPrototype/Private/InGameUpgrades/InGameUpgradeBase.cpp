// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUpgrades/InGameUpgradeBase.h"

#include "Game/RGGameMode.h"
#include "Waves/EnemySpawnSystem.h"


int32 UInGameUpgradeBase::GetXPCost()
{
	return 0;
}

void UInGameUpgradeBase::Execute(FInGameUpgradeRow* Upgrade)
{
	ARGGameMode *RGGamemode = GetWorld()->GetAuthGameMode<ARGGameMode>();
	IncreaseValue = Upgrade->FlatValueIncrease * RGGamemode->EnemySpawnSystem->WaveID;
}

