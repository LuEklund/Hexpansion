// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_ShootAoE.h"

#include "AI/RogueAIController.h"
#include "Buildings/Tower.h"
#include "Enemy/EnemyBase.h"


EBTNodeResult::Type UBTTask_ShootAoE::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);



	const ARogueAIController *AIController = Cast<ARogueAIController>(OwnerComp.GetOwner());
	ATower *Tower = Cast<ATower>(AIController->GetPawn());
	if (!Tower)
	{
		UE_LOG(LogTemp, Error, TEXT("UBTServiceGetNearby_BlueprintBase::TickNode: Only works with ATower"));
		return EBTNodeResult::Failed;
	}
	
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
		if (IsValid(Enemy) && Enemy->GetActorLocation().Z >= 0.f)
		{
			Tower->Fire(Enemy);
		}
	}
	return (EBTNodeResult::Succeeded);
}
