// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EndGameData.generated.h"

UCLASS()
class RGPROTOTYPE_API AEndGameData : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEndGameData();

	float TotalDamageDealt = 0.0f;
	float TotalDamageByArrow = 0.0f;
	float TotalDamageBySniper = 0.0f;
	float TotalDamageByAoE = 0.0f;
	
	float TotalEnemiesKilled = 0.0f;

	float TotalTowersPlaced = 0.0f;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
