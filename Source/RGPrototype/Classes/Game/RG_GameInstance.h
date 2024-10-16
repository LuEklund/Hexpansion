// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interface/SaveInterface.h"
#include "RG_GameInstance.generated.h"

struct FInGameUpgradeRow;
class USaveGame;
class URGSaveGame;
class UInGameUpgradesData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnXPChange);


/**
 * 
 */
UCLASS()
class RGPROTOTYPE_API URG_GameInstance : public UGameInstance, public ISaveInterface
{
	GENERATED_BODY()
public:
	virtual void Init() override;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Stats")
	FOnXPChange	OnXPChangeDelegate;
	
	UPROPERTY()
	TObjectPtr<UAudioComponent>	MusicAudioComponent;

	UFUNCTION(BlueprintCallable)
	void StopMusic();
	
	UFUNCTION(BlueprintCallable)
	void	PlayMusic(USoundBase* Music);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInGameUpgradesData>	InGameUpgradesData;

	UFUNCTION(BlueprintCallable)
	FGameplayTag	UnlockInGameUpgrade(FName Name);

	//Save game
	UPROPERTY()
	TObjectPtr<URGSaveGame>	SaveGame;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame>	SaveGameClass;
	UPROPERTY(EditDefaultsOnly)
	FString	SaveSlotName = "Default";
	void OnLoadGameFromSlotComplete(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGame);
	//Save Game Interface Start
	virtual void LoadGameData(bool bAsync) override;
	virtual void SaveGameData(bool bAsync) override;
	virtual URGSaveGame* GetGameData() override;
	
	virtual void SavePlayerStats(FPlayerStats Stats, bool bAsync) override;
	virtual void SavePermaUpgrades(bool bAsync) override;
	//Save Game Interface End

	virtual void Shutdown() override;

	UPROPERTY(BlueprintReadWrite)
	int32	StartingBonusGoldMultiplier = 1;

	UPROPERTY(BlueprintReadWrite)
	int32	PlayerXp = 100;

	UPROPERTY(BlueprintReadWrite)
	float	DifficultyScale = 1.f;
};
