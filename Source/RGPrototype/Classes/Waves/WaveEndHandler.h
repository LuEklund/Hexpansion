// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveEndHandler.generated.h"

UCLASS()
class RGPROTOTYPE_API AWaveEndHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaveEndHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Adds xp to game instance and resets the game when survived MaxWaves
	void GainXp();
	void HandleVictory();

	// After surviving MaxWaves you pass the difficulty
	int32 MaxWaves = 20;
	int32 CurrentWave = 0;

	// Every X waves you get a in-game upgrade card to choose
	int32 InGameUpgradeFrequency = 3;
};
