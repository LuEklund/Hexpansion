// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Map/MapTile.h"
#include "MapExits.generated.h"

class USplineComponent;

#define UPRIGHT 0b1
#define UPABOVE 0b10
#define UPLEFT 0b100
#define DOWNLEFT 0b1000
#define DOWNBELOW 0b10000
#define DOWNRIGHT 0b100000

UCLASS()
class RGPROTOTYPE_API AMapExits : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapExits();
	int		ExitNumber;
	int		EntryNumber;
	bool	CanExpand;
	float TileSize;
	FVector MyCenter;
	FVector MyEntry;
	FVector MyExit;
	UPROPERTY()
	TObjectPtr<AMapTile> MyTilePtr;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent>	MySpline;
	std::vector<FVector> PathToBase;
	void MakeNewPathFromOld(const AMapExits& Other);
	virtual void BeginPlay() override;
	void AddPathSplines() const;
	USplineComponent* GetSpline() const;
	void CheckExpand();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FVector PositionOfEdge(int32 EdgeNumber, const FVector& ChunkMiddle) const;
	static bool FindNextPossiblePaths(int *Answers,  const int Target);
	bool CheckWalkable(const FVector& Location, float Radius) const;
protected:

};
