// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemySpawnerData.generated.h"

/**
 * 
 */


class AEnemyBase;

USTRUCT(BlueprintType)
struct FGroup
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AEnemyBase>	EnemyClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32	Amount = 0;
	
};

USTRUCT(BlueprintType)
struct FWave
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FGroup>	EnemyGroups;
};


UCLASS()
class RGPROTOTYPE_API UEnemySpawnerData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AEnemyBase>	Boss;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FWave>	Waves;

};
