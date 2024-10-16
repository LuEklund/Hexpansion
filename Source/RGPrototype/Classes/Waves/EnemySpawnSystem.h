// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "data/EnemySpawnerData.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnSystem.generated.h"

class UEnemySpawnerData;
class AEnemyBase;


UCLASS()
class RGPROTOTYPE_API AEnemySpawnSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawnSystem();
	virtual void Tick(float DeltaTime) override;

	int32	EnemiesAlive = 0;

	UPROPERTY(EditDefaultsOnly)
	float	Scaling = 0.25f;
	
	UPROPERTY(EditDefaultsOnly, Category="Timer|a")
	float			SpawnDelay = 0.2f;
	UPROPERTY(EditDefaultsOnly, Category="Timer|a")
	float			SpawnDelayVariation = 0.2f;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UEnemySpawnerData>	EnemySpawnerData;
	
	UFUNCTION()
	void	SpawnWave();
	void SetRandomSpawnTimer();
	void	SpawnNextEnemy();
	
	FTimerHandle	SpawnTimerHandle;
	TArray<FGroup>	EnemyGroups;
	
	UPROPERTY(BlueprintReadOnly)
	int32	WaveID = 0;
protected:
	virtual void BeginPlay() override;

private:
	

	int32	CurrentSquadIndex = 0;
	int32	CurrentEnemyIndex = 0;
};
