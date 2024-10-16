// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/BuildActor.h"

#include "Components/DecalComponent.h"
#include "Components/WidgetComponent.h"
#include "Game/RGGameMode.h"
#include "Map/BuildableMapTile.h"

// Sets default values
ABuildActor::ABuildActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(MeshComponent);
	// MeshComponent->SetupAttachment(GetRootComponent());

	AttackRangeCircle = CreateDefaultSubobject<UDecalComponent>("AttackRangeCircle");
	AttackRangeCircle->SetupAttachment(MeshComponent);
	
	TextWidget = CreateDefaultSubobject<UWidgetComponent>("Text Widget");
	TextWidget->SetupAttachment(MeshComponent);

}

// Called when the game starts or when spawned
void ABuildActor::BeginPlay()
{
	Super::BeginPlay();
	RGGameMode = GetWorld()->GetAuthGameMode<ARGGameMode>();
	AttackRangeCircle->SetRelativeScale3D(GetActorScale3D() * .45f);
}

void ABuildActor::SetDisplayAttackRange(float InAttackRange)
{
	AttackRangeCircle->SetRelativeScale3D(FVector(0.42f) * (InAttackRange / 100.f));
}

void ABuildActor::Update(const FHitResult& HitActor, FGameplayTag BuildType)
{
	TextWidget->SetVisibility(true);

	ABuildableMapTile *Tile = Cast<ABuildableMapTile>(HitActor.GetActor());
	if (Tile && RGGameMode->CanAfford(BuildType) && Tile->bHasTower == false)
	{
		MeshComponent->SetMaterial(0, Available);
	}
	else
	{
		MeshComponent->SetMaterial(0, Unavailable);
	}
	FVector Min, Max;
	Tile->MeshComponent->GetLocalBounds(Min, Max);
	float Height = Max.Z - Min.Z;
	
	SetDisplayAttackRange(RGGameMode->GetBuildingInfo(BuildType).AttackRange + ((int32)Tile->Height) * RGGameMode->HexWidth);
	// UE_LOG(LogTemp, Error, TEXT("Range: %f"), RGGameMode->GetBuildingInfo(BuildType).AttackRange + ((int32)Tile->Height) * RGGameMode->HexWidth);

	// Create combined FString with "Number" and Height
	if (Tile->Height)
	{
		FString CombinedString = FString::Printf(TEXT("+%d Range"), (int32)Tile->Height);
		SetExtraHeigthText(FText::FromString(CombinedString));
	}
	else
	{
		FString CombinedString = FString::Printf(TEXT(""));
		SetExtraHeigthText(FText::FromString(CombinedString));
	}
	
	// UE_LOG(LogTemp, Error, TEXT("Hieght %f"), Height);
	SetActorLocation(Tile->GetActorLocation() + FVector(0.f,0.f, Height));
}


