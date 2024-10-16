// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/ChunkMapGenerator.h"

#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"

// Sets default values
AChunkMapGenerator::AChunkMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChunkMapGenerator::BeginPlay()
{
	Super::BeginPlay();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this; // No specific owner
	SpawnParams.Instigator = GetInstigator(); // No specific instigator
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	Chunks.Add(GetWorld()->SpawnActor<AMapChunkBase>(ChunkStartEnd, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams));
	USplineComponent *Spline = Chunks[Chunks.Num() - 1]->Spline;
	Chunks[0]->SpawnPoints();
	Head = Spline->GetLocationAtSplineInputKey(0, ESplineCoordinateSpace::World);
	// UE_LOG(LogTemp, Warning, TEXT("New HEad at location: %s"), *Head.ToString());

}

TSubclassOf<AMapChunkBase> AChunkMapGenerator::GetOptimalChunkClass(TArray<int32> Array)
{
	int32	OptimalIndex = 0;
	int32	HigestScore = 0;
	for (int32 i = 1; i < Array.Num() - 1; ++i)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Index %d Scored = %d"), i, Array[i]);

		if (Array[i] > HigestScore)
		{
			OptimalIndex = i;
			HigestScore = Array[i];
		}
		else if (Array[i] == HigestScore)
		{
			FMath::RandRange(0,1) ? OptimalIndex = i : OptimalIndex;
		}
	}
	// UE_LOG(LogTemp, Warning, TEXT("%d WON!"), OptimalIndex);

	switch (OptimalIndex)
	{
	case 1:
		return Chunks60DegreeClasses[FMath::RandRange(0, Chunks60DegreeClasses.Num() - 1)];
	case 2:
		return Chunks120DegreeClasses[FMath::RandRange(0, Chunks120DegreeClasses.Num() - 1)];
	case 3:
		return Chunks180DegreeClasses[FMath::RandRange(0, Chunks180DegreeClasses.Num() - 1)];
	case 4:
		return Chunks240DegreeClasses[FMath::RandRange(0, Chunks240DegreeClasses.Num() - 1)];
	case 5:
		return Chunks300DegreeClasses[FMath::RandRange(0, Chunks300DegreeClasses.Num() - 1)];
	default:
		return ChunksClasses[FMath::RandRange(0, ChunksClasses.Num() - 1)];
	}
}

void AChunkMapGenerator::GetLeftRightDirections(FVector& LeftDirection, FVector& RightDirection, FVector& ForwardVector)
{
	FVector ForwardDirection = ForwardVector.GetSafeNormal();

	LeftDirection = ForwardDirection.RotateAngleAxis(-60, FVector::UpVector);

	RightDirection = ForwardDirection.RotateAngleAxis(60, FVector::UpVector);

}

bool AChunkMapGenerator::CanSpawnAt(const FVector& Location)
{
	// Perform collision check at SpawnLocation
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); // Ignore the generator itself
	
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Location,
		Location, // Start and end at the same location for a stationary check
		FQuat::Identity,
		ECC_Visibility, // Adjust this to the appropriate collision channel
		FCollisionShape::MakeSphere(50.f), // Adjust the radius of the collision sphere
		CollisionParams
	);
	
	// DrawDebugSphere(GetWorld(), Location + FVector(0.f, 0.f, 50.f), 50.f, 16, FColor::Black, true, 5.f, 0, 3.f);
	
	return !bHit;
}


// Called every frame
void AChunkMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}

TArray<int32> AChunkMapGenerator::EvaluateExpandChunk(FVector ChunkOrigin, FVector	ForwardVector)
{
	TArray<int32>  Scores;
	if (!CanSpawnAt(ChunkOrigin))
	{
		return Scores;
	}
	Scores.Add(1);
	// FVector	ForwardVector = (ChunkOrigin - Head).GetSafeNormal() * HexRadius;
	// FVector	ForwardVector = (ChunkOrigin - Head).GetSafeNormal() * HexRadius;
	ForwardVector = ForwardVector * HexRadius;
	// DrawDebugSphere(GetWorld(), ChunkOrigin + FVector(0.f, 0.f, 50.f), 50.f, 16, FColor::Green, true, 2.f, 0, 3.f);
	// DrawDebugSphere(GetWorld(), ForwardVector * HexRadius + FVector(0.f, 0.f, 50.f), 50.f, 16, FColor::Yellow, true, 2.f, 0, 3.f);

	for (int32 Direction = 0; Direction < 6; ++Direction)
	{
		// FColor Color = FColor::Red;
		// switch (Direction)
		// {
		// case 0:
		// 	Color = FColor::Red;
		// 	break;
		// case 1:
		// 	Color = FColor::Yellow;
		// 	break;
		// case 2:
		// 	Color = FColor::Green;
		// 	break;
		// case 3:
		// 	Color = FColor::Black;
		// 	break;
		// case 4:
		// 	Color = FColor::Blue;
		// 	break;
		// case 5:
		// 	Color = FColor::White;
		// 	break;
		// }
		int32 Score = 0;
		//Get angel
		const FVector DirectionVector = ForwardVector.RotateAngleAxis(60 * Direction - 120, FVector::UpVector);
		//the corner position of the chunk
		FVector CornerPosition = ChunkOrigin + DirectionVector * 3;
		// DrawDebugSphere(GetWorld(), CornerPosition + FVector(0.f, 0.f, 50.f), 50.f, 16, FColor::White, true, 2.f, 0, 3.f);

		//Check if the corner can expand
		FVector LeftDirection, RightDirection;
		FVector	CurrentDirectionForwardVector = CornerPosition - ChunkOrigin;
		GetLeftRightDirections(LeftDirection, RightDirection, CurrentDirectionForwardVector);

		// DrawDebugSphere(GetWorld(), CornerPosition + FVector(0.f, 0.f, 50.f), 50.f, 16, Color, true, 2.f, 0, 3.f);

		FVector LeftOffset = FVector(HexRadius * LeftDirection.X, HexRadius * LeftDirection.Y, 0.f);
		FVector LeftSpawnLocation = CornerPosition + LeftOffset;
		Score += CanSpawnAt(LeftSpawnLocation) * 2;

		FVector RightOffset = FVector(HexRadius * RightDirection.X, HexRadius * RightDirection.Y, 0.f);
		FVector RightSpawnLocation = CornerPosition + RightOffset + LeftOffset;
		Score += CanSpawnAt(RightSpawnLocation) * 2;
		Scores.Add(Score);
		// UE_LOG(LogTemp, Warning, TEXT("Direction: %d Scored = %d"), Direction, Score);
	}
	return (Scores);
}



void AChunkMapGenerator::Expand()
{
	// SpawnChunkAtLocation(ChunkEmpty, RightSpawnLocation);
	FVector LeftDirection, RightDirection;
	FVector ForwardVector = Head - Chunks[Chunks.Num() - 1]->GetActorLocation();
	GetLeftRightDirections(LeftDirection, RightDirection, ForwardVector);
	// TSubclassOf<AMapChunkBase> CunkClass = ChunksClasses[FMath::RandRange(0, ChunksClasses.Num()-1)];

	FVector LeftOffset = FVector(HexRadius * LeftDirection.X, HexRadius * LeftDirection.Y, 0.f);
	FVector LeftSpawnLocation = Head + LeftOffset * 4;
	FVector LeftForwardVector = Head + LeftOffset * 4 - Head;
	TArray<int32>  LeftScores = EvaluateExpandChunk(LeftSpawnLocation, LeftForwardVector.GetSafeNormal());
	int32	LeftSpawnPoints = 0;
	int32 INDEX = 0;
	for (int32 Score : LeftScores)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Left: Index %d Scored = %d"), INDEX, Score);
	
		INDEX++;
		LeftSpawnPoints += Score;
	}

	FVector RightOffset = FVector(HexRadius * RightDirection.X, HexRadius * RightDirection.Y, 0.f);
	FVector RightSpawnLocation = Head + RightOffset * 4 + LeftOffset;
	FVector RightForwardVector = Head + RightOffset * 4 - Head;
	TArray<int32>  RightScores = EvaluateExpandChunk(RightSpawnLocation, RightForwardVector.GetSafeNormal());
	int32	RightSpawnPoints = 0;
	INDEX = 0;

	for (int32 Score : RightScores)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Right: Index %d Scored = %d"), INDEX, Score);
		INDEX++;

		RightSpawnPoints += Score;
	}

	if (LeftSpawnPoints == 0 && RightSpawnPoints == 0) return;;
	
	bool	bRight = static_cast<bool>(FMath::RandRange(0,1));

	//try being close
	if (!RightSpawnPoints || (LeftSpawnPoints && LeftSpawnPoints < RightSpawnPoints)) bRight = false;
	else if (!LeftSpawnPoints || (RightSpawnPoints && LeftSpawnPoints > RightSpawnPoints)) bRight = true;
	//
	//Avoid being close
	// if (LeftSpawnPoints > RightSpawnPoints) bRight = false;
	// else if (LeftSpawnPoints < RightSpawnPoints) bRight = true;

	
	// UE_LOG(LogTemp, Error, TEXT("Left: %d, right %d, Right %d"), LeftSpawnPoints, RightSpawnPoints, bRight);
	// UE_LOG(LogTemp, Error, TEXT("ArrayRight: %d, ArrayLeft %d"), RightScores.Num(), LeftScores.Num());


	
	if (!bRight)
	{
		if (AMapChunkBase *newChunk = SpawnChunkAtLocation(GetOptimalChunkClass(LeftScores), LeftSpawnLocation))
		{
			Chunks.Add(newChunk);
			++MaxChunkID;
			bNewChunk = true;
			if (RotateChunk(Head + LeftDirection * HexRadius))
			{
				Chunks[Chunks.Num() - 1]->SpawnPoints();
			}
			if (RightSpawnPoints)
			{
				// AMapChunkBase *newEmptyChunk =SpawnChunkAtLocation(ChunkEmpty, RightSpawnLocation);
				// newEmptyChunk->SpawnPoints();
			}
		}
	}
	else
	{
		if (AMapChunkBase *newChunk = SpawnChunkAtLocation(GetOptimalChunkClass(RightScores), RightSpawnLocation))
		{
			Chunks.Add(newChunk);
			++MaxChunkID;
			bNewChunk = true;
			if (RotateChunk(Head + RightDirection * HexRadius))
			{
				Chunks[Chunks.Num() - 1]->SpawnPoints();
			}
			if (RightSpawnPoints)
			{
				// AMapChunkBase *newEmptyChunk =SpawnChunkAtLocation(ChunkEmpty, LeftSpawnLocation);
				// newEmptyChunk->SpawnPoints();
			}
		}
	}

	// // //Update head
	if (bNewChunk)
	{
		bNewChunk = false;
		USplineComponent *Spline = Chunks[Chunks.Num() - 1]->Spline;
		Spline->AddSplinePoint(Head, ESplineCoordinateSpace::World);
		Head = Spline->GetLocationAtSplineInputKey(0, ESplineCoordinateSpace::World);
		// UE_LOG(LogTemp, Warning, TEXT("New HEad at location: %s"), *Head.ToString());
	}
}

FVector AChunkMapGenerator::GetEndNode()
{
	return Head;
}

AMapChunkBase* AChunkMapGenerator::SpawnChunkAtLocation(TSubclassOf<AMapChunkBase> CunkClass, const FVector& Location)
{
	// Your logic to spawn a chunk at the specified location
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this; // No specific owner
	SpawnParams.Instigator = GetInstigator(); // No specific instigator
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	return (GetWorld()->SpawnActor<AMapChunkBase>(CunkClass, Location, FRotator::ZeroRotator, SpawnParams));

}



bool AChunkMapGenerator::RotateChunk(FVector SpawnLocation)
{
	//Update rotation of chunk
	int32 Point = 0;
	while (Point < 6)
	{
		FRotator Rotate(0,Point * 60,0);
		Chunks[Chunks.Num() - 1]->RotateChunk(Rotate);
		// UE_LOG(LogTemp, Warning, TEXT("Point: %d, Angel: %f"), Point, Rotate.Yaw);

		UBoxComponent *BoxConnector = Chunks[Chunks.Num() - 1]->BoxConnector;
		// Get the box extents and transform
		FVector BoxExtent = BoxConnector->GetScaledBoxExtent();
		FTransform BoxTransform = BoxConnector->GetComponentTransform();

		// Transform the location to the box's local space
		FVector LocalLocation = BoxTransform.InverseTransformPosition(SpawnLocation);

		// Check if the local location is within the box extents
		bool bIsWithinBox = (FMath::Abs(LocalLocation.X) <= BoxExtent.X &&
			FMath::Abs(LocalLocation.Y) <= BoxExtent.Y &&
			FMath::Abs(LocalLocation.Z) <= BoxExtent.Z);
		if (bIsWithinBox)
		{
			return true;
		}
		else
		{
			Point++;
		}
	}
	return false;
}
