// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/MikeTests/MapExits.h"
#include "Components/SplineComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMapExits::AMapExits()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	MySpline = CreateDefaultSubobject<USplineComponent>("Spline");
	ExitNumber = 0;
	CanExpand = true;
	TileSize = 0;
	MyCenter =  FVector(0);
	MyEntry = FVector(0);
	MyExit = FVector(0);
	MyTilePtr = nullptr;

}

void AMapExits::MakeNewPathFromOld(const AMapExits& Other)
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
	EntryNumber = Other.EntryNumber;
	MyExit = Other.MyExit;
	MyTilePtr = Other.MyTilePtr;
	ExitNumber = Other.ExitNumber;
	MyTilePtr = Other.MyTilePtr;
	AddPathSplines();
}

// Called when the game starts or when spawned
void AMapExits::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMapExits::AddPathSplines() const
{
	MySpline->ClearSplinePoints();
	size_t index = PathToBase.size();
	FString FormattedString = FString::Printf(TEXT("We have path size: %d"), index);;
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FormattedString);
	while (index > 0)
	{
		FormattedString = FString::Printf(TEXT("We put spline at: %s"), *PathToBase.at(--index).ToString());
		MySpline->AddSplinePoint(PathToBase.at(index), ESplineCoordinateSpace::World);
		
	}
}

USplineComponent* AMapExits::GetSpline() const
{
	return MySpline;
}

FVector AMapExits::PositionOfEdge(int32 EdgeNumber, const FVector& ChunkMiddle) const
{
	FVector ReturnPos(0);
	int Multipler1 = 0 , Multipler2 = 0;
	ReturnPos = ChunkMiddle;
	switch(EdgeNumber) {
	case 1:
		Multipler1 = 0;
		Multipler2 = 3;
		break;
	case 2:
		Multipler1 = 1;
		Multipler2 = 3;
		break;
	case 3:
		Multipler1 = 2;
		Multipler2 = 3;
		break;
	case 4:
		Multipler1 = 3;
		Multipler2 = 3;
		break;
	case 5:
		Multipler1 = 3;
		Multipler2 = 2;
		break;
	case 6:
		Multipler1 = 3;
		Multipler2 = 1;
		break;
	case 7:
		Multipler1 = 3;
		Multipler2 = 0;
		break;
	case 8:
		Multipler1 = 2;
		Multipler2 = -1;
		break;
	case 9:
		Multipler1 = 1;
		Multipler2 = -2;
		break;
	case 10:
		Multipler1 = 0;
		Multipler2 = -3;
		break;
	case 11:
		Multipler1 = -1;
		Multipler2 = -3;
		break;
	case 12:
		Multipler1 = -2;
		Multipler2 = -3;
		break;
	case 13:
		Multipler1 = -3;
		Multipler2 = -3;
		break;
	case 14:
		Multipler1 = -3;
		Multipler2 = -2;
		break;
	case 15:
		Multipler1 = -3;
		Multipler2 = -1;
		break;
	case 16:
		Multipler1 = -3;
		Multipler2 = 0;
		break;
	case 17:
		Multipler1 = -2;
		Multipler2 = 1;
		break;
	case 18:
		Multipler1 = -1;
		Multipler2 = 2;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Incorrect position requested: %d"), EdgeNumber);
		break;
	}
	ReturnPos.X += cos(FMath::DegreesToRadians(-60)) * TileSize * Multipler1 + cos(FMath::DegreesToRadians(180)) * TileSize * Multipler2;
	ReturnPos.Y += sin(FMath::DegreesToRadians(-60)) * TileSize * Multipler1 + sin(FMath::DegreesToRadians(180)) * TileSize * Multipler2;
	
	return ReturnPos;
}

bool AMapExits::FindNextPossiblePaths(int* Answers, const int Target)
{
	
	switch (Target)
	{
	case 1:
		Answers[0] = UPLEFT | DOWNLEFT;
		Answers[1] = 13;
		Answers[2] = 0;
		Answers[3] = 7;
		Answers[4] = 8;
		break;
	case 2:
		Answers[0] = UPLEFT;
		Answers[1] = 12;
		Answers[2] = 13;
		Answers[3] = 0;
		Answers[4] = 0;
		break;
	case 3:
		Answers[0] = UPLEFT;
		Answers[1] = 11;
		Answers[2] = 12;
		Answers[3] = 0;
		Answers[4] = 0;
		break;
	case 4:
		Answers[0] = UPLEFT | UPABOVE;
		Answers[1] = 16;
		Answers[2] = 0;
		Answers[3] = 10;
		Answers[4] = 11;
		break;
	case 5:
		Answers[0] = UPABOVE;
		Answers[1] = 15;
		Answers[2] = 16;
		Answers[3] = 0;
		Answers[4] = 0;
		break;
	case 6:
		Answers[0] = UPABOVE;
		Answers[1] = 14;
		Answers[2] = 15;
		Answers[3] = 0;
		Answers[4] = 0;
		break;
	case 7:
		Answers[0] = UPRIGHT | UPABOVE;
		Answers[1] = 1;
		Answers[2] = 0;
		Answers[3] = 13;
		Answers[4] = 14;
		break;
	case 8:
		Answers[0] = UPRIGHT;
		Answers[1] = 1;
		Answers[2] = 18;
		Answers[3] = 0;
		Answers[4] = 0;
		break;
	case 9:
		Answers[0] = UPRIGHT;
		Answers[1] = 17;
		Answers[2] = 18;
		Answers[3] = 0;
		Answers[4] = 0;
		break;
	case 10:
		Answers[0] = UPRIGHT | DOWNRIGHT;
		Answers[1] = 16;
		Answers[2] = 17;
		Answers[3] = 4;
		Answers[4] = 0;
		break;
	case 11:
		Answers[0] = DOWNRIGHT;
		Answers[1] = 3;
		Answers[2] = 4;
		Answers[3] = 0;
		Answers[4] = 0;
		break;
	case 12:
		Answers[0] = DOWNRIGHT;
		Answers[1] = 2;
		Answers[2] = 3;
		Answers[3] = 0;
		Answers[4] = 0;
		break;
	case 13:
		Answers[0] = DOWNBELOW | DOWNRIGHT;
		Answers[1] = 7;
		Answers[2] = 0;
		Answers[3] = 1;
		Answers[4] = 2;
		break;
	case 14:
		Answers[0] = DOWNBELOW;
		Answers[1] = 6;
		Answers[2] = 7;
		Answers[3] = 0;
		Answers[4] = 0;
		break;
	case 15:
		Answers[0] = DOWNBELOW;
		Answers[1] = 5;
		Answers[2] = 6;
		Answers[3] = 0;
		Answers[4] = 0;
		break;
	case 16:
		Answers[0] = DOWNLEFT | DOWNBELOW;
		Answers[1] = 10;
		Answers[2] = 0;
		Answers[3] = 4;
		Answers[4] = 5;
		break;
	case 17:
		Answers[0] = DOWNLEFT;
		Answers[1] = 9;
		Answers[2] = 10;
		Answers[3] = 0;
		Answers[4] = 0;
		break;
	case 18:
		Answers[0] = DOWNLEFT;
		Answers[1] = 8;
		Answers[2] = 9;
		Answers[3] = 0;
		Answers[4] = 0;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Incorrect position requested: %d"), Target);
		return false;
	}
	return true;
}

bool AMapExits::CheckWalkable(const FVector& Location, float Radius) const
{
	auto MyWorld = GetWorld();
	if (!MyWorld) return false;

	TArray<AActor*> OverlappingActors;
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	TArray<AActor*> ActorsToIgnore;
	
	bool bOverlap = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		Location,
		Radius,
		ObjectTypes,
		nullptr,
		ActorsToIgnore,
		OverlappingActors
	);

	if (bOverlap)
	{
		for(auto e : OverlappingActors)
		{
			UE_LOG(LogTemp, Error, TEXT("We hit this guy: %s"), *e->GetName());
		}
		return false;
	}
	return true;
}


void AMapExits::CheckExpand()
{
	
	FVector SurroundingCenters[6] = {FVector(0)};
		int angle = 0;
		
		// downRight
		SurroundingCenters[5].X = PositionOfEdge( 10, MyCenter).X + cos(FMath::DegreesToRadians(60)) * TileSize * 4 ;
		SurroundingCenters[5].Y =  PositionOfEdge( 10, MyCenter).Y + sin(FMath::DegreesToRadians(60)) * TileSize * 4 ;
		// down
		FVector ArrowDirection(0);
		SurroundingCenters[4].X = PositionOfEdge( 13, MyCenter).X + cos(FMath::DegreesToRadians(120)) * TileSize * 4 ;
		SurroundingCenters[4].Y =  PositionOfEdge( 13, MyCenter).Y + sin(FMath::DegreesToRadians(120)) * TileSize * 4 ;
		
		// downleft
		SurroundingCenters[3].X = PositionOfEdge( 16, MyCenter).X + cos(FMath::DegreesToRadians(180)) * TileSize * 4 ;
		SurroundingCenters[3].Y =  PositionOfEdge( 16, MyCenter).Y + sin(FMath::DegreesToRadians(180)) * TileSize * 4 ;
		
		
		// upLeft
		SurroundingCenters[2].X = PositionOfEdge( 1, MyCenter).X + cos(FMath::DegreesToRadians(240)) * TileSize * 4 ;
		SurroundingCenters[2].Y =  PositionOfEdge( 1, MyCenter).Y + sin(FMath::DegreesToRadians(240)) * TileSize * 4 ;
		
		// above*
		SurroundingCenters[1].X = PositionOfEdge( 4, MyCenter).X + cos(FMath::DegreesToRadians(300)) * TileSize * 4 ;
		SurroundingCenters[1].Y =  PositionOfEdge( 4, MyCenter).Y + sin(FMath::DegreesToRadians(300)) * TileSize * 4 ;

		// upRight
		SurroundingCenters[0].X = PositionOfEdge( 7, MyCenter).X + cos(FMath::DegreesToRadians(0)) * TileSize * 4 ;
		SurroundingCenters[0].Y =  PositionOfEdge( 7, MyCenter).Y + sin(FMath::DegreesToRadians(0)) * TileSize * 4 ;
		std::vector<FVector> PossibleExpands;
		int ExpandPaths[5];
		FindNextPossiblePaths(ExpandPaths, ExitNumber);
		int Possibilities = 0;
		for( int i = 0; i < 6; i++ )
		{
			if ((ExpandPaths[0] >> i) % 2)
			{
				if (CheckWalkable(SurroundingCenters[i], 1.0f))
				{
					PossibleExpands.push_back(SurroundingCenters[i]);
				}
				
				Possibilities++;
			}
		}
		if (PossibleExpands.empty())
		{
			CanExpand = false;
			//TODO: Make portal here
		}
	CanExpand = true;
}

// Called every frame
void AMapExits::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

