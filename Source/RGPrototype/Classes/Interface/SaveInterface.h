// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/Interface.h"
#include "SaveInterface.generated.h"


struct FPlayerStats;
class URGSaveGame;



// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USaveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RGPROTOTYPE_API ISaveInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	FAsyncLoadGameFromSlotDelegate	AsyncLoadGameFromSlotDelegate;
	
	virtual void LoadGameData(bool bAsync) = 0;
	virtual void SaveGameData(bool bAsync) = 0;
	virtual URGSaveGame* GetGameData() = 0;

	//Save Functions
	virtual void SavePlayerStats(FPlayerStats Stats, bool bAsync) = 0;
	virtual void SavePermaUpgrades(bool bAsync) = 0;
};
