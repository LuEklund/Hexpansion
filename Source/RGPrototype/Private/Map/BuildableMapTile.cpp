// BuildableMapTile.cpp
#include "Map/BuildableMapTile.h"

#include "Buildings/Tower.h"
#include "Engine/World.h"
#include "Game/RGGameMode.h"

// Sets default values

ABuildableMapTile::ABuildableMapTile()
{
	PrimaryActorTick.bCanEverTick = false;
	// EnableInput(GetWorld()->GetFirstPlayerController());
	// OnClicked.AddDynamic(this, &ABuildableMapTile::BuildNotifyActorOnClicked);
	SetActorEnableCollision(true);
}

void ABuildableMapTile::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		EnableInput(PlayerController);
	}
}


void ABuildableMapTile::SpawnBase()
{
	if (SpawnedBase)
	{
		// Spawn the actor at the specified location
		FActorSpawnParameters TestParams;
		TestParams.Owner = this; // No specific owner
		TestParams.Instigator = GetInstigator(); // No specific instigator
		TestParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnedTowerPos = GetActorLocation();

		const UPrimitiveComponent* RootPrimitive = Cast<UPrimitiveComponent>(GetRootComponent());
		
		// Get the local bounds of the root component
		auto e = RootPrimitive->GetLocalBounds();
		bHasTower = true;
		SpawnedTowerPos.Z += e.Origin.Z + e.BoxExtent.Z - e.Origin.Z + (e.BoxExtent.Z);
		UE_LOG(LogTemp, Warning, TEXT("We got position successfully! %s"), *SpawnedTowerPos.ToString());
		auto SpawnedTowerInstance = GetWorld()->SpawnActor<ANexus>(SpawnedBase, SpawnedTowerPos, FRotator(0,90,0), TestParams);
		if (SpawnedTowerInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("Base spawned successfully!"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn Base!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn Base!"));
	}
}


ATower* ABuildableMapTile::BuildTower(TSubclassOf<ATower>	TowerClass)
{
	if (TowerClass && !bHasTower)
	{
		// Spawn the actor at the specified location
		FActorSpawnParameters TestParams;
		TestParams.Owner = this; // No specific owner
		TestParams.Instigator = GetInstigator(); // No specific instigator
		TestParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnedTowerPos = GetActorLocation();

		const UPrimitiveComponent* RootPrimitive = Cast<UPrimitiveComponent>(GetRootComponent());
	
		// Get the local bounds of the root component
		auto e = RootPrimitive->GetLocalBounds();

		SpawnedTowerPos.Z += e.Origin.Z + e.BoxExtent.Z - e.Origin.Z + e.BoxExtent.Z;
		UE_LOG(LogTemp, Warning, TEXT("We got position successfully! %s"), *SpawnedTowerPos.ToString());
		auto SpawnedTowerInstance = GetWorld()->SpawnActor<ATower>(TowerClass, SpawnedTowerPos, FRotator::ZeroRotator, TestParams);
		if (SpawnedTowerInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("Tower spawned successfully!"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn tower!"));
		}
		SpawnedTowerInstance->OwnerTile = this;
		bHasTower = true;
		return SpawnedTowerInstance;
	}
	return nullptr;
}
