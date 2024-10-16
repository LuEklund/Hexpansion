// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/FinalMapGenerator/RG_MapGenerator.h"

#include <numeric>
#include <random>

#include "Game/RGGameMode.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Map/BuildableMapTile.h"
#include "Map/MapTile.h"

ARG_MapGenerator::ARG_MapGenerator()
{
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MapGenerator"));
	RootComponent = MeshComponent;
}

void ARG_MapGenerator::BeginPlay()
{
	Super::BeginPlay();
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	GenerateFirstChunk();
	for (int i = 0; i < 50; i++)
	{
		if (AllPathEnds[0]->CanExpand)
			Expand(AllPathEnds[0]->MyExit);
	}
	GetWorld()->GetAuthGameMode<ARGGameMode>()->HexWidth = TileSize;
}

AMapTile* ARG_MapGenerator::SpawnNewTile(int Type, const FVector& Location)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	if(Type == BUILDABLE_TILE)
	{
		// Generate a Perlin noise value
		double NoiseValue = PerlinNoiser.noise(Location.X * 0.1, Location.Y * 0.1); // Scale the location for noise

		// Determine the type of tile to spawn based on noise value
		if (NoiseValue < H3Chance) // 15% for BuildableTileH3
			return GetWorld()->SpawnActor<AMapTile>(BuildableTileH3, Location, FRotator(0), SpawnParams);
		if (NoiseValue < H3Chance + H2Chance) // 25% for BuildableTileH2 (15% + 25% = 40%)
			return GetWorld()->SpawnActor<AMapTile>(BuildableTileH2, Location, FRotator(0), SpawnParams);
		// The rest (60%) for BuildableTile
		return GetWorld()->SpawnActor<AMapTile>(BuildableTile, Location, FRotator(0), SpawnParams);
	}
	if(Type == WALKABLE_TILE)
	{
		return GetWorld()->SpawnActor<AMapTile>(WalkableTileStraight, Location, FRotator(0), SpawnParams);
	}
	if(Type == PORTAL_TILE)
	{
		return GetWorld()->SpawnActor<AMapTile>(TunnelTile, Location, FRotator(0), SpawnParams);
	}
	if(Type == SPLIT_TILE)
	{
		return GetWorld()->SpawnActor<AMapTile>(SplitTile, Location, FRotator(0), SpawnParams);
	}
	return nullptr;
}

float CalculateRotator(int Entry, int Exit)
{
	// int First = (Entry << 4) + Exit;
	UE_LOG(LogTemp, Error, TEXT("Entr: %d, Exist: %d"), Entry, Exit);
	switch (Entry)
	{
		case 1:
			switch (Exit)
			{
				case 1:
					return 0;
				case 2:
					return 0;
				case 3:
					return 60;
				case 4:
					return 60;
				case 5:
					return 300;
				case 6:
					return 300;
				default:
					return 0;
			}
		case 2:
			switch (Exit)
			{
				case 1:
					return 0;
				case 2:
					return 60;
				case 3:
					return 0;
				case 4:
					return 120;
				case 5:
					return 120;
				case 6:
					return 0;
				default:
					return 0;
			}			
		case 3:
			switch (Exit)
			{
				case 1:
					return 60;
				case 2:
					return 60;
				case 3:
					return 0;
				case 4:
					return 120;
				case 5:
					return 180;
				case 6:
					return 0;
				default:
					return 0;
			}
		case 4:
			switch (Exit)
			{
				case 1:
					return 60;
				case 2:
					return 120;
				case 3:
					return 120;
				case 4:
					return 0;
				case 5:
					return 180;
				case 6:
					return 240;
				default:
					return 0;
			}
		case 5:
			switch (Exit)
			{
				case 1:
					return 300;
				case 2:
					return 120;
				case 3:
					return 180;
				case 4:
					return 180;
				case 5:
					return 0;
				case 6:
					return 240;
				default:
					return 0;
			}
		case 6:
			switch (Exit)
			{
				case 1:
					return 300;
				case 2:
					return 0;
				case 3:
					return 0;
				case 4:
					return 240;
				case 5:
					return 2400;
				case 6:
					return 0;
				default:
					return 0;
			}
		default:
			return 0;
	}
	return 0;
}

AMapTile* ARG_MapGenerator::SpawnNewWalkableTile(const FVector& Previous, const FVector& Location, const FVector& Next)
{
	int Entry = 0, Exit = 0;
	FVector Tester(0);
	for (int i  = 0; i < 6; i++)
	{
		Tester.X = Location.X + cos(FMath::DegreesToRadians(60 * i)) * TileSize;
		Tester.Y = Location.Y + sin(FMath::DegreesToRadians(60 * i)) * TileSize;
		if (FVector::Dist(Tester, Previous) < 1.0f)
			Entry = i + 1;
	}
	for (int i  = 0; i < 6; i++)
	{
		Tester.X = Location.X + cos(FMath::DegreesToRadians(60 * i)) * TileSize;
		Tester.Y = Location.Y + sin(FMath::DegreesToRadians(60 * i)) * TileSize;
		if (FVector::Dist(Tester, Next) < 1.0f)
			Exit = i + 1;
	}
	int Angle = abs(Entry - Exit);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	FRotator MyRotator(0, CalculateRotator(Entry, Exit), 0);
	switch (Angle)
	{
		case  1:
			return GetWorld()->SpawnActor<AMapTile>(WalkableTile30Degrees, Location, MyRotator, SpawnParams);
		case  2:
			return GetWorld()->SpawnActor<AMapTile>(WalkableTile60Degrees, Location, MyRotator, SpawnParams);
		case  3:
			return GetWorld()->SpawnActor<AMapTile>(WalkableTileStraight, Location, MyRotator, SpawnParams);
		case  4:
			return GetWorld()->SpawnActor<AMapTile>(WalkableTile60Degrees, Location, MyRotator, SpawnParams);
		default:
			return GetWorld()->SpawnActor<AMapTile>(WalkableTileStraight, Location, MyRotator, SpawnParams);
	}
}


FVector ARG_MapGenerator::PositionOfNthEdge(int32 EdgeNumber, const FVector& ChunkMiddle) const
{
	FVector ReturnPos(0);
	int MultiplierA = 0 , MultiplierB = 0;
	ReturnPos = ChunkMiddle;
	switch(EdgeNumber) {
	case 1:
		MultiplierA = 0;
		MultiplierB = 3;
		break;
	case 2:
		MultiplierA = 1;
		MultiplierB = 3;
		break;
	case 3:
		MultiplierA = 2;
		MultiplierB = 3;
		break;
	case 4:
		MultiplierA = 3;
		MultiplierB = 3;
		break;
	case 5:
		MultiplierA = 3;
		MultiplierB = 2;
		break;
	case 6:
		MultiplierA = 3;
		MultiplierB = 1;
		break;
	case 7:
		MultiplierA = 3;
		MultiplierB = 0;
		break;
	case 8:
		MultiplierA = 2;
		MultiplierB = -1;
		break;
	case 9:
		MultiplierA = 1;
		MultiplierB = -2;
		break;
	case 10:
		MultiplierA = 0;
		MultiplierB = -3;
		break;
	case 11:
		MultiplierA = -1;
		MultiplierB = -3;
		break;
	case 12:
		MultiplierA = -2;
		MultiplierB = -3;
		break;
	case 13:
		MultiplierA = -3;
		MultiplierB = -3;
		break;
	case 14:
		MultiplierA = -3;
		MultiplierB = -2;
		break;
	case 15:
		MultiplierA = -3;
		MultiplierB = -1;
		break;
	case 16:
		MultiplierA = -3;
		MultiplierB = 0;
		break;
	case 17:
		MultiplierA = -2;
		MultiplierB = 1;
		break;
	case 18:
		MultiplierA = -1;
		MultiplierB = 2;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Incorrect position requested: %d"), EdgeNumber);
		break;
	}
	ReturnPos.X += cos(FMath::DegreesToRadians(-60)) * TileSize * MultiplierA + cos(FMath::DegreesToRadians(180)) * TileSize * MultiplierB;
	ReturnPos.Y += sin(FMath::DegreesToRadians(-60)) * TileSize * MultiplierA + sin(FMath::DegreesToRadians(180)) * TileSize * MultiplierB;
	
	return ReturnPos;
}

void ARG_MapGenerator::FillAdjacentHexagonVectors()
{
	// upRight
	AdjacentChunks[TopRight].X = PositionOfNthEdge( 7, FVector::ZeroVector).X + cos(FMath::DegreesToRadians(0)) * TileSize * 4 ;
	AdjacentChunks[TopRight].Y =  PositionOfNthEdge( 7, FVector::ZeroVector).Y + sin(FMath::DegreesToRadians(0)) * TileSize * 4 ;
	
	// above*
	AdjacentChunks[Top].X = PositionOfNthEdge( 4, FVector::ZeroVector).X + cos(FMath::DegreesToRadians(300)) * TileSize * 4 ;
	AdjacentChunks[Top].Y =  PositionOfNthEdge( 4, FVector::ZeroVector).Y + sin(FMath::DegreesToRadians(300)) * TileSize * 4 ;
		
	// upLeft
	AdjacentChunks[TopLeft].X = PositionOfNthEdge( 1, FVector::ZeroVector).X + cos(FMath::DegreesToRadians(240)) * TileSize * 4 ;
	AdjacentChunks[TopLeft].Y =  PositionOfNthEdge( 1, FVector::ZeroVector).Y + sin(FMath::DegreesToRadians(240)) * TileSize * 4 ;
		
	// downleft
	AdjacentChunks[BotLeft].X = PositionOfNthEdge( 16, FVector::ZeroVector).X + cos(FMath::DegreesToRadians(180)) * TileSize * 4 ;
	AdjacentChunks[BotLeft].Y =  PositionOfNthEdge( 16, FVector::ZeroVector).Y + sin(FMath::DegreesToRadians(180)) * TileSize * 4 ;

	// down
	FVector ArrowDirection(0);
	AdjacentChunks[Bot].X = PositionOfNthEdge( 13, FVector::ZeroVector).X + cos(FMath::DegreesToRadians(120)) * TileSize * 4 ;
	AdjacentChunks[Bot].Y =  PositionOfNthEdge( 13, FVector::ZeroVector).Y + sin(FMath::DegreesToRadians(120)) * TileSize * 4 ;

	// downRight
	AdjacentChunks[BotRight].X = PositionOfNthEdge( 10, FVector::ZeroVector).X + cos(FMath::DegreesToRadians(60)) * TileSize * 4 ;
	AdjacentChunks[BotRight].Y =  PositionOfNthEdge( 10, FVector::ZeroVector).Y + sin(FMath::DegreesToRadians(60)) * TileSize * 4 ;
	
}

bool ARG_MapGenerator::CheckWalkable(const FVector& Location, float Radius) const
{
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
		return false;
	}
	return true;
}

void ARG_MapGenerator::FillBuildables(const FVector& Center)
{
	size_t i = 0;
	size_t y = 0;
	int x = 0;
	FVector Tester(0);
	int32 nums[7] ={4,3,2, 1, 18, 17, 16};
	SpawnedLastTime = 0;
	while (y < 7)
	{
		if (TileString.at(i) == '\n')
		{
			y++;
			x = 0;
			i++;
			continue ;
		}
		Tester = PositionOfNthEdge(nums[y], Center);
		Tester.X += cos(FMath::DegreesToRadians(0)) * TileSize * x;
		Tester.Y += sin(FMath::DegreesToRadians(0)) * TileSize * x;
		//DrawDebugSphere(GetWorld(), Tester, 25.f, 24, FColor::Red, true, -1.0f, 0, 2.0f);
		if (CheckWalkable(Tester, 1.0f))
		{
			//UE_LOG(LogTemp, Error, TEXT("We called this function!"));
			if (TileString.at(i) == '1')
				SpawnNewTile(BUILDABLE_TILE, Tester);
		}
		x++;
		i++;
	}
}

void ARG_MapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARG_MapGenerator::Expand()
{
	Super::Expand();
}

void ARG_MapGenerator::FindNextPossiblePaths(int* Answers, int Target)
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
	}
}

FVector ARG_MapGenerator::GetARandomSecondExit(const FVector& Middle, const FVector& NewExit) const
{
	
	FVector Tester(0);
	std::vector<FVector> PossibleExpands;
	for (int k = 1; k < 19; k++)
	{
		Tester = PositionOfNthEdge(k, Middle);
		if (CheckWalkable(Tester, TileSize) && FVector::Dist(NewExit, Tester) > TileSize * 3)
			PossibleExpands.push_back(Tester);
	}
	if (PossibleExpands.empty())
	{
		NextExpandPtr->CanExpand = false;
		return Middle;
	}
	std::sort(PossibleExpands.begin(), PossibleExpands.end(), [&](const FVector& a, const FVector& b)
		{
			return FVector::Dist(a, NewExit) > FVector::Dist(b, NewExit);
		});
	
	//return PossibleExpands.at(rand() % PossibleExpands.size());
	return PossibleExpands.at(0);
}

bool ARG_MapGenerator::MakeNewSplit(const FVector& Center, const FVector& Entry, const FVector& NewExit,
	const FVector& SplitExit, int SplitExitNumber)
{
	std::vector<FVector> MyNeighbors;
	std::vector<AMapTile *> TilesSpawned;
	std::vector<FVector> TileLocations;
	
	FVector Tester(0); //TODO:investigate this issue
	FVector EntryCopy = Entry;
	if (EntryCopy == NewExit || EntryCopy == SplitExit)
		return true;
	TilesSpawned.push_back(SpawnNewTile(WALKABLE_TILE, EntryCopy));
	TileLocations.push_back(Entry);
	while (FVector::Dist(EntryCopy, Center) > 1.0f)
	{
		for (int i = 1; i < 7; i++)
		{
			Tester.X = EntryCopy.X + cos(FMath::DegreesToRadians(60 * i)) * TileSize;
			Tester.Y = EntryCopy.Y + sin(FMath::DegreesToRadians(60 * i)) * TileSize;
			if (Tester == Center)
			{
				EntryCopy = Center;
				TileLocations.push_back(EntryCopy);
				goto  FirstWhileLoop;
			}
			//
			
			if(CheckWalkable(Tester, 1.0f)  && (FVector::Dist(Tester, Center) < TileSize * 3 || FVector::Dist(Tester, Center) < 1.0f) && (HasNoBadNeighbors(Tester) || FVector::Dist(Tester, Center) < 1.0f)) //&& (FVector::Dist(Tester, Center) < TileSize * 2.5 || FVector::Dist(Tester, Exit) < 1.0f)
			{
				//DrawDebugSphere(GetWorld(), Tester, 25.f, 24, FColor::Red, true, -1.0f, 0, 2.0f);
				MyNeighbors.push_back(Tester);
			}
		}
		std::sort(MyNeighbors.begin(), MyNeighbors.end(), [&](const FVector& a, const FVector& b)
		{
			return FVector::Dist(a, Center) < FVector::Dist(b, Center);
		});
		if (MyNeighbors.empty())
		{
			for (auto e : TilesSpawned)
				e->Destroy();
			TilesSpawned.clear();
			TileLocations.clear();
			MyNeighbors.clear();
			return false;
		}
		// Entry = MyNeighbors.at(std::rand() % MyNeighbors.size());
		EntryCopy = MyNeighbors.at(0);
		TilesSpawned.push_back(SpawnNewTile(WALKABLE_TILE, EntryCopy));
		TileLocations.push_back(EntryCopy);
		MyNeighbors.clear();
	}
FirstWhileLoop:
	int SizeToCenter = TileLocations.size();
	std::vector<FVector> TileLocationsOriginal(TileLocations);
	std::vector<FVector> TileLocationsSplit(TileLocations);
	while (FVector::Dist(EntryCopy, NewExit) > 1.0f)
	{
		for (int i = 1; i < 7; i++)
		{
			Tester.X = EntryCopy.X + cos(FMath::DegreesToRadians(60 * i)) * TileSize;
			Tester.Y = EntryCopy.Y + sin(FMath::DegreesToRadians(60 * i)) * TileSize;
			if (Tester == NewExit)
			{
				EntryCopy = NewExit;
				TileLocations.push_back(EntryCopy);
				goto  OriginalExitLoopEnd;
			}
			//
			
			if(CheckWalkable(Tester, 1.0f)  && (FVector::Dist(Tester, Center) < TileSize * 3 || FVector::Dist(Tester, NewExit) < 1.0f) && (HasNoBadNeighbors(Tester) || FVector::Dist(Tester, NewExit) < 1.0f)) //&& (FVector::Dist(Tester, Center) < TileSize * 2.5 || FVector::Dist(Tester, Exit) < 1.0f)
			{
				//DrawDebugSphere(GetWorld(), Tester, 25.f, 24, FColor::Red, true, -1.0f, 0, 2.0f);
				MyNeighbors.push_back(Tester);
			}
		}
		std::sort(MyNeighbors.begin(), MyNeighbors.end(), [&](const FVector& a, const FVector& b)
		{
			return FVector::Dist(a, NewExit) < FVector::Dist(b, NewExit);
		});
		if (MyNeighbors.empty())
		{
			for (auto e : TilesSpawned)
				e->Destroy();
			TilesSpawned.clear();
			TileLocations.clear();
			MyNeighbors.clear();
			return false;
		}
		// Entry = MyNeighbors.at(std::rand() % MyNeighbors.size());
		EntryCopy = MyNeighbors.at(0);
		TilesSpawned.push_back(SpawnNewTile(WALKABLE_TILE, EntryCopy));
		TileLocationsOriginal.push_back(EntryCopy);
		MyNeighbors.clear();
	}
	OriginalExitLoopEnd:
	EntryCopy = Center;
	while (FVector::Dist(EntryCopy, SplitExit) > 1.0f)
	{
		for (int i = 1; i < 7; i++)
		{
			Tester.X = EntryCopy.X + cos(FMath::DegreesToRadians(60 * i)) * TileSize;
			Tester.Y = EntryCopy.Y + sin(FMath::DegreesToRadians(60 * i)) * TileSize;
			if (Tester == SplitExit)
			{
				EntryCopy = SplitExit;
				TileLocations.push_back(EntryCopy);
				goto  SplitExitLoopEnd;
			}
			//
			
			if(CheckWalkable(Tester, 1.0f)  && (FVector::Dist(Tester, Center) < TileSize * 3 || FVector::Dist(Tester, SplitExit) < 1.0f) ) //&& (HasNoBadNeighbors(Tester) || FVector::Dist(Tester, SplitExit) < 1.0f)) //&& (FVector::Dist(Tester, Center) < TileSize * 2.5 || FVector::Dist(Tester, Exit) < 1.0f)
			{
				//DrawDebugSphere(GetWorld(), Tester, 25.f, 24, FColor::Red, true, -1.0f, 0, 2.0f);
				MyNeighbors.push_back(Tester);
			}
		}
		std::sort(MyNeighbors.begin(), MyNeighbors.end(), [&](const FVector& a, const FVector& b)
		{
			return FVector::Dist(a, SplitExit) < FVector::Dist(b, SplitExit);
		});
		if (MyNeighbors.empty())
		{
			for (auto e : TilesSpawned)
				e->Destroy();
			TilesSpawned.clear();
			TileLocations.clear();
			MyNeighbors.clear();
			return false;
		}
		// Entry = MyNeighbors.at(std::rand() % MyNeighbors.size());
		EntryCopy = MyNeighbors.at(0);
		TilesSpawned.push_back(SpawnNewTile(WALKABLE_TILE, EntryCopy));
		TileLocationsSplit.push_back(EntryCopy);
		MyNeighbors.clear();
	}
	SplitExitLoopEnd:
	for (auto e : TilesSpawned)
		e->Destroy();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	auto SplitNode = GetWorld()->SpawnActor<APathHead>(APathHead::StaticClass(), FVector(0,0,500), FRotator(0), SpawnParams);
	SplitNode->MakeNewPathFromOld(*NextExpandPtr);
	if (NextExpandPtr->MyTilePtr != nullptr)
	{
		NextExpandPtr->MyTilePtr->Destroy();
		NextExpandPtr->MyTilePtr = SpawnNewWalkableTile(NextExpandPtr->PathToBase.at(NextExpandPtr->PathToBase.size() - 2),NextExpandPtr->PathToBase.at(NextExpandPtr->PathToBase.size() - 1), TileLocations.at(0));
	}
	SpawnNewTile(SPLIT_TILE, Center);
	for (auto e = 0; e < TileLocationsOriginal.size() - 1; e++)
	{
		if (CheckWalkable(TileLocationsOriginal.at(e), 1.0f))
		{
			NextExpandPtr->MyTilePtr =  SpawnNewWalkableTile(NextExpandPtr->PathToBase.back(),  TileLocationsOriginal.at(e), TileLocationsOriginal.at(e + 1));
			// NextExpandPtr->MyTilePtr =  SpawnNewTile(WALKABLE_TILE,  TileLocationsOriginal.back());
			if (NextExpandPtr->MyTilePtr == nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("Hudston Problem.."));
			}
		}
		NextExpandPtr->PathToBase.push_back(TileLocationsOriginal.at(e));
	}
	NextExpandPtr->MyTilePtr =  SpawnNewTile(PORTAL_TILE,  TileLocationsOriginal.back());
	if (NextExpandPtr->MyTilePtr == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Hudston Problem.."));
	}
	NextExpandPtr->PathToBase.push_back(TileLocationsOriginal.back());
	int i = 0;
	for (auto e = 0; e < TileLocationsSplit.size() - 1; e++)
	{
		if (i++ < SizeToCenter)
		{
			SplitNode->PathToBase.push_back(TileLocationsSplit.at(e));
			continue ;
		}
		SplitNode->MyTilePtr =  SpawnNewWalkableTile(SplitNode->PathToBase.back(),  TileLocationsSplit.at(e), TileLocationsSplit.at(e + 1));
		if (SplitNode->MyTilePtr == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Hudston Problem.."));
		}
		SplitNode->PathToBase.push_back(TileLocationsSplit.at(e));
	}
	SplitNode->MyTilePtr =  SpawnNewTile(PORTAL_TILE,  TileLocationsSplit.back());
	if (SplitNode->MyTilePtr == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Hudston Problem.."));
	}
	SplitNode->PathToBase.push_back(TileLocationsSplit.back());
	SplitNode->MyExit = SplitExit;
	SplitNode->ExitNumber = SplitExitNumber;
	FillBuildables(Center);
	NextExpandPtr->AddPathSplines();
	SplitNode->AddPathSplines();
	AllPathEnds.Add(SplitNode);
	ShouldSplit = false;
	return true ;
}

void ARG_MapGenerator::Expand(FVector ExpandPoint)
{

	NextExpandPtr = nullptr;
	for (auto e : AllPathEnds)
	{
		e->CanExpand = CheckExpand(e->MyExit);
		if (e->CanExpand  &&  FVector::Dist(e->MyExit, ExpandPoint) <= TileSize)
		{
			NextExpandPtr = e;
		}
			
	}
	if (!NextExpandPtr)
		return ;
	
	std::vector<FVector> PossibleExpands;
	int ExpandPaths[5] {};
	FindNextPossiblePaths(ExpandPaths, NextExpandPtr->ExitNumber);
	for( int i = 0; i < 6; i++ )
	{
		if ((ExpandPaths[0] >> i) % 2)
		{
			DrawDebugSphere(GetWorld(), AdjacentChunks[i] + NextExpandPtr->MyCenter, 16.f, 16,FColor::Red, false, 10.f);

			if (CheckWalkable(AdjacentChunks[i] + NextExpandPtr->MyCenter, 1.0f))
				PossibleExpands.push_back(AdjacentChunks[i] + NextExpandPtr->MyCenter);
		}
	}
	if (PossibleExpands.empty()) // this should never happen anymore due to the check done when we choose the expand point anyways but good check for now
	{
		NextExpandPtr->CanExpand = false;
		return ;
	}
	FVector NewEntry(0);
	int Random = rand() % PossibleExpands.size();
	// 	Random = FVector::Dist(PossibleExpands.at(0), Homebase) < FVector::Dist(PossibleExpands.at(1), Homebase) ? 0 : 1;
	FVector NewCenter = PossibleExpands.at(Random);
	PossibleExpands.clear();
	for (int i = 0; i < 6; i++)
	{
		if (CheckWalkable(NewCenter + AdjacentChunks[i], 1.0f))
			PossibleExpands.push_back(NewCenter + AdjacentChunks[i]);
	}
	// if (PossibleExpands.size() >= 2 && ShouldSplit == false && MaxSplit)
	if (PossibleExpands.size() >= 2 && ShouldSplit == false)
	{
		if (rand() % SplitChance + 1 == 1)
		{
			ShouldSplit = true;
			SplitChance = 7;
			MaxSplit--;
		}
		else
			SplitChance -= 1;
	}
	
	std::vector<int> PossibleExits;
	for (int i = 1; i < 19; i++)
	{
		//DrawDebugSphere(GetWorld(), PositionOfNthEdge(i, NewCenter), 25.f, 24, FColor::Red, false, 5.0f, 0, 2.0f);
		//UE_LOG(LogTemp, Error, TEXT("The distance is) %f"), FVector::Dist(PositionOfNthEdge(i, NewCenter), NextExpandPtr->MyExit));
		if (FVector::Dist(PositionOfNthEdge(i, NewCenter), NextExpandPtr->MyExit) <= (TileSize + 0.1f))
		{
			PossibleExits.push_back(i);
		}
	}
	int RandomEntry = PossibleExits.at(rand() % PossibleExits.size());
	NewEntry = PositionOfNthEdge(RandomEntry, NewCenter);
	int NewExitNumber = 0;
	int SplitExitNumber = 0;
	FVector SplitExit(0);
	FVector NewExit = GetARandomExit(NewCenter);
	if (ShouldSplit)
		SplitExit = GetARandomSecondExit(NewCenter, NewExit);
	for (int i = 1; i < 19; i++)
	{
		if (PositionOfNthEdge(i, NewCenter) == NewExit)
			NewExitNumber = i;
	}
	if (ShouldSplit && FVector::Dist(SplitExit, NewExit) > TileSize && FVector::Dist(NewCenter, NewExit))
	{
		for (int i = 1; i < 19; i++)
		{
			if (FVector::Dist(PositionOfNthEdge(i, NewCenter), SplitExit) < 1.0f)
				SplitExitNumber = i;
		}	
	}
	// SpawnNewTile(PORTAL_TILE, NewCenter);
	// SpawnNewTile(PORTAL_TILE, NewEntry);
	// SpawnNewTile(PORTAL_TILE, NewExit);
	// return ;
	NextExpandPtr->MyEntry = NewEntry;
	NextExpandPtr->MyCenter = NewCenter;
	NextExpandPtr->ExitNumber = NewExitNumber;
	NextExpandPtr->MyExit = NewExit;
	if (ShouldSplit)
	{
		int MadnessCheck = 0;
		bool	bSuccess = false;
		while ( MadnessCheck++ < 150  && bSuccess == false)
		{
			bSuccess = MakeNewSplit(NewCenter,  NewEntry, NewExit, SplitExit, SplitExitNumber);
		}
		if (bSuccess == false)
		{
			while(!MakeNewChunk(NewCenter,  NewEntry, NewExit))
			{
			}
		}
	}
	else
	{
		while(!MakeNewChunk(NewCenter,  NewEntry, NewExit))
		{
		}
	}
	ShouldSplit = false;
	for (auto e : AllPathEnds)
	{
		e->CanExpand = CheckExpand(e->MyExit);
		if (!e->CanExpand)
		{
			if (e->MyTilePtr == nullptr)
				goto DoneHere ;
			e->MyTilePtr->Destroy();
			e->MyTilePtr = SpawnNewTile(PORTAL_TILE, e->MyExit);
		}
DoneHere:
		continue ;
	}
}

FVector ARG_MapGenerator::GetARandomExit(const FVector& Middle)
{
	FVector Tester(0);
	std::vector<FVector> PossibleExpands;
	for (int k = 1; k < 19; k++)
	{
		Tester = PositionOfNthEdge(k, Middle);
		if (CheckWalkable(Tester, TileSize))
			PossibleExpands.push_back(Tester);
	}
	if (PossibleExpands.empty())
	{
		NextExpandPtr->CanExpand = false;
		return Middle;
	}
	std::sort(PossibleExpands.begin(), PossibleExpands.end(), [&](const FVector & a, const FVector & b)
	{
		return FVector::Dist(NextExpandPtr->MyCenter, a) < FVector::Dist(NextExpandPtr->MyCenter, b);
	});
	if (rand() % 5 == 1)
		return PossibleExpands.at(0);
	return PossibleExpands.at(rand() % PossibleExpands.size());
}

void ARG_MapGenerator::GetPositions(std::vector<FVector>& InVector)
{
	for (auto e : AllPathEnds)
	{
		e->CanExpand = CheckExpand(e->MyExit);
		if (e->CanExpand)
		{
			InVector.push_back(e->MyExit);
		}
		else
		{
			if (e->MyTilePtr)
				e->MyTilePtr->Destroy();
			e->MyTilePtr = SpawnNewTile(PORTAL_TILE, e->MyExit);
		}
	}
}

TArray<USplineComponent *> ARG_MapGenerator::GetSplines()
{
	TArray<USplineComponent *> Splines;
	for (APathHead* PathHead : AllPathEnds)
	{
		Splines.Add(PathHead->MySpline);
	}
	return Splines;
}

FVector ARG_MapGenerator::GetEndNode()
{
	return AllPathEnds[0]->MyExit;
}

USplineComponent* ARG_MapGenerator::GetSpline()
{
	return AllPathEnds[rand() % AllPathEnds.Num()]->MySpline;
}

void ARG_MapGenerator::GenerateFirstChunk()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	FVector Tester(0);
	AMapTile *TesterTile = SpawnNewTile(WALKABLE_TILE, Tester);

	TesterTile->CalculateSideLength();
	TesterTile->CalculateHeight();
	TileHeight = TesterTile->TileHeight;
	TileSize = TesterTile->SideLength;
	// GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Red, FString::Printf(TEXT("SIZE: %f"), TileSize));

	TesterTile->Destroy();
	
	
	Tester = FVector(0);
	Tester.X += cos(FMath::DegreesToRadians(0)) * TileSize * 3;
	Tester.Y += sin(FMath::DegreesToRadians(0)) * TileSize * 3;
	FillAdjacentHexagonVectors();
	int int1 = std::rand() % 18 + 1;
	NextExpandPtr = GetWorld()->SpawnActor<APathHead>(APathHead::StaticClass(), FVector(0,0,500), FRotator(0), SpawnParams);
	if (!NextExpandPtr)
		return ;
	NextExpandPtr->MyEntry = Tester;
	NextExpandPtr->MyCenter = Tester;
	UE_LOG(LogTemp, Error, TEXT("We have original entry) %d"), int1);
	NextExpandPtr->MyExit = PositionOfNthEdge(int1, NextExpandPtr->MyCenter);
	NextExpandPtr->EntryNumber = 0;
	NextExpandPtr->ExitNumber = int1;
	NextExpandPtr->TileSize = TileSize;
	NextExpandPtr->TileHeight = TileHeight;
	HomeBase = NextExpandPtr->MyEntry;
	while(!MakeNewChunk(NextExpandPtr->MyCenter, NextExpandPtr->MyCenter, NextExpandPtr->MyExit))
		;
	ABuildableMapTile* HomeBasePtr = Cast<ABuildableMapTile>(HomeBaseTile);
	HomeBasePtr->SpawnBase();
	AllPathEnds.Add(NextExpandPtr);
	FillBuildables(NextExpandPtr->MyCenter);
}

bool ARG_MapGenerator::CheckExpand(const FVector& EndPoint)
{
	FVector Tester(0);
	for (int i = 1; i < 7; i++)
	{
		Tester.X = EndPoint.X + cos(FMath::DegreesToRadians(60 * i)) * TileSize;
		Tester.Y = EndPoint.Y + sin(FMath::DegreesToRadians(60 * i)) * TileSize;
		//DrawDebugSphere(GetWorld(), Tester, 25.f, 24, FColor::Red, true, -1.0f, 0, 2.0f);
		if (CheckWalkable(Tester, 1.0f))
			return true;
	}
	return false ;
}

bool ARG_MapGenerator::MakeNewChunk(const FVector& Center, FVector Entry, const FVector& Exit)
{
	std::vector<FVector> MyNeighbors;
	std::vector<AMapTile *> TilesSpawned;
	std::vector<FVector> TileLocations;
	FVector Tester(0); //TODO:investigate this issue
	if (Entry == Exit)
		return true;
	TilesSpawned.push_back(SpawnNewTile(WALKABLE_TILE, Entry));
	TileLocations.push_back(Entry);
	while (FVector::Dist(Entry, Exit) > 1.0f)
	{
		for (int i = 1; i < 7; i++)
		{
			Tester.X = Entry.X + cos(FMath::DegreesToRadians(60 * i)) * TileSize;
			Tester.Y = Entry.Y + sin(FMath::DegreesToRadians(60 * i)) * TileSize;
			if (Tester == Exit)
			{
				Entry = Exit;
				TileLocations.push_back(Entry);
				goto  MakeNewChunk_END;
			}
			//
			
			if(CheckWalkable(Tester, 1.0f)  && (FVector::Dist(Tester, Center) < TileSize * 3 || FVector::Dist(Tester, Exit) < 1.0f) && (HasNoBadNeighbors(Tester) || FVector::Dist(Tester, Exit) < 1.0f)) //&& (FVector::Dist(Tester, Center) < TileSize * 2.5 || FVector::Dist(Tester, Exit) < 1.0f)
			{
				//DrawDebugSphere(GetWorld(), Tester, 25.f, 24, FColor::Red, true, -1.0f, 0, 2.0f);
				MyNeighbors.push_back(Tester);
			}
		}
		std::sort(MyNeighbors.begin(), MyNeighbors.end(), [&](const FVector& a, const FVector& b)
		{
			return FVector::Dist(a, Exit) < FVector::Dist(b, Exit);
		});
		if (MyNeighbors.empty())
		{
			for (auto e : TilesSpawned)
				e->Destroy();
			TilesSpawned.clear();
			TileLocations.clear();
			MyNeighbors.clear();
			return false;
		}
		// Entry = MyNeighbors.at(std::rand() % MyNeighbors.size());
		Entry = MyNeighbors.at(0);
		TilesSpawned.push_back(SpawnNewTile(WALKABLE_TILE, Entry));
		TileLocations.push_back(Entry);
		MyNeighbors.clear();
	}
MakeNewChunk_END:
	for (auto e : TilesSpawned)
		e->Destroy();
	if (HomeBaseBuilt == false)
	{
		HomeBaseTile = SpawnNewTile(BUILDABLE_TILE, TileLocations.at(0));
		NextExpandPtr->MyTilePtr = HomeBaseTile;
		HomeBaseBuilt = true;
		NextExpandPtr->PathToBase.push_back(TileLocations.at(0));
	}
	else
	{
		NextExpandPtr->MyTilePtr->Destroy();
		NextExpandPtr->MyTilePtr = SpawnNewWalkableTile(NextExpandPtr->PathToBase.at(NextExpandPtr->PathToBase.size() - 2),NextExpandPtr->PathToBase.at(NextExpandPtr->PathToBase.size() - 1), TileLocations.at(0));
	}
	for (auto e = 0; e < TileLocations.size() - 1 ; e++)
	{
		if (CheckWalkable( TileLocations.at(e), 1.0f))
		{
			NextExpandPtr->MyTilePtr = SpawnNewWalkableTile(NextExpandPtr->PathToBase.back(), TileLocations.at(e), TileLocations.at(e + 1));
			// NextExpandPtr->MyTilePtr = SpawnNewTile(WALKABLE_TILE, e);
			NextExpandPtr->PathToBase.push_back(TileLocations.at(e));
		}
	}
	
	NextExpandPtr->MyTilePtr = SpawnNewTile(PORTAL_TILE, TileLocations.back());
	// NextExpandPtr->MyTilePtr = SpawnNewTile(WALKABLE_TILE, e);
	NextExpandPtr->PathToBase.push_back(TileLocations.back());
	FillBuildables(Center);
	NextExpandPtr->AddPathSplines();
	return true ;
}

bool ARG_MapGenerator::HasNoBadNeighbors(const FVector& Position) const
{
	std::vector<FVector> MyNeighbors;
	FVector Tester(0);
	
	for (int i = 1; i < 7; i++)
	{
		Tester.X = Position.X + cos(FMath::DegreesToRadians(60 * i)) * TileSize;
		Tester.Y = Position.Y + sin(FMath::DegreesToRadians(60 * i)) * TileSize;
	//	DrawDebugSphere(GetWorld(), Tester, 25.f, 24, FColor::Red, true, -1.0f, 0, 2.0f);
			
		if(!CheckWalkable(Tester, 1.0f) ){
			MyNeighbors.push_back(Tester);
		}
	}
	if (MyNeighbors.size() < 2)
		return true;
	return false;
}


SimplePerlinNoise::SimplePerlinNoise()
{
	// Initialize the permutation vector with values from 0 to 255
	p.resize(256);
	std::iota(p.begin(), p.end(), 0);

	// Shuffle using the standard random number generator
	std::default_random_engine engine(std::random_device{}());
	std::shuffle(p.begin(), p.end(), engine);

	// Duplicate the permutation vector
	p.insert(p.end(), p.begin(), p.end());
}

double SimplePerlinNoise::noise(double x, double y)
{
	// Find unit grid cell containing point
	int X = static_cast<int>(std::floor(x)) & 255;
	int Y = static_cast<int>(std::floor(y)) & 255;

	// Get relative x and y coordinates of point within that cell
	x -= std::floor(x);
	y -= std::floor(y);

	// Compute fade curves for x and y
	double u = fade(x);
	double v = fade(y);

	// Hash coordinates of the 4 square corners
	int aa = p[p[X] + Y];
	int ab = p[p[X] + Y + 1];
	int ba = p[p[X + 1] + Y];
	int bb = p[p[X + 1] + Y + 1];

	// Add blended results from 4 corners of the square
	double res = lerp(v, lerp(u, grad(aa, x, y), grad(ba, x - 1, y)),
					  lerp(u, grad(ab, x, y - 1), grad(bb, x - 1, y - 1)));
	return (res + 1.0) / 2.0; // Convert to [0, 1] range
}

double SimplePerlinNoise::fade(double t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double SimplePerlinNoise::lerp(double t, double a, double b)
{
	return a + t * (b - a);
}

double SimplePerlinNoise::grad(int hash, double x, double y)
{
	int h = hash & 7;
	double u = h < 4 ? x : y;
	double v = h < 4 ? y : x;
	return ((h & 1) ? -u : u) + ((h & 2) ? -2.0 * v : 2.0 * v);
}