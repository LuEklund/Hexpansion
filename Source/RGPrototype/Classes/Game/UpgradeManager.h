// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UpgradeManager.generated.h"

struct FInGameUpgradeRow;
class ARGGameMode;
struct FGameplayTag;
/**
 * 
 */
UCLASS()
class RGPROTOTYPE_API UUpgradeManager : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TObjectPtr<ARGGameMode>	RGGamemode;

	UPROPERTY(BlueprintReadOnly)
	TArray<FInGameUpgradeRow>	UnlockedInGameUpgrades;
	UFUNCTION(BlueprintCallable)
	TArray<FInGameUpgradeRow> GetRandomUpgrades();
	FInGameUpgradeRow *GetUpgradeByName(FName Name);
	
	void Upgrade(FInGameUpgradeRow &UpgradeInfo);
	
};
