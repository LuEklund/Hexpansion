#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapTile.generated.h"

UCLASS()
class RGPROTOTYPE_API AMapTile : public AActor
{
	GENERATED_BODY()
    
public:    
	// Sets default values for this actor's properties
	AMapTile();

	// Property to hold the size of the side
	float SideLength;
	float TileHeight;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:    
	void CalculateSideLength();
	void CalculateHeight();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

private:
};
