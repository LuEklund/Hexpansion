// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "data/BuildingData.h"
#include "UI/RGWidgetController.h"
#include "OverlayWidgetController.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInGameUpgrade);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOverlayExpand);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTowerInfoDisplay, bool, bHideInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameOver, bool, bWon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnValueChange, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCardInfoUpdate, FGameplayTag, UpdatedType);



UCLASS(BlueprintType, Blueprintable)
class RGPROTOTYPE_API UOverlayWidgetController : public URGWidgetController
{
	GENERATED_BODY()
public:
	virtual void	BindCallbacksToDependencies();
	UPROPERTY(BlueprintAssignable, Category="Stats")
	FOnValueChange	OnEnemyAmountChange;
	UPROPERTY(BlueprintAssignable, Category="Stats")
	FOnValueChange	OnEnemyDeath;
	UPROPERTY(BlueprintAssignable, Category="Stats")
	FOnValueChange	OnPlayerHealthChange;
	UPROPERTY(BlueprintAssignable, Category="Stats")
	FOnValueChange	OnGoldChange;
	UPROPERTY(BlueprintAssignable, Category="Stats")
	FOnValueChange	OnXpChange;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Stats")
	FOnCardInfoUpdate	OnTowerSelectChange;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Stats")
	FOnCardInfoUpdate	OnUpdateCardInfoChange;
	UPROPERTY(BlueprintAssignable, Category="Stats")
	FOnTowerInfoDisplay	OnTowerInfoDisplayChange;

	UPROPERTY(BlueprintAssignable, Category="Stats")
	FOnInGameUpgrade	OnShowInGameUpgrades;

	UPROPERTY(BlueprintAssignable, Category="Expand")
	FOnOverlayExpand	OnOverlayExpandDelegate;


	UPROPERTY(BlueprintAssignable, Category="GameOver")
	FOnGameOver			OnGameOverDelegate;

	
};
