// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/MapChunkBase.h"

#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Map/MapTile.h"

// Sets default values
AMapChunkBase::AMapChunkBase()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("RootComponent"));

	BoxConnector = CreateDefaultSubobject<UBoxComponent>("BoxConnector");
	BoxConnector->SetupAttachment(GetRootComponent());

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
	Spline->SetupAttachment(GetRootComponent());
	
	int32 Hexagons = 1 + 3 * HexLayers * (HexLayers+1);
	TileMeshes.SetNum(Hexagons);
	float HexRadius = 174.205078f;  // Adjust the radius as needed

	int32	CurrentIndex = 0;
	FName Name = FName(*FString::Printf(TEXT("Mesh_%d"), CurrentIndex));
	TileMeshes[CurrentIndex] = CreateDefaultSubobject<UStaticMeshComponent>(Name);
	TileMeshes[CurrentIndex]->SetupAttachment(GetRootComponent());
	//Section = how many pathways out from the center (what x-agon)
	for (int32 Section = 0; Section < 6; ++Section)
	{
		float Angle = FMath::DegreesToRadians(60.0f * Section);
		
		for (int32 Layer = 1; Layer <= HexLayers; ++Layer)
		{
			CurrentIndex++;
			float LayerRadius = Layer * HexRadius;
			float X = LayerRadius * FMath::Cos(Angle);
			float Y = LayerRadius * FMath::Sin(Angle);
			
			Name = FName(*FString::Printf(TEXT("Mesh_%d"), CurrentIndex));
			TileMeshes[CurrentIndex] = CreateDefaultSubobject<UStaticMeshComponent>(Name);
			TileMeshes[CurrentIndex]->SetupAttachment(GetRootComponent());
			TileMeshes[CurrentIndex]->SetRelativeLocation(FVector(X, Y, 0.f));
			UStaticMeshComponent *MainMesh = TileMeshes[CurrentIndex];
			float AngleInner = FMath::DegreesToRadians((60.0f * Section) + 120.f);
			for (int32 SideExpand = 0; SideExpand < Layer - 1; SideExpand++)
			{
				CurrentIndex++;
				X = (SideExpand + 1) * HexRadius * FMath::Cos(AngleInner);
				Y = (SideExpand + 1) * HexRadius * FMath::Sin(AngleInner);
				Name = FName(*FString::Printf(TEXT("Mesh_%d"), CurrentIndex));
				TileMeshes[CurrentIndex] = CreateDefaultSubobject<UStaticMeshComponent>(Name);
				TileMeshes[CurrentIndex]->SetupAttachment(GetRootComponent());
				TileMeshes[CurrentIndex]->SetRelativeLocation(FVector(MainMesh->GetRelativeLocation().X + X,MainMesh->GetRelativeLocation().Y + Y, 0.f));
			}
		}
	}
}

void AMapChunkBase::RotateChunk(FRotator Rotation)
{
	// Rotate AMapChunkBase
	SetActorRotation(Rotation);

	// if (!MapTiles.Num())
	// {
	// 	SpawnPoints();
	// }




	

	// Update MapTiles positions relative to the rotated AMapChunkBase
	// for (AMapTile* Tile : MapTiles)
	// {
	// 	// Check if Tile is valid before attempting to update its position
	// 	if (Tile)
	// 	{
	// 		// Calculate the new position relative to AMapChunkBase's origin
	// 		FVector RelativePosition = Tile->GetActorLocation() - GetActorLocation();
	// 		RelativePosition = Rotation.RotateVector(RelativePosition);
	// 		Tile->SetActorLocation(GetActorLocation() + RelativePosition);
	// 	}
	// 	else
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("Invalid AMapTile found in MapTiles."));
	// 	}
	// }
}

// Called when the game starts or when spawned
void AMapChunkBase::BeginPlay()
{
	Super::BeginPlay();
	// UE_LOG(LogTemp, Error, TEXT("RootPos2 %s"), *GetRootComponent()->GetRelativeLocation().ToString());

	

}

// Called every frame
void AMapChunkBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AMapTile* AMapChunkBase::SpawnMapTile(float X, float Y, int32 Index)
{
	FActorSpawnParameters TestParams;
	TestParams.Owner = this; // No specific owner
	TestParams.Instigator = GetInstigator(); // No specific instigator
	TestParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AMapTile *MapTile = nullptr;
	if (TileMeshes[Index]->GetMaterial(0) == PathMaterial)
	{
		MapTile = GetWorld()->SpawnActor<AMapTile>(WalkableTile, FVector(X,Y,0.f), FRotator::ZeroRotator, TestParams);
	}
	else
	{
		MapTile = GetWorld()->SpawnActor<AMapTile>(BuildableTile, FVector(X,Y,0.f), FRotator::ZeroRotator, TestParams);
	}
	MapTiles.Add(MapTile);

	return (MapTile);
}

void AMapChunkBase::SpawnPoints()
{
	// MapTiles.SetNum(37);

	float HexRadius = 174.205078f;  // Adjust the radius as needed

	//Section = how many pathways out from the center (what x-agon)
	for (int32 i = 0; i < TileMeshes.Num(); ++i)
	{
		SpawnMapTile(TileMeshes[i]->GetComponentLocation().X,TileMeshes[i]->GetComponentLocation().Y, i);
	}
	// for (int32 Section = 0; Section < 6; ++Section)
	// {
	// 	float Angle = FMath::DegreesToRadians(60.0f * Section);
	// 	
	// 	for (int32 Layer = 1; Layer <= HexLayers; ++Layer)
	// 	{
	// 		CurrentIndex++;
	// 		float LayerRadius = Layer * HexRadius;
	// 		float X = LayerRadius * FMath::Cos(Angle);
	// 		float Y = LayerRadius * FMath::Sin(Angle);
	// 		AMapTile *MainTile = SpawnMapTile(GetActorLocation().X + X,GetActorLocation().Y + Y, CurrentIndex);
	// 		float AngleInner = FMath::DegreesToRadians((60.0f * Section) + 120.f);
	// 		for (int32 SideExpand = 0; SideExpand < Layer - 1; SideExpand++)
	// 		{
	// 			CurrentIndex++;
	// 			X = (SideExpand + 1) * HexRadius * FMath::Cos(AngleInner);
	// 			Y = (SideExpand + 1) * HexRadius * FMath::Sin(AngleInner);
	// 			SpawnMapTile(MainTile->GetActorLocation().X + X,MainTile->GetActorLocation().Y + Y, CurrentIndex);
	// 		}
	// 	}
	// }

	
	// Loop through the array and destroy the components
	for (UStaticMeshComponent* MeshComponent : TileMeshes)
	{
		if (MeshComponent)
		{
			MeshComponent->DestroyComponent();
		}
	}
	
	// Clear the array
	TileMeshes.Empty();
}

