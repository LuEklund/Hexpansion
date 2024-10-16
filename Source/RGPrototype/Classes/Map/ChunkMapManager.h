// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChunkMapManager.generated.h"

USTRUCT()
struct FChunkInfo
{
	GENERATED_BODY()

	bool	bTaken = false;
	float	Degree = 0.f;
};

USTRUCT()
struct FYAxisChunk
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<int32, FChunkInfo>	YAxis;
};

UCLASS()
class RGPROTOTYPE_API AChunkMapManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunkMapManager();

	UPROPERTY()
	TMap<int32, FYAxisChunk>	MapContainer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
