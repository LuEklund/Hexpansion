// BuildableMapTile.h
#pragma once

#include "CoreMinimal.h"
#include "MapTile.h"
#include "Buildings/Nexus.h"
#include "BuildableMapTile.generated.h"

struct FKey;
class ATower;

UCLASS()
class RGPROTOTYPE_API ABuildableMapTile : public AMapTile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuildableMapTile();
	bool bHasTower = false;

	// Function to spawn the actor
	// UFUNCTION(BlueprintCallable)
	// void SpawnTower();
	// void SpawnSlowTower();
	void SpawnBase();
	
	// The class of the actor to spawn
	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<ATower> SpawnedTower;

	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<ATower> SpawnedSlowTower;
	
	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<ANexus> SpawnedBase;
	
	// The spawn location
	UPROPERTY(EditAnywhere, Category = "Spawner")
	FVector SpawnedTowerPos;

	UPROPERTY(EditAnywhere, Category = "TileHeight")
	float Height = 0;

protected:
	virtual void BeginPlay() override;

public:
	// Called when the actor is clicked
	UFUNCTION()
	ATower* BuildTower(TSubclassOf<ATower> TowerClass);
};
