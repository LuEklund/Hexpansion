// Fill out your copyright notice in the Description page of Project Settings.


#include "Waves/EnemySpawnSystem.h"

#include "data/EnemySpawnerData.h"
#include "Enemy/EnemyBase.h"
#include "Game/RGGameMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Map/RG_MapBase.h"

AEnemySpawnSystem::AEnemySpawnSystem()
{
	PrimaryActorTick.bCanEverTick = true;
}


void AEnemySpawnSystem::SpawnWave()
{
	if (EnemySpawnerData->Waves.Num() <= 0) return;

	ARGGameMode *GameMode = GetWorld()->GetAuthGameMode<ARGGameMode>();
	GameMode->EnemyWaveSpawnAmount(0);
	
	//Calculate amount of enemies that will be spawned
	int32 Index = WaveID < EnemySpawnerData->Waves.Num() ? WaveID : EnemySpawnerData->Waves.Num() - 1;
	FWave& Wave = EnemySpawnerData->Waves[Index];
	// TArray<USplineComponent *> Splines = GameMode->MapSpawner->GetSplines();
	// int32 Spawns = Splines.Num();
	if (GameMode->PreviousUpgradeType == EUpgradeType::BOSS)
	{
		UE_LOG(LogTemp, Warning, TEXT("BOSS!!"));
		FGroup BossGroup;
		BossGroup.Amount = 1;
		BossGroup.EnemyClass = EnemySpawnerData->Boss;
		EnemyGroups.Add(BossGroup);
		EnemiesAlive += 1;

	}
	for (const FGroup &Squad :  Wave.EnemyGroups)
	{
		// UE_LOG(LogTemp, Error, TEXT("WAVE IDDDDDDDDDD: %d"), WaveID);
		if (Squad.Amount > 0)
			EnemyGroups.Add(Squad);
		EnemiesAlive += Squad.Amount;
	}

	
	// Reset indices for new wave
	// CurrentSquadIndex = 0;
	// CurrentEnemyIndex = 0;
	
	SetRandomSpawnTimer();
}

void AEnemySpawnSystem::SetRandomSpawnTimer()
{
	// Generate a random delay between 1 and 2 seconds
	// UE_LOG(LogTemp, Error, TEXT("VALUE: %f"), SpawnDelayVariation);
	float rand = UKismetMathLibrary::RandomFloatInRange(-SpawnDelayVariation, SpawnDelayVariation);
	// UE_LOG(LogTemp, Error, TEXT("rand: %f"), rand);
	float RandomTimeOffset = SpawnDelay + rand;
	// UE_LOG(LogTemp, Error, TEXT("randVal: %f"), RandomTimeOffset);
	if (RandomTimeOffset <= 0.01f) RandomTimeOffset = 0.01f;
	// Set the timer with the random delay
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawnSystem::SpawnNextEnemy, RandomTimeOffset, false);
}

void AEnemySpawnSystem::SpawnNextEnemy()
{
	if (EnemySpawnerData->Waves.Num() <= 0) return;

	//Done spawning
	if (EnemyGroups.IsEmpty()) 
	{
		// Wave finished, clear timer
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
		++WaveID;
		// if (++WaveID <= EnemySpawnerData->Waves.Num())
		return;
	}
	
	int32 RandomIndex = std::rand() % EnemyGroups.Num();
	// UE_LOG(LogTemp, Error, TEXT("RANDOM INDEX: %d"), RandomIndex);
	if (EnemyGroups[RandomIndex].Amount == 0)
		EnemyGroups.RemoveAt(RandomIndex);
	else
	{
		ARGGameMode *GameMode = GetWorld()->GetAuthGameMode<ARGGameMode>();
		// TArray<USplineComponent *> Splines = GameMode->MapSpawner->GetSplines();
		// for (int32 i = 0; i < Splines.Num(); ++i)
		// {
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AEnemyBase* Enemy = GetWorld()->SpawnActor<AEnemyBase>(EnemyGroups[RandomIndex].EnemyClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
			Enemy->SetUpEnemy(GameMode->MapSpawner->GetSpline());
			Enemy->MaxHealth *= pow(1 + Scaling , WaveID);
			// UE_LOG(LogTemp, Error, TEXT("Health: %f, WaveID: %d, Scaling: %f, = POWER: %f"), Enemy->MaxHealth, WaveID, Scaling, pow(1 + Scaling , WaveID));
			Enemy->Health = Enemy->MaxHealth;
		// }
		
		EnemyGroups[RandomIndex].Amount -= 1;
		if (EnemyGroups[RandomIndex].Amount == 0)
			EnemyGroups.RemoveAt(RandomIndex);
	}
	SetRandomSpawnTimer();
}


void AEnemySpawnSystem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemySpawnSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

