// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RogueAIController.generated.h"

class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class RGPROTOTYPE_API ARogueAIController : public AAIController
{
	GENERATED_BODY()

public:
	ARogueAIController();
	
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent>	BehaviorTreeComponent;
};
