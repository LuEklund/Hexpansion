// Fill out your copyright notice in the Description page of Project Settings.


#include "Waves/WaveEndHandler.h"

#include "Game/RG_GameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWaveEndHandler::AWaveEndHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWaveEndHandler::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWaveEndHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaveEndHandler::GainXp()
{
	GetGameInstance<URG_GameInstance>()->PlayerXp += CurrentWave + 1;
	UE_LOG(LogTemp, Error, TEXT("Current XP: %d"), GetGameInstance<URG_GameInstance>()->PlayerXp);
}

void AWaveEndHandler::HandleVictory()
{
	if (GetWorld())
	{
		CurrentWave = 0;
		// Get the full map name, including path
		FString CurrentLevelName = GetWorld()->GetMapName();
		// Remove the path prefix to get just the level name
		CurrentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
        
		// Reload the current level
		UGameplayStatics::OpenLevel(GetWorld(), FName(*CurrentLevelName));
	}
}


