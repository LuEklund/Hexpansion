// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "data/BuildingData.h"
#include "data/InGameUpgradesData.h"
#include "GameFramework/GameMode.h"
#include "Brushes/SlateImageBrush.h"
#include "Player/RGPlayerController.h"
#include "RGGameMode.generated.h"

class UUpgradeManager;
class UBuildingData;
class ARG_MapBase;
class AChunkMapGenerator;
class ARG_GameState;
class ARGExpandButton;
class AMapTileSpawner;
class AEnemySpawnSystem;
class AWaveEndHandler;

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStatsValueChange, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCardStatsChange, FGameplayTag);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTowerClicked, bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameOverGM, bool);
DECLARE_MULTICAST_DELEGATE(FOnUpgrade);
DECLARE_MULTICAST_DELEGATE(FOnExpandPressed);

UENUM(BlueprintType)
enum class EUpgradeType : uint8
{
	UTILITY_ATTACK_RANGE UMETA(DisplayName = "Utility_attack_range"),
	UTILITY_ATTACK_SPEED UMETA(DisplayName = "Utility_attack_speed"),
	DAMAGE UMETA(DisplayName = "Damage"),
	BOSS UMETA(DisplayName = "Boss"),
	SPECIAL_EFFECT UMETA(DisplayName = "Special_effect"),
	GOLD UMETA(DisplayName = "Gold")
};

UCLASS()
class RGPROTOTYPE_API ARGGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	ARGGameMode();
	virtual void BeginPlay() override;

public:
	//Upgrades
	UPROPERTY(BlueprintReadOnly)
	EUpgradeType PreviousUpgradeType;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UUpgradeManager>	UpgradeManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brushes")
	FSlateBrush BrushGold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brushes")
	FSlateBrush BrushAttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brushes")
	FSlateBrush BrushAttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brushes")
	FSlateBrush SpecialEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brushes")
	FSlateBrush BrushBoss;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brushes")
	FSlateBrush BrushDamage;


	//EnemySpawner
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemySpawnSystem> 	EnemySpawnSystemClass;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AEnemySpawnSystem>	EnemySpawnSystem;

	//Map
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARG_MapBase> 	MapSpawnerClass;
	UPROPERTY()
	TObjectPtr<ARG_MapBase>		MapSpawner;
	FOnExpandPressed			ExpandPressedDelegate;

	//GameState
	UPROPERTY()
	TObjectPtr<ARG_GameState>		RGGameState;

	//Music
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase>	Music;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	TSoftObjectPtr<UWorld> LevelToLoad;

	//Enemies
	void				EnemyDeath();
	void				EnemyWaveSpawnAmount(int32 InAmount);
	FOnStatsValueChange  	EnemyDeathsDelegate;
	FOnStatsValueChange  	WaveStartDelegate;

	//WaveEndHandler
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWaveEndHandler> 	WaveEndHandlerClass;
	UPROPERTY()
	TObjectPtr<AWaveEndHandler> WaveEndHandler;

	//PLayer Stats
	void				PlayerTakeDamage(int32 InDamage);
	int32				PlayerHealth = 20;
	FOnStatsValueChange		PlayerHealthChangeDelegate;

	//Building Info
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UBuildingData>	BuildingData;
	FOnCardStatsChange			BuildingCardStatsChangeDelegate;
	FOnCardStatsChange			BuildingCardDeselectAllDelegate;
	void						UpdateBuildingMenu(FGameplayTag BuildType);

	//In Game Upgrades
	FOnUpgrade						OnInGameUpgrade;

	//GAME
	FOnGameOverGM					GameOverDelegate;
	
	//Selected Building
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ATower>			CurrentSelectedTower;
	FOnTowerClicked				TowerClickedDelegate;
	
	UFUNCTION(BlueprintCallable)
	FBuilding& GetBuildingInfo(FGameplayTag type);
	
	void				GainGold(int32 Gold);
	bool				RequestGold(int32 Gold);
	bool				CanAfford(FGameplayTag Build);

	UPROPERTY(BlueprintReadOnly)
	int32				PlayerGold = 1000;
	float				PlayerGoldMultiplier = 1.f;
	FOnStatsValueChange  PlayerGoldChangeDelegate;

	UPROPERTY(BlueprintReadOnly)
	int32	PlayerXp;
	float	HexWidth = 0;
	FOnStatsValueChange  PlayerXpChangeDelegate;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARGExpandButton>	ExpandButtonClass;
	UPROPERTY()
	TArray<ARGExpandButton *> ExpandButtons;

	
	void ShowExpandButton();
	void AddExpandButtons();
	void RemoveButtons();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase>	StartSound;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase>	ExpandSound;
	int32 GetTowerCount();
	UFUNCTION()
	void	Expand(UPrimitiveComponent* Component, FKey Key);

	UFUNCTION(BlueprintCallable)
	void Upgrade(UPARAM(ref) FInGameUpgradeRow &UpgradeInfo);
	

	
	
};
