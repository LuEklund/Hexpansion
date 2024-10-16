// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RGGameplayTags.h"
#include "Engine/DataAsset.h"
#include "InGameUpgradesData.generated.h"


class UInGameUpgradeBase;

USTRUCT(BlueprintType)
struct FInGameUpgradeRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D>	Primary_image = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D>	Secondary_image = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FName>	Dependencies;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName	Name = FName();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName	Description = FName();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 UnlockXPCost = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float FlatValueIncrease = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag State = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag UpgradeType = FGameplayTag();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInGameUpgradeBase>	UpgradeClass;
};

USTRUCT(BlueprintType)
struct FPermaUpgradeCategory
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag State = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UDataTable>>	Upgrades;
};

/**
 * 
 */
UCLASS()
class RGPROTOTYPE_API UInGameUpgradesData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, FPermaUpgradeCategory> InGameUpgradeCategories;

	UFUNCTION(BlueprintCallable)
	TMap<FName, FPermaUpgradeCategory> &GetInGameUpgradeCategories();

	UFUNCTION(BlueprintCallable)
	TArray<FInGameUpgradeRow> GetUpgradesForCategory(FName Name);
	
	TArray<FInGameUpgradeRow> GetUpgrades();

	FInGameUpgradeRow* GetUpgradeByName(FName Name);
	
	FInGameUpgradeRow FindUpgradeForTag(const FGameplayTag AbilityTag, bool bLogNotFound = false) const;

};
