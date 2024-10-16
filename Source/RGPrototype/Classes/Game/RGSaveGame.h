// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RGSaveGame.generated.h"


struct FGameplayTag;

USTRUCT()
struct FPlayerStats
{
	GENERATED_BODY()
public:
	UPROPERTY()
	int32 PlayerXP = 0;

	UPROPERTY()
	int32 BonusGoldMultiplier = 0;
};

/**
 * 
 */
UCLASS()
class RGPROTOTYPE_API URGSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FGameplayTag>	UnlockedUpgrades;

	UPROPERTY()
	FPlayerStats PlayerStats;
	
};
