// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Nexus.generated.h"

class ARGGameMode;

UCLASS()
class RGPROTOTYPE_API ANexus : public AActor
{
	GENERATED_BODY()
	
public:	
	ANexus();
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent>	NexusMeshComponent;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void	BeginOverlap(UPrimitiveComponent *OnComponentBeginOverlap,
							AActor* OtherActor,
							UPrimitiveComponent* OtherComp,
							int32 OtherBodyIndex, bool
							bFromSweep, const
							FHitResult & SweepResult);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<ARGGameMode>	GameMode;

};
