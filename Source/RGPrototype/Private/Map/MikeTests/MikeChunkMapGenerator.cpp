// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/MikeTests/MikeChunkMapGenerator.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Game/RGGameMode.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AMikeChunkMapGenerator::AMikeChunkMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
	// Initialize the mesh component
	MeshComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MikeCunkMapGenerator"));
	RootComponent = MeshComponent;
	NextExpandPtr  = nullptr;
}

AMapTile* AMikeChunkMapGenerator::SpawnNewTile(int Type, const FVector& Location)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	if(Type == BUILDABLE_TILE)
	{
		return MyWorld->SpawnActor<AMapTile>(BuildableTile, Location, FRotator(0), SpawnParams);
	}
	if(Type == WALKABLE_TILE)
	{
		return MyWorld->SpawnActor<AMapTile>(WalkableTile, Location, FRotator(0), SpawnParams);
	}
	if(Type == PORTAL_TILE)
	{
		return MyWorld->SpawnActor<AMapTile>(TunnelTile, Location, FRotator(0), SpawnParams);
	}
	if(Type == SPLIT_TILE)
	{
		return MyWorld->SpawnActor<AMapTile>(TunnelTile, Location, FRotator(0), SpawnParams);
	}
	return nullptr;
}




FVector AMikeChunkMapGenerator::PositionOfEdge(int32 EdgeNumber, const FVector& ChunkMiddle) const
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

bool AMikeChunkMapGenerator::CheckExpandTwoWays(const FVector& MyCenter, int ExitNumber) const
{
	
	FVector SurroundingCenters[6] {};

		// downRight
		SurroundingCenters[5].X = PositionOfEdge( 10, MyCenter).X + cos(FMath::DegreesToRadians(60)) * TileSize * 4 ;
		SurroundingCenters[5].Y =  PositionOfEdge( 10, MyCenter).Y + sin(FMath::DegreesToRadians(60)) * TileSize * 4 ;
		// down

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
		int ExpandPaths[5] {};
		FindNextPossiblePaths(ExpandPaths, ExitNumber);
		for( int i = 0; i < 6; i++ )
		{
			if ((ExpandPaths[0] >> i) % 2)
			{
				if (CheckWalkable(SurroundingCenters[i], 1.0f))
					PossibleExpands.push_back(SurroundingCenters[i]);
			}
		}
		if (PossibleExpands.size() >= 2)
		{
			return true;
		}
	return false;
}

bool AMikeChunkMapGenerator::MakeNewChunk(const FVector& Center, FVector Entry, const FVector& Exit)
{
	std::vector<FVector> MyNeighbors;
	std::vector<AMapTile *> TilesSpawned;
	std::vector<FVector> TileLocations;
	FVector Tester(0); //TODO:investigate this issue
	if (Entry == Exit)
		return true;
	UE_LOG(LogTemp, Error, TEXT("We called this function!"));
	UE_LOG(LogTemp, Error, TEXT("We have exit: %s"), *Exit.ToString());
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
				goto  MakeNewChunk_END;
			}
			if(CheckWalkable(Tester, 1.0f)  && (FVector::Dist(Tester, Center) < TileSize * 3 || FVector::Dist(Tester, Exit) < 1.0f) && (HasNoBadNeighbors(Tester) || FVector::Dist(Tester, Exit) < 1.0f)) //&& (FVector::Dist(Tester, Center) < TileSize * 2.5 || FVector::Dist(Tester, Exit) < 1.0f)
			{
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
	if(Exit == Center)
	{
		NextExpandPtr->MyTilePtr = SpawnNewTile(PORTAL_TILE, Entry);
		for (auto e : TileLocations)
		{
			if(CheckWalkable(e, 1.0f))
				SpawnNewTile(WALKABLE_TILE, e);
			NextExpandPtr->PathToBase.push_back(e);
		}
		NextExpandPtr->PathToBase.push_back(Entry);
		NextExpandPtr->CanExpand = false;
	}
	else
	{
		bool Splitter = true;
		if (FirstMade && Splitter && CheckExpandTwoWays(Center, NextExpandPtr->ExitNumber))
		{
			NextExpandPtr->MyTilePtr = SpawnNewTile(PORTAL_TILE, Entry);
			for (auto e : TileLocations)
			{
				SpawnNewTile(WALKABLE_TILE, e);
				NextExpandPtr->PathToBase.push_back(e);
			}
			NextExpandPtr->PathToBase.push_back(Entry);
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			auto a = GetWorld()->SpawnActor<AMapExits>(AMapExits::StaticClass(), FVector(0), FRotator(0), SpawnParams);
			a->MakeNewPathFromOld(*NextExpandPtr);
			AllPathEnds.Add(a);
		}
			
		else
		{
			NextExpandPtr->MyTilePtr = SpawnNewTile(WALKABLE_TILE, Entry);
			for (auto e : TileLocations)
			{
				SpawnNewTile(WALKABLE_TILE, e);
				NextExpandPtr->PathToBase.push_back(e);
			}
			NextExpandPtr->PathToBase.push_back(Entry);
		}
	}
	NextExpandPtr->AddPathSplines();
	return true ;
}

bool AMikeChunkMapGenerator::HasNoBadNeighbors(const FVector& Position) const
{
	std::vector<FVector> MyNeighbors;
	FVector Tester(0);
	
	for (int i = 1; i < 7; i++)
	{
		Tester.X = Position.X + cos(FMath::DegreesToRadians(60 * i)) * TileSize;
		Tester.Y = Position.Y + sin(FMath::DegreesToRadians(60 * i)) * TileSize;
		if(!CheckWalkable(Tester, 1.0f) ){
				MyNeighbors.push_back(Tester);
		}
	}
	if (MyNeighbors.size() < 2)
		return true;
	return false;
}

void AMikeChunkMapGenerator::FillBuildables(FVector Center)
{
	size_t i = 0;
	size_t y = 0;
	int x = 0;
	FVector Tester(0);
	int32 nums[7] ={4,3,2, 1, 18, 17, 16};
	while (y < 7)
	{
		if (TileString.at(i) == '\n')
		{
			y++;
			x = 0;
			i++;
			continue ;
		}
		Tester = PositionOfEdge(nums[y], Center);
		Tester.X += cos(FMath::DegreesToRadians(0)) * TileSize * x;
		Tester.Y += sin(FMath::DegreesToRadians(0)) * TileSize * x;
		if (CheckWalkable(Tester, 1.0f))
		{
			if (TileString.at(i) == '1')
				SpawnNewTile(BUILDABLE_TILE, Tester);
			else
				SpawnNewTile(WALKABLE_TILE, Tester);
		}
		x++;
		i++;
	}
}

bool AMikeChunkMapGenerator::FindNextPossiblePaths(int *Answers,  const int Target)
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

FVector AMikeChunkMapGenerator::GetARandomExit(const FVector& Middle)
{
	FVector Tester(0);
	std::vector<FVector> PossibleExpands;
	for (int k = 1; k < 19; k++)
	{
		Tester = PositionOfEdge(k, Middle);
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

bool AMikeChunkMapGenerator::CanPathExpand(const AMapExits* CheckExit) const
{
	if (!CheckExit)
		return false;
	return CheckExit->CanExpand;
}

bool AMikeChunkMapGenerator::CheckWalkable(const FVector& Location, float Radius) const
{
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
		return false;
	}
	return true;
}

void AMikeChunkMapGenerator::Expand(void)
{
	if (MyWorld)
	{
		
		std::vector<AMapExits *> PossibleExits;
		for (auto e : AllPathEnds)
		{
			e->CheckExpand();
			if (e->CanExpand)
				PossibleExits.push_back(e);
		}
		int ChooseExit = rand() % AllPathEnds.Num();
		NextExpandPtr = PossibleExits.at(ChooseExit);
		FVector SurroundingCenters[6] = {FVector(0)};
		
		// downRight
		SurroundingCenters[5].X = PositionOfEdge( 10, NextExpandPtr->MyCenter).X + cos(FMath::DegreesToRadians(60)) * TileSize * 4 ;
		SurroundingCenters[5].Y =  PositionOfEdge( 10, NextExpandPtr->MyCenter).Y + sin(FMath::DegreesToRadians(60)) * TileSize * 4 ;
		// down
		FVector ArrowDirection(0);
		SurroundingCenters[4].X = PositionOfEdge( 13, NextExpandPtr->MyCenter).X + cos(FMath::DegreesToRadians(120)) * TileSize * 4 ;
		SurroundingCenters[4].Y =  PositionOfEdge( 13, NextExpandPtr->MyCenter).Y + sin(FMath::DegreesToRadians(120)) * TileSize * 4 ;
		
		// downleft
		SurroundingCenters[3].X = PositionOfEdge( 16, NextExpandPtr->MyCenter).X + cos(FMath::DegreesToRadians(180)) * TileSize * 4 ;
		SurroundingCenters[3].Y =  PositionOfEdge( 16, NextExpandPtr->MyCenter).Y + sin(FMath::DegreesToRadians(180)) * TileSize * 4 ;
		
		
		// upLeft
		SurroundingCenters[2].X = PositionOfEdge( 1, NextExpandPtr->MyCenter).X + cos(FMath::DegreesToRadians(240)) * TileSize * 4 ;
		SurroundingCenters[2].Y =  PositionOfEdge( 1, NextExpandPtr->MyCenter).Y + sin(FMath::DegreesToRadians(240)) * TileSize * 4 ;
		
		// above*
		SurroundingCenters[1].X = PositionOfEdge( 4, NextExpandPtr->MyCenter).X + cos(FMath::DegreesToRadians(300)) * TileSize * 4 ;
		SurroundingCenters[1].Y =  PositionOfEdge( 4, NextExpandPtr->MyCenter).Y + sin(FMath::DegreesToRadians(300)) * TileSize * 4 ;
		
		// upRight
		SurroundingCenters[0].X = PositionOfEdge( 7, NextExpandPtr->MyCenter).X + cos(FMath::DegreesToRadians(0)) * TileSize * 4 ;
		SurroundingCenters[0].Y =  PositionOfEdge( 7, NextExpandPtr->MyCenter).Y + sin(FMath::DegreesToRadians(0)) * TileSize * 4 ;
		std::vector<FVector> PossibleExpands;
		int ExpandPaths[5] {};
		FindNextPossiblePaths(ExpandPaths, NextExpandPtr->ExitNumber);
		for( int i = 0; i < 6; i++ )
		{
			if ((ExpandPaths[0] >> i) % 2)
			{
				if (CheckWalkable(SurroundingCenters[i], 1.0f))
				{
					PossibleExpands.push_back(SurroundingCenters[i]);
				}
				DrawDebugSphere(GetWorld(), SurroundingCenters[i], 64.f, 16,FColor::Blue, false, 10.f);
				UE_LOG(LogTemp, Error, TEXT("We are checking for %d i :D"), i);
			}
		}
		if (PossibleExpands.empty()) // this should never happen anymore due to the check done when we choose the expand point anyways but good check for now
		{
			NextExpandPtr->CanExpand = false;
			return ;
		}
		FVector NewEntry(0);
		int Random = rand() % PossibleExpands.size();
		// if (PossibleExpands.size()  == 2)
		// 	Random = FVector::Dist(PossibleExpands.at(0), Homebase) < FVector::Dist(PossibleExpands.at(1), Homebase) ? 0 : 1;
		FVector NewCenter = PossibleExpands.at(Random);
		int RandomEntry =  std::rand() % 2;
		if (Random == 0)
		{
				RandomEntry =  std::rand() % 2;
				if (RandomEntry == 0 && ExpandPaths[RandomEntry + 1] != 0)
					RandomEntry = ExpandPaths[RandomEntry + 1];
				else
				{
					RandomEntry = (RandomEntry + 1) % 2;
					RandomEntry = ExpandPaths[RandomEntry + 1];
				}
		}
		if (Random == 1)
		{
			RandomEntry =  std::rand() % 2;
			if (RandomEntry == 0 && ExpandPaths[RandomEntry + 3] != 0)
				RandomEntry = ExpandPaths[RandomEntry + 3];
			else
			{
				RandomEntry = (RandomEntry + 1) % 2;
				RandomEntry = ExpandPaths[RandomEntry + 3];
			}
		}
		UE_LOG(LogTemp, Error, TEXT("We chose: RandomEntry %d"), RandomEntry);
		NewEntry = PositionOfEdge(RandomEntry, NewCenter);
		if (FVector::Dist(PositionOfEdge(NextExpandPtr->ExitNumber, NextExpandPtr->MyCenter), NewEntry) > TileSize * 2)
		{
			// Expand();
			return ;
		}
		
		int NewExitNumber = 0;
		FVector NewExit = GetARandomExit(NewCenter);
		for (int i = 0; i < 19; i++)
		{
			if (PositionOfEdge(i, NewCenter) == NewExit)
				NewExitNumber = i;
		}
		// SpawnNewTile(PORTAL_TILE, NewCenter);
		// SpawnNewTile(PORTAL_TILE, NewEntry);
		// SpawnNewTile(PORTAL_TILE, NewExit);
		// return ;
		NextExpandPtr->MyEntry = NewEntry;
		NextExpandPtr->MyCenter = NewCenter;
		NextExpandPtr->ExitNumber = NewExitNumber;
		NextExpandPtr->MyExit = NewExit;
		while(!MakeNewChunk(NewCenter,  NewEntry, NewExit))
		{
		}
		FillBuildables(NewCenter);
	}
}

void AMikeChunkMapGenerator::GetPositions(std::vector<FVector>& InVector)
{
	for (auto e : AllPathEnds)
	{
		e->CheckExpand();
		if (e->CanExpand)
		{
			InVector.push_back(e->MyExit);
		}
	}
}

void AMikeChunkMapGenerator::Expand(FVector  ExpandPoint)
{
	if (MyWorld)
	{
		NextExpandPtr = nullptr;
		for (auto e : AllPathEnds)
		{
			e->CheckExpand();
			if (e->CanExpand  &&  FVector::Dist(e->MyExit, ExpandPoint) <= TileSize)
			{
				NextExpandPtr = e;
			}
				
		}
		if (!NextExpandPtr)
			return ;
		FVector SurroundingCenters[6] = {FVector(0)};
		
		// downRight
		SurroundingCenters[5].X = PositionOfEdge( 10, NextExpandPtr->MyCenter).X + cos(FMath::DegreesToRadians(60)) * TileSize * 4 ;
		SurroundingCenters[5].Y =  PositionOfEdge( 10, NextExpandPtr->MyCenter).Y + sin(FMath::DegreesToRadians(60)) * TileSize * 4 ;
		// down
		FVector ArrowDirection(0);
		SurroundingCenters[4].X = PositionOfEdge( 13, NextExpandPtr->MyCenter).X + cos(FMath::DegreesToRadians(120)) * TileSize * 4 ;
		SurroundingCenters[4].Y =  PositionOfEdge( 13, NextExpandPtr->MyCenter).Y + sin(FMath::DegreesToRadians(120)) * TileSize * 4 ;
		
		// downleft
		SurroundingCenters[3].X = PositionOfEdge( 16, NextExpandPtr->MyCenter).X + cos(FMath::DegreesToRadians(180)) * TileSize * 4 ;
		SurroundingCenters[3].Y =  PositionOfEdge( 16, NextExpandPtr->MyCenter).Y + sin(FMath::DegreesToRadians(180)) * TileSize * 4 ;
		
		
		// upLeft
		SurroundingCenters[2].X = PositionOfEdge( 1, NextExpandPtr->MyCenter).X + cos(FMath::DegreesToRadians(240)) * TileSize * 4 ;
		SurroundingCenters[2].Y =  PositionOfEdge( 1, NextExpandPtr->MyCenter).Y + sin(FMath::DegreesToRadians(240)) * TileSize * 4 ;
		
		// above*
		SurroundingCenters[1].X = PositionOfEdge( 4, NextExpandPtr->MyCenter).X + cos(FMath::DegreesToRadians(300)) * TileSize * 4 ;
		SurroundingCenters[1].Y =  PositionOfEdge( 4, NextExpandPtr->MyCenter).Y + sin(FMath::DegreesToRadians(300)) * TileSize * 4 ;
		
		// upRight
		SurroundingCenters[0].X = PositionOfEdge( 7, NextExpandPtr->MyCenter).X + cos(FMath::DegreesToRadians(0)) * TileSize * 4 ;
		SurroundingCenters[0].Y =  PositionOfEdge( 7, NextExpandPtr->MyCenter).Y + sin(FMath::DegreesToRadians(0)) * TileSize * 4 ;
		std::vector<FVector> PossibleExpands;
		int ExpandPaths[5] {};
		FindNextPossiblePaths(ExpandPaths, NextExpandPtr->ExitNumber);
		for( int i = 0; i < 6; i++ )
		{
			if ((ExpandPaths[0] >> i) % 2)
			{
				DrawDebugSphere(GetWorld(), SurroundingCenters[i], 16.f, 16,FColor::Red, false, 10.f);

				if (CheckWalkable(SurroundingCenters[i], 1.0f))
					PossibleExpands.push_back(SurroundingCenters[i]);
			}
		}
		if (PossibleExpands.empty()) // this should never happen anymore due to the check done when we choose the expand point anyways but good check for now
		{
			NextExpandPtr->CanExpand = false;
			return ;
		}
		FVector NewEntry(0);
		int Random = rand() % PossibleExpands.size();
		// if (PossibleExpands.size()  == 2)
		// 	Random = FVector::Dist(PossibleExpands.at(0), Homebase) < FVector::Dist(PossibleExpands.at(1), Homebase) ? 0 : 1;
		FVector NewCenter = PossibleExpands.at(Random);
		int RandomEntry =  std::rand() % 2;
		if (Random == 0)
		{
				RandomEntry =  std::rand() % 2;
				if (RandomEntry == 0 && ExpandPaths[RandomEntry + 1] != 0)
					RandomEntry = ExpandPaths[RandomEntry + 1];
				else
				{
					RandomEntry = (RandomEntry + 1) % 2;
					RandomEntry = ExpandPaths[RandomEntry + 1];
				}
		}
		if (Random == 1)
		{
			RandomEntry =  std::rand() % 2;
			if (RandomEntry == 0 && ExpandPaths[RandomEntry + 3] != 0)
				RandomEntry = ExpandPaths[RandomEntry + 3];
			else
			{
				RandomEntry = (RandomEntry + 1) % 2;
				RandomEntry = ExpandPaths[RandomEntry + 3];
			}
		}
		UE_LOG(LogTemp, Error, TEXT("We chose: %d exit"), RandomEntry);
		NewEntry = PositionOfEdge(RandomEntry, NewCenter);
		if (FVector::Dist(PositionOfEdge(NextExpandPtr->ExitNumber, NextExpandPtr->MyCenter), NewEntry) > TileSize * 2)
		{
			// Expand();
			return ;
		}
		
		int NewExitNumber = 0;
		FVector NewExit = GetARandomExit(NewCenter);
		for (int i = 0; i < 19; i++)
		{
			if (PositionOfEdge(i, NewCenter) == NewExit)
				NewExitNumber = i;
		}
		// SpawnNewTile(PORTAL_TILE, NewCenter);
		// SpawnNewTile(PORTAL_TILE, NewEntry);
		// SpawnNewTile(PORTAL_TILE, NewExit);
		// return ;
		NextExpandPtr->MyEntry = NewEntry;
		NextExpandPtr->MyCenter = NewCenter;
		NextExpandPtr->ExitNumber = NewExitNumber;
		NextExpandPtr->MyExit = NewExit;
		while(!MakeNewChunk(NewCenter,  NewEntry, NewExit))
		{
		}
		FillBuildables(NewCenter);
	}
}

FVector AMikeChunkMapGenerator::GetEndNode()
{
	
	return AllPathEnds[rand() % AllPathEnds.Num()]->MyExit;
}


// Called when the game starts or when spawned
void AMikeChunkMapGenerator::BeginPlay()
{
	Super::BeginPlay();
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	MyWorld = GetWorld();

	GenerateFirstChunk();
}

USplineComponent* AMikeChunkMapGenerator::GetSpline()
{
	return AllPathEnds[rand() % AllPathEnds.Num()]->MySpline;
}

void AMikeChunkMapGenerator::GenerateFirstChunk()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	FVector Tester(0);
	AMapTile *TesterTile = SpawnNewTile(WALKABLE_TILE, Tester);

	TesterTile->CalculateSideLength();
	TileSize = TesterTile->SideLength;
	// GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Red, FString::Printf(TEXT("SIZE: %f"), TileSize));

	TesterTile->Destroy();
	
	Tester = FVector(0);
	Tester.X += cos(FMath::DegreesToRadians(0)) * TileSize * 3;
	Tester.Y += sin(FMath::DegreesToRadians(0)) * TileSize * 3;
	int int1 = 13; // std::rand() % 18 + 1;
	NextExpandPtr = GetWorld()->SpawnActor<AMapExits>(AMapExits::StaticClass(), FVector(0,0,500), FRotator(0), SpawnParams);
	if (!NextExpandPtr)
		return ;
	NextExpandPtr->MyEntry = Tester;
	NextExpandPtr->MyCenter = Tester;
	UE_LOG(LogTemp, Error, TEXT("We have original entry) %d"), int1);
	NextExpandPtr->MyExit = PositionOfEdge(int1, NextExpandPtr->MyCenter);
	NextExpandPtr->EntryNumber = 0;
	NextExpandPtr->ExitNumber = int1;
	NextExpandPtr->TileSize = TileSize;
	Homebase = NextExpandPtr->MyEntry;
	while(!MakeNewChunk(NextExpandPtr->MyCenter, NextExpandPtr->MyCenter, NextExpandPtr->MyExit))
		;
	AllPathEnds.Add(NextExpandPtr);
	FillBuildables(NextExpandPtr->MyCenter);
	FirstMade = true;
}

// Called every frame
void AMikeChunkMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

