// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/ChunkMapManager.h"

// Sets default values
AChunkMapManager::AChunkMapManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChunkMapManager::BeginPlay()
{
	Super::BeginPlay();

	for (int32 x = -5; x <= 5; ++x)
	{
		FYAxisChunk YAxisChunk; // Create a new FYAxisChunk for this x

		for (int32 y = -5; y <= 5; ++y)
		{
			FChunkInfo ChunkInfo; // Initialize FChunkInfo with default values
			YAxisChunk.YAxis.Add(y, ChunkInfo); // Add the chunk info to the Y axis map
		}
		MapContainer.Add(x, YAxisChunk); // Add the Y axis chunk to the map container
	}
	UE_LOG(LogTemp, Error, TEXT("Size :%d"), MapContainer.Num());
}

// Called every frame
void AChunkMapManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

