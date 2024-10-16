#include "Map/MapTile.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMapTile::AMapTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MapTilerMesh"));
	SetRootComponent(MeshComponent);

	// Initialize SideLength to a default value
	SideLength = 0.0f;
}

// Called when the game starts or when spawned
void AMapTile::BeginPlay()
{
	Super::BeginPlay();
    
}

void AMapTile::CalculateSideLength()
{
	if (MeshComponent)
	{
		UStaticMesh* StaticMesh = MeshComponent->GetStaticMesh();
		
		if (MeshComponent->GetStaticMesh())
		{
			FVector Min,Max;
			MeshComponent->GetLocalBounds(Min, Max);
			SideLength = Max.X - Min.X;
			// GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Black, FString::Printf(TEXT("SideLength: %f"), SideLength));

		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, TEXT("StaticMesh is not set properly."));
			UE_LOG(LogTemp, Error, TEXT("MeshComponent or StaticMesh is not set properly."));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, TEXT("MeshComponent is not set properly."));
		UE_LOG(LogTemp, Error, TEXT("MeshComponent or StaticMesh is not set properly."));
	}
}

void AMapTile::CalculateHeight()
{
	if(MeshComponent)
	{
		const UPrimitiveComponent* RootPrimitive = Cast<UPrimitiveComponent>(GetRootComponent());
		
		// Get the local bounds of the root component
		auto e = RootPrimitive->GetLocalBounds();
		TileHeight =  e.Origin.Z + e.BoxExtent.Z - e.Origin.Z + (e.BoxExtent.Z);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, TEXT("MeshComponent is not set properly."));
		UE_LOG(LogTemp, Error, TEXT("MeshComponent or StaticMesh is not set properly."));
	}
}
