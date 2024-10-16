// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapChunkBase.h"
#include "RG_MapBase.h"
#include "GameFramework/Actor.h"
#include "ChunkMapGenerator.generated.h"

UCLASS()
class RGPROTOTYPE_API AChunkMapGenerator : public ARG_MapBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunkMapGenerator();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMapChunkBase> ChunkStartEnd;
	
	UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AMapChunkBase> ChunkEmpty;


	// //0 Degree
	// UPROPERTY(EditDefaultsOnly)
	// TArray<TSubclassOf<AMapChunkBase>>	Chunks0DegreeClasses;
	//60 Degree
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AMapChunkBase>>	Chunks60DegreeClasses;
	//120 Degree
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AMapChunkBase>>	Chunks120DegreeClasses;
	//180 Degree
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AMapChunkBase>>	Chunks180DegreeClasses;
	//240 Degree
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AMapChunkBase>>	Chunks240DegreeClasses;
	//300 Degree
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AMapChunkBase>>	Chunks300DegreeClasses;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AMapChunkBase>>	ChunksClasses;


	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AMapChunkBase>>	Chunks;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32	MaxChunkID;

	
	FVector Head;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//Base class Overwrite
	UFUNCTION(BlueprintCallable)
	virtual void Expand() override;
	virtual FVector GetEndNode() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	float HexRadius = 174.205078f;  // Adjust the radius as needed
	bool	bNewChunk = false;

	TSubclassOf<AMapChunkBase> GetOptimalChunkClass(TArray<int32> Array);
	void GetLeftRightDirections(FVector& LeftDirection, FVector& RightDirection, FVector& FromPosition);
	bool CanSpawnAt(const FVector& Location);
	
	bool RotateChunk(FVector SpawnLocation);
	AMapChunkBase* SpawnChunkAtLocation(TSubclassOf<AMapChunkBase> CunkClass, const FVector& Location);

	//NEW SPAWN CHUNK
	TArray<int32> EvaluateExpandChunk(FVector ChunkOrigin, FVector ForwardVector);



};
