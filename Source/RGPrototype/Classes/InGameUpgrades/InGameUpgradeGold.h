// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InGameUpgrades/InGameUpgradeBase.h"
#include "InGameUpgradeGold.generated.h"

/**
 * 
 */

// UENUM()
// enum EGoldUpgradeType
// {
// 	FlatInstantGold,
// 	PercentInstantGold,
// 	PercentagePerGoldReceive
// 	
// };
UENUM(BlueprintType)
enum class EGoldUpgradeType : uint8
{
	FlatInstantGold UMETA(DisplayName = "FlatInstantGold"),
	PercentInstantGold UMETA(DisplayName = "PercentInstantGold"),
	PercentagePerGoldReceive UMETA(DisplayName = "PercentagePerGoldReceive")
};

UCLASS()
class RGPROTOTYPE_API UInGameUpgradeGold : public UInGameUpgradeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	EGoldUpgradeType Type;
	
	virtual void Execute(FInGameUpgradeRow* Upgrade) override;
};
