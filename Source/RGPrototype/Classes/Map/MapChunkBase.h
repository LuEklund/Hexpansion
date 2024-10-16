// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapChunkBase.generated.h"

class UBoxComponent;
class USplineComponent;
class AMapTile;

UCLASS()
class RGPROTOTYPE_API AMapChunkBase : public AActor
{
	GENERATED_BODY()


	
public:	
	// Sets default values for this actor's properties
	AMapChunkBase();

	void	RotateChunk(FRotator Rotation);
	
	//Layer of hex Chunk; TotalHexagons=1+3n(n+1)
	UPROPERTY()
	int32 HexLayers = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ChunkID = 0;

	//Blueprint editor setup
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance>	PathMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance>	BaseMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UStaticMeshComponent>>	TileMeshes;

	//Register Path
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent>	BoxConnector;
	
	//Spline Path for chunk
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USplineComponent>	Spline;

	//The actual tiles array
	UPROPERTY()
	TArray<AMapTile *> MapTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WalkableSpawn")
	TSubclassOf<AMapTile> WalkableTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuildableSpawn")
	TSubclassOf<AMapTile> BuildableTile;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SpawnPoints();



private:
	AMapTile* SpawnMapTile(float X, float Y, int32 Index);

};
