#include "Map/MapTileSpawner.h"

#include "Engine/Engine.h"
#include "Components/SplineComponent.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Game/RGGameMode.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Map/BuildableMapTile.h"

class ABuildableMapTile;
// Sets default values
AMapTileSpawner::AMapTileSpawner()
{
	// Set this pawn to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
	// Initialize the mesh component
	MeshComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TilerSpawner"));
	RootComponent = MeshComponent;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");

	MyWorld = GetWorld();
}

AMapTile* AMapTileSpawner::SpawnNewTile(int Type, FVector Location)
{
	if (!MyWorld)
			return nullptr;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	if(Type == 1)
	{
		return MyWorld->SpawnActor<AMapTile>(BuildableTile, Location, FRotator(0), SpawnParams);
	}
	if(Type == 2)
	{
		Spline->AddSplinePoint(Location, ESplineCoordinateSpace::World);
		return MyWorld->SpawnActor<AMapTile>(WalkableTile, Location, FRotator(0), SpawnParams);
	}
	if(Type == 3)
	{
		Spline->AddSplinePoint(Location, ESplineCoordinateSpace::World);
		return nullptr;
	}
	if(Type == 4)
	{
		return MyWorld->SpawnActor<AMapTile>(TunnelTile, Location, FRotator(0), SpawnParams);
	}
	if(Type == 5)
	{
		Spline->AddSplinePoint(Location, ESplineCoordinateSpace::World);
		return MyWorld->SpawnActor<AMapTile>(TunnelTile, Location, FRotator(0), SpawnParams);
	}
	return nullptr;
}

bool AMapTileSpawner::CheckWalkable(const FVector& Location, float Radius)
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
		for (auto e : OverlappingActors)
			if (e->GetActorLocation().Z >= -1.0f)
				return false;
		return true;
	}
	return true;
}

bool AMapTileSpawner::CheckPathable(const FVector& Location)
{
	FVector Copy = Location;
	int Checks = 0;
	for (int i = 1; i < 7; i++)
	{
		Copy.X = Location.X + cos(FMath::DegreesToRadians(60 * i)) * TileSize;
		Copy.Y = Location.Y + sin(FMath::DegreesToRadians(60 * i)) * TileSize;
		if (CheckWalkable(Copy, 1.0f))
			Checks++;
	}
	if (Checks >= 2)
		return true;
	return false;
}

// Called when the game starts or when spawned
void AMapTileSpawner::BeginPlay()
{
Super::BeginPlay();
Spline->ClearSplinePoints();
std::srand(static_cast<unsigned int>(std::time(nullptr)));
FActorSpawnParameters TestParams;
TestParams.Owner = this; // No specific owner
TestParams.Instigator = GetInstigator(); // No specific instigator
TestParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
AMapTile* TestActor = GetWorld()->SpawnActor<AMapTile>(WalkableTile, FVector(0), FRotator(0),  TestParams);
TileSideSize = TestActor->SideLength;
TestActor->Destroy();
//size_t Index;

TileSize = std::sqrt(3) * TileSideSize + 1.0f; // Width of a hexagon
TileSideSize += 1.0f;
FActorSpawnParameters SpawnParams;
SpawnParams.Owner = this;
SpawnParams.Instigator = GetInstigator();
FVector start(0);
FirstBase.Pos = start;
FirstBase.BBuildable = true;
FirstBase.TilePtr = MyWorld->SpawnActor<AMapTile>(BuildableTile, start, FRotator(0), SpawnParams);
TileSideSize = FirstBase.TilePtr->SideLength;
// TileSize = std::sqrt(3) * TileSideSize + 1.0f; // Width of a hexagon 174.205078
TileSize = 174.205078; // Width of a hexagon 
GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Hello, %f"), TileSize));
MyTiles.Add(FirstBase.TilePtr);
TheLastBefore = FirstBase;
Spline->AddSplinePoint(start, ESplineCoordinateSpace::World);
if (!FirstBase.TilePtr)
{
UE_LOG(LogTemp, Error, TEXT("Failed to spawn MapTile at location: %s"), *(FirstBase.Pos).ToString());
return ;
}
ABuildableMapTile *First = Cast<ABuildableMapTile>(FirstBase.TilePtr);
First->SpawnBase();
start.X += cos(FMath::DegreesToRadians(60)) * TileSize;
start.Y += sin(FMath::DegreesToRadians(60)) * TileSize;
LastBase.Pos = start;
LastBase.BWalkable = true;
LastBase.TilePtr = SpawnNewTile(2, start);
MyTiles.Add(LastBase.TilePtr);
// GetWorld()->GetAuthGameMode<ARGGameMode>()->GainGold(200);
if (!LastBase.TilePtr)
{
UE_LOG(LogTemp, Error, TEXT("Failed to spawn MapTile at location: %s"), *(LastBase.Pos).ToString());
return ;
}
// GetWorld()->GetAuthGameMode<ARGGameMode>()->GainGold(200);
GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Hello, this is a debug message!"));
for (int i = 0; i < 10; i++)
{
if (CanExpand)
Expand();
}
// GetWorld()->GetAuthGameMode<ARGGameMode>()->ShowExpand();
}

void AMapTileSpawner::FillTheWallsAround(const FVector PosToSurround, const FVector PosToStayAwayFrom, const float Distance, int Depth)
{
	if (Depth == 2)
		return ;
	FVector Tester2;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	for (int i = 1; i < 7; i++)
	{
		Tester2.X = PosToSurround.X + cos(FMath::DegreesToRadians(60 * i)) * TileSize;
		Tester2.Y = PosToSurround.Y + sin(FMath::DegreesToRadians(60 * i)) * TileSize;
		if(CheckWalkable(Tester2, 1.0f) )
		{
			MyTiles.Add(SpawnNewTile(1, Tester2));
		}
	}
	if (std::rand() % 6 == 1)
		return;
	for (int i = 1; i < 7; i++)
	{
		Tester2.X = PosToSurround.X + cos(FMath::DegreesToRadians(60 * i)) * TileSize * 2;
		Tester2.Y = PosToSurround.Y + sin(FMath::DegreesToRadians(60 * i)) * TileSize * 2;
		if(CheckWalkable(Tester2, 1.0f) && FVector::Dist(Tester2,PosToStayAwayFrom) > Distance)
		{
			MyTiles.Add(SpawnNewTile(1, Tester2));
		}
	}
	if (std::rand() % 4 == 1)
		return;
	for (int i = 1; i < 7; i++)
	{
		Tester2.X = PosToSurround.X + cos(FMath::DegreesToRadians(60 * i)) * TileSize * 3;
		Tester2.Y = PosToSurround.Y + sin(FMath::DegreesToRadians(60 * i)) * TileSize * 3;
		if(CheckWalkable(Tester2, 1.0f) && FVector::Dist(Tester2,PosToStayAwayFrom) > Distance * 3)
		{
			(void)SpawnNewTile(1, Tester2);
		}
	}
	// if (std::rand() % 4 == 1)
	// 	return;
	// for (int i = 1; i < 7; i++)
	// {
	// 	Tester2.X = PosToSurround.X + cos(FMath::DegreesToRadians(60 * i)) * TileSize * 4;
	// 	Tester2.Y = PosToSurround.Y + sin(FMath::DegreesToRadians(60 * i)) * TileSize * 4;
	// 	if(CheckWalkable(Tester2, 1.0f) && FVector::Dist(Tester2,PosToStayAwayFrom) > Distance * 4)
	// 	{
	// 		MyWorld->SpawnActor<AMapTile>(BuildableTile, Tester2, FRotator(0), SpawnParams);
	// 	}
	// }
}

struct PossiblePaths
{
	int		Index;
	FVector Pos;
	float DistanceFromHome;
};

void AMapTileSpawner::Expand(void)
{
	if (MyWorld && CanExpand)
	{
		int theWay = std::rand() % 6 + 1;
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		FVector CheckTile = FirstBase.Pos;
		float Distance = TileSize; 
		int i = theWay;
		CheckTile.X = FirstBase.Pos.X + cos(FMath::DegreesToRadians(60 * i)) * Distance;
		CheckTile.Y = FirstBase.Pos.Y + sin(FMath::DegreesToRadians(60 * i)) * Distance;
		while (!CheckWalkable(CheckTile, 1.0f))
		{
			Distance += TileSize;
			CheckTile.X = FirstBase.Pos.X + cos(FMath::DegreesToRadians(60 * i)) * Distance;
			CheckTile.Y = FirstBase.Pos.Y + sin(FMath::DegreesToRadians(60 * i)) * Distance;
		}
		std::vector<PossiblePaths> PossiblePathWays;
		FVector Tester = LastBase.Pos;
		FVector Tester2 = Tester;
		for (int e = 1; e < 7; e++)
		{
			Tester2.X = Tester.X + cos(FMath::DegreesToRadians(60 * e)) * TileSize;
			Tester2.Y = Tester.Y + sin(FMath::DegreesToRadians(60 * e)) * TileSize;
			if(CheckWalkable(Tester2, 1.0f) && CheckPathable(Tester2))
			{
				PossiblePaths newPath;
				newPath.Index = e;
				newPath.Pos = Tester2;
				newPath.DistanceFromHome = FVector::Dist(Tester2, CheckTile);
				PossiblePathWays.push_back(newPath);
			}
		}
		if(PossiblePathWays.empty())
		{
			TunnelExpand();
			return ;
		}
		std::sort(PossiblePathWays.begin(), PossiblePathWays.end(), [](const struct PossiblePaths& a, const struct PossiblePaths& b) {
		return a.DistanceFromHome < b.DistanceFromHome;
		});
		if (std::rand() % 3 == 5)
			i = PossiblePathWays.back().Index;
		else
			i = PossiblePathWays.at(0).Index;
		UE_LOG(LogTemp, Error, TEXT("We got multiplier %d"), i)
		Tester.X += cos(FMath::DegreesToRadians(60 * i)) * TileSize;
		Tester.Y += sin(FMath::DegreesToRadians(60 * i)) * TileSize;
		AMapTile* TestActor = SpawnNewTile(2, Tester);
		MyTiles.Add(TestActor);
		if(!TestActor)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn MapTile at location: %s"), *(Tester).ToString())
			return ;
		}
		//UE_LOG(LogTemp, Error, TEXT("Spawned MapTile at location: %s"), *(Tester).ToString())
		FillTheWallsAround(TheLastBefore.Pos, Tester,  TileSize, 1);
		TheLastBefore = LastBase;
		LastBase.Pos = Tester;
		LastBase.TilePtr = TestActor;
		//FillTheWorld();
	}
}

void AMapTileSpawner::TunnelExpand(void)
{
	if (MyWorld && CanExpand)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		if (IsValid(LastBase.TilePtr)) LastBase.TilePtr->Destroy();
		LastBase.TilePtr = SpawnNewTile(4, LastBase.Pos);
		MyTiles.Add(LastBase.TilePtr);
		std::vector<PossiblePaths> PossiblePathWays;
		FVector Tester = LastBase.Pos;
		int Multiplier = (std::rand() % 6) + 1;
		AMapTile *TestActor = nullptr;
		bool FoundAWay = false;

		Tester.X += cos(FMath::DegreesToRadians(60 * Multiplier)) * TileSize;
		Tester.Y += sin(FMath::DegreesToRadians(60 * Multiplier)) * TileSize;
		while( FoundAWay == false)
		{
			FoundAWay = CheckWalkable(Tester, 1.0f);
			if (FoundAWay)
			{
				TestActor = SpawnNewTile(5, Tester);
				MyTiles.Add(TestActor);
				continue ;
			}
			else
			{
				Tester.Z = -2.0f;
				TestActor = SpawnNewTile(2, Tester);
				MyTiles.Add(TestActor);
			}
			Tester.X += cos(FMath::DegreesToRadians(60 * Multiplier)) * TileSize;
			Tester.Y += sin(FMath::DegreesToRadians(60 * Multiplier)) * TileSize;
			Tester.Z = LastBase.Pos.Z;
		}
		FillTheWallsAround(TheLastBefore.Pos, Tester,  TileSize, 1);
		TheLastBefore = LastBase;
		LastBase.Pos = Tester;
		LastBase.TilePtr = TestActor;
		Expand();
	}
}

void AMapTileSpawner::FillTheWorld()
{
	float Distance = TileSize;
	float WorldRadius;
	WorldRadius =  FVector::Dist(FirstBase.Pos, LastBase.Pos);
	if (WorldRadius <= TileSize)
		return ;
	WorldRadius -= TileSize;
	FVector CheckTile = FirstBase.Pos;
	CheckTile.X = 0;
	CheckTile.Y = 0;
	while (Distance <= WorldRadius)
	{
		for (int i = 1; i < 7 ; i++)
		{
			CheckTile.X = FirstBase.Pos.X + cos(FMath::DegreesToRadians(60 * i)) * Distance;
			CheckTile.Y = FirstBase.Pos.Y + sin(FMath::DegreesToRadians(60 * i)) * Distance;
			if(CheckWalkable(CheckTile, 1.0f))
				MyTiles.Add(SpawnNewTile(1, CheckTile));
		}
		Distance += TileSize;
	}
}

// Called to bind functionality to input
void AMapTileSpawner::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
