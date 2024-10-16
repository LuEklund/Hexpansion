// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TowerShoot.h"

#include "AI/RogueAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Buildings/Tower.h"
#include "Enemy/EnemyBase.h"

EBTNodeResult::Type UBTTask_TowerShoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	UObject *Object = OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey());
	AEnemyBase *Enemy = Cast<AEnemyBase>(Object);
	if (!Enemy)
	{
		return (EBTNodeResult::Succeeded);
	}
	else
	{
		//Get Tower Ai Controller
		if (ARogueAIController const *Controller = Cast<ARogueAIController>(OwnerComp.GetOwner()))
		{
			//get Tower pawn
			if (ATower *Tower = Cast<ATower>(Controller->GetPawn()))
			{
				// DrawDebugSphere(GetWorld(), Tower->GetActorLocation(), Tower->BuildingInfo.AttackRange, 32, FColor::Red, false, 1.f);

				Tower->Fire(Enemy);
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(HaveShotEnemy.SelectedKeyName, true);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return (EBTNodeResult::Succeeded);
			}
		}
	}
	return (EBTNodeResult::Failed);
}
