// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OverlayWidgetController.h"
#include "Game/RGGameMode.h"

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GetRG_GM()->EnemyDeathsDelegate.AddLambda(
	[this](int32 EnemiesDied)
	{
		OnEnemyDeath.Broadcast(EnemiesDied);
	});
	GetRG_GM()->WaveStartDelegate.AddLambda(
	[this](int32 EnemiesSpawned)
	{
		OnEnemyAmountChange.Broadcast(EnemiesSpawned);
	});
	GetRG_GM()->PlayerHealthChangeDelegate.AddLambda(
	[this](int32 DamageReceived)
	{
		OnPlayerHealthChange.Broadcast(DamageReceived);
	});
	GetRG_GM()->PlayerGoldChangeDelegate.AddLambda(
	[this](int32 NewGoldValue)
	{
		OnGoldChange.Broadcast(NewGoldValue);
	});
	GetRG_GM()->PlayerXpChangeDelegate.AddLambda(
[this](int32 NewXpValue)
	{
		OnXpChange.Broadcast(NewXpValue);
	});

	//Buildings cards
	GetRG_GM()->BuildingCardDeselectAllDelegate.AddLambda(
	[this](FGameplayTag BuildingTag)
	{
		OnTowerSelectChange.Broadcast(BuildingTag);
	});
	GetRG_GM()->BuildingCardStatsChangeDelegate.AddLambda(
	[this](FGameplayTag BuildingTag)
	{
		OnUpdateCardInfoChange.Broadcast(BuildingTag);
	});

	//Clicked Tower
	GetRG_GM()->TowerClickedDelegate.AddLambda(
	[this](bool bHide)
	{
		OnTowerInfoDisplayChange.Broadcast(bHide);
	});

	GetRG_GM()->OnInGameUpgrade.AddLambda(
	[this]()
	{
		OnShowInGameUpgrades.Broadcast();
	});

	//Expand
	GetRG_GM()->ExpandPressedDelegate.AddLambda(
	[this]()
	{
		OnOverlayExpandDelegate.Broadcast();
	});

	//Game Over
	GetRG_GM()->GameOverDelegate.AddLambda(
	[this](bool bWon)
	{
		OnGameOverDelegate.Broadcast(bWon);
	});
}


