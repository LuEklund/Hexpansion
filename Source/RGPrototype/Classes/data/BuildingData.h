// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "BuildingData.generated.h"

class UAbilityBase;
class ATower;
/**
 * 
 */


USTRUCT(BlueprintType)
struct FUpgradeTowerRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Cost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Bleed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Stun = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Slow = 0.f;
};


USTRUCT(BlueprintType)
struct FBuilding
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName	Name = FName();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D>	Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag	BuildingTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32	Cost = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32	CostIncrease = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float	AttackRange = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float	AttackDamage = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float	AttackSpeed = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float	Bleed = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float	Stun = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float	Slow = 0.f;

	UPROPERTY(BlueprintReadOnly)
	int32	Level = 1;
	// FScalableFloat xd;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Class Deafults")
	TObjectPtr<UDataTable>	UpgradeTable;
	// FScalableFloat XPReward = FScalableFloat();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ATower> TowerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAbilityBase>	AttackAbilityClass;
	
};

UCLASS()
class RGPROTOTYPE_API UBuildingData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FBuilding>	Buildings;

	UFUNCTION(BlueprintCallable)
	FBuilding& GetBuildingByTag(FGameplayTag Tag);

};
