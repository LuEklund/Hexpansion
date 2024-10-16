// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Map/RG_MapBase.h"
#include "RG_MapGenerator.generated.h"

/**
 * 
 */

#define UPRIGHT	            0b1
#define UPABOVE	           0b10
#define UPLEFT	          0b100
#define DOWNLEFT         0b1000
#define DOWNBELOW       0b10000
#define DOWNRIGHT      0b100000


#define WALKABLE_TILE 1
#define BUILDABLE_TILE 2
#define PORTAL_TILE 3
#define SPLIT_TILE 4


class AMapTile;

class SimplePerlinNoise
{
public:
	SimplePerlinNoise();

	// Generates a value between 0 and 1 based on Perlin noise
	double noise(double x, double y);

private:
	std::vector<int> p;

	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y);
};

UCLASS()
class RGPROTOTYPE_API APathHead : public  AActor
{
	GENERATED_BODY()
public:
	int		ExitNumber;
	int		EntryNumber;
	bool	CanExpand;
	float   TileSize;
	float	TileHeight;
	FVector MyCenter;
	FVector MyEntry;
	FVector MyExit;
	TObjectPtr<AMapTile> MyTilePtr;
	std::vector<FVector> PathToBase;
	TObjectPtr<USplineComponent>	MySpline;
	APathHead()
	{
		TileSize = 0.0f;
		CanExpand = false;
		MyCenter = FVector(0);
		MyEntry = FVector(0);
		MyExit = FVector(0);
		EntryNumber = 0;
		ExitNumber = 0;
		MySpline = CreateDefaultSubobject<USplineComponent>("Spline");
		MyTilePtr = nullptr;
	}
	void MakeNewPathFromOld(const APathHead& Other)
	{
		for (FVector e : Other.PathToBase)
		{
			this->PathToBase.push_back(e);
		}
		ExitNumber = Other.CanExpand;
		CanExpand = true;
		TileSize = Other.TileSize;
		MyCenter = Other.MyCenter;
		MyEntry = Other.MyEntry;
		TileHeight = Other.TileHeight;
		EntryNumber = Other.EntryNumber;
		AddPathSplines();
	}
	void AddPathSplines() const
	{
		MySpline->ClearSplinePoints();
		size_t index = PathToBase.size();
		FString FormattedString = FString::Printf(TEXT("We have path size: %d"), index);;
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FormattedString);
		while (index > 0)
		{
			FormattedString = FString::Printf(TEXT("We put spline at: %s"), *PathToBase.at(--index).ToString());
			FVector Tester = PathToBase.at(index);
			Tester.Z = TileHeight;
			MySpline->AddSplinePoint(Tester, ESplineCoordinateSpace::World);
		
		}
	};
};

enum EMyNeighbors
{
	TopRight,
	Top,
	TopLeft,
	BotLeft,
	Bot,
	BotRight,
};

UCLASS()
class RGPROTOTYPE_API ARG_MapGenerator : public ARG_MapBase
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	ARG_MapGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//Head Node Struct
	
	const std::string TileString = (std::string)("1111\n11111\n111111\n1111111\n111111\n11111\n1111\n");
	float TileSize = 0;
	FVector HomeBase = FVector(0);
	bool ShouldSplit = false;
	FVector AdjacentChunks[6] {};
	UPROPERTY()
	APathHead  *NextExpandPtr = nullptr;;
	UPROPERTY()
	TArray<APathHead *> AllPathEnds;
	UPROPERTY()
	AMapTile *HomeBaseTile = nullptr;
	bool HomeBaseBuilt = false;
	SimplePerlinNoise PerlinNoiser;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="H3Chance")
	float H3Chance = 0.10f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="H2Chance")
	float H2Chance = 0.35f;
	int32 SplitChance = 7;
	int32 MaxSplit = 4;
	float TileHeight = 0;


	//All possible different tiles
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USceneComponent* MeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WalkableSpawn")
	TSubclassOf<AMapTile> WalkableTileStraight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WalkableSpawn")
	TSubclassOf<AMapTile> WalkableTileDiagonal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WalkableSpawn")
	TSubclassOf<AMapTile> WalkableTile30Degrees;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WalkableSpawn")
	TSubclassOf<AMapTile> WalkableTile60Degrees;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PortalTileSpawn")
	TSubclassOf<AMapTile> TunnelTile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SplitTileSpawn")
	TSubclassOf<AMapTile> SplitTile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuildableSpawn")
	TSubclassOf<AMapTile> BuildableTile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuildableSpawn")
	TSubclassOf<AMapTile> BuildableTileH2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuildableSpawn")
	TSubclassOf<AMapTile> BuildableTileH3;


	int SpawnedLastTime = 0;
	//Spawn a new tile
	AMapTile *SpawnNewTile(int Type, const FVector& Location);

	AMapTile* SpawnNewWalkableTile(const FVector& Previous, const FVector& Location, const FVector& Next);

	//Given the center find the Nth edge
	FVector PositionOfNthEdge(int32 EdgeNumber, const FVector& ChunkMiddle) const;

	//Initialize 0 to 5 vectors to find the adjacent chunk of that position
	void FillAdjacentHexagonVectors();

	//checks if tile place is occupied
	bool CheckWalkable(const FVector& Location, float Radius) const;
	
	//generate the first chunk
	void GenerateFirstChunk();

	//CheckExpand for a position
	bool CheckExpand(const FVector& EndPoint);
	
	//generating the chunks
	bool MakeNewChunk(const FVector& Center, FVector Entry, const FVector& Exiter);
	bool HasNoBadNeighbors(const FVector& Position) const;
	void FillBuildables(const FVector& Center);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void Expand() override;
	static void FindNextPossiblePaths(int* Answers, int Target);
	FVector GetARandomSecondExit(const FVector& Middle, const FVector& NewExit) const;
	bool MakeNewSplit(const FVector& Center, const FVector& Entry, const FVector& NewExit, const FVector& SplitExit, int SplitExitNumber);
	virtual void Expand(FVector ExpandPoint) override;
	FVector GetARandomExit(const FVector& Middle);
	virtual void GetPositions(std::vector<FVector> &InVector) override;
	virtual TArray<USplineComponent *> GetSplines() override;
	virtual FVector GetEndNode() override;
	virtual USplineComponent* GetSpline() override;
	

	
};
