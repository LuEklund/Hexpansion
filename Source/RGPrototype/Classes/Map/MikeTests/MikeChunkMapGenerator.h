// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapExits.h"
#include "GameFramework/Actor.h"
#include "../RG_MapBase.h"
#include "MikeChunkMapGenerator.generated.h"

class USplineComponent;

#define UPRIGHT 0b1
#define UPABOVE 0b10
#define UPLEFT 0b100
#define DOWNLEFT 0b1000
#define DOWNBELOW 0b10000
#define DOWNRIGHT 0b100000

#define WALKABLE_TILE 1
#define BUILDABLE_TILE 2
#define PORTAL_TILE 3
#define SPLIT_TILE 4

USTRUCT()
struct FChunkDetails
{
	GENERATED_BODY()
	FVector Entry, Exit, Middle;
	int32 EntryNumber, ExitNumber;
	FChunkDetails()
	{
		Middle = FVector(0);
		Entry = FVector(0);
		Exit = FVector(0);
		EntryNumber = 0;
		ExitNumber = 0;
	}
};

UCLASS()
class RGPROTOTYPE_API AMikeChunkMapGenerator : public ARG_MapBase
{
	GENERATED_BODY()
	
public:
	const std::string TileString = (std::string)("1111\n11111\n111111\n1111111\n111111\n11111\n1111\n");
	float TileSize;
	FVector Homebase;
	bool FirstMade = false;
	UPROPERTY()
	UWorld* MyWorld;
	UPROPERTY()
	AMapExits  *NextExpandPtr;
	UPROPERTY()
	TArray<AMapExits *> AllPathEnds;
	// Sets default values for this actor's properties
	AMikeChunkMapGenerator();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USceneComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WalkableSpawn")
	TSubclassOf<AMapTile> WalkableTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PortalTileSpawn")
	TSubclassOf<AMapTile> TunnelTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SplitTileSpawn")
	TSubclassOf<AMapTile> SplitTile;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuildableSpawn")
	TSubclassOf<AMapTile> BuildableTile;
	
	virtual void Expand() override;
	virtual void GetPositions(std::vector<FVector> &InVector) override;
	virtual void Expand(FVector ExpandPoint) override;
	virtual FVector GetEndNode() override;
	virtual USplineComponent* GetSpline() override;
	void GenerateFirstChunk();
	bool CheckWalkable(const FVector& Location, float Radius) const;
	AMapTile *SpawnNewTile(int Type, const FVector& Location);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FVector PositionOfEdge(int32 EdgeNumber, const FVector& ChunkMiddle) const;
	bool MakeNewChunk(const FVector& Center, FVector Entry, const FVector& Exiter);
	bool HasNoBadNeighbors(const FVector& Position) const;
	void FillBuildables(FVector Center);
	static bool FindNextPossiblePaths(int *Answers,  const int Target);
	FVector GetARandomExit(const FVector& Middle);
	bool CheckExpandTwoWays(const FVector& MyCenter, int ExitNumber) const;
	bool CanPathExpand(const AMapExits * CheckExit) const;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
