// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_GetFurthestEnemy.h"

#include "AI/RogueAIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Buildings/Tower.h"
#include "Enemy/EnemyBase.h"

void UBTService_GetFurthestEnemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const ARogueAIController *AIController = Cast<ARogueAIController>(OwnerComp.GetOwner());
	const ATower *Tower = Cast<ATower>(AIController->GetPawn());
	if (!Tower)
	{
		UE_LOG(LogTemp, Error, TEXT("UBTService_GetFurthestEnemy::TickNode: Only works with ATower"));
		return;
	}
	float	FurthestDistance = TNumericLimits<float>::Max();
	AActor	*FurthestActor = nullptr;
	
	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams	SphereParams;
	FCollisionObjectQueryParams ObjectQueryParams(FCollisionObjectQueryParams::InitType::AllObjects);

	// Modify ObjectQueryParams to only include the Pawn channel
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		Tower->GetActorLocation(),
		FQuat::Identity,
		ObjectQueryParams,
		FCollisionShape::MakeSphere(Tower->BuildingInfo.AttackRange),
		SphereParams
	);
	// GetWorld()->OverlapMultiByObjectType(Overlaps, Tower->GetActorLocation(), FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Tower->TowerRange), SphereParams);
	// DrawDebugSphere(GetWorld(), OwnerComp.GetOwner()->GetActorLocation(), Tower->TowerRange, 16, FColor::Red, false, 4.f);
	for (FOverlapResult &Overlap : Overlaps)
	{
		AEnemyBase *Enemy = Cast<AEnemyBase>(Overlap.GetActor());
		if (IsValid(Enemy) && Enemy->MaxHealth > Enemy->PreDamage)
		{
			const float Distance = Enemy->DistanceToReachBase - Enemy->DistanceTraveled;
			if (Distance < FurthestDistance)
			{
				FurthestDistance = Distance;
				FurthestActor = Enemy;
			}
			// const int32 ChunkID = Enemy->CurrentChunkIndex;
			// if (ChunkID < ClosestChunkID)
			// {
			// 	FurthestDistance = 0.f;
			// }
			// if (ChunkID <= ClosestChunkID && Distance > FurthestDistance)
			// {
			// 	ClosestChunkID = ChunkID;
			// 	FurthestDistance = Distance;
			// 	FurthestActor = Enemy;
			// }
		}
	}

	// UE_LOG(LogTemp, Error, TEXT("attacking INDEX: %d"), ClosestChunkID);




	
	// TArray<AActor*>	Enemies;
	//
	// UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyBase::StaticClass(), Enemies);
	//
	// // float	FurthestDistance = -1.f;
	// // AActor	*FurthestActor = nullptr;
	// for (AActor *Actor : Enemies)
	// {
	// 	AEnemyBase *Enemy = Cast<AEnemyBase>(Actor);
	// 	if (IsValid(Enemy))
	// 	{
	// 		const float Distance = Enemy->DistanceTraveled;
	// 		if (Distance > FurthestDistance)
	// 		{
	// 			FurthestDistance = Distance;
	// 			FurthestActor = Enemy;
	// 		}
	// 	}
	// }
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetEnemy, FurthestActor);
}
