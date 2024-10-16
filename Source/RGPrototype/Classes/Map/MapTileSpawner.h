#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MapTile.h"
#include "MapTileSpawner.generated.h"

class USplineComponent;

USTRUCT()
struct FMapNode
{
    GENERATED_BODY()
    FVector Pos;
    bool	BBuildable;
    bool	BWalkable;
    UPROPERTY()
    TObjectPtr<AMapTile> TilePtr;
    FMapNode()
    {
        Pos = FVector(0);
        BBuildable = false;
        BWalkable = false;
    }
};

UCLASS()
class RGPROTOTYPE_API AMapTileSpawner : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    AMapTileSpawner();
    float TileSize;
    float TileSideSize;
    bool CanExpand = true;
    int LastChosen = 0;
    FMapNode LastBase;
    FMapNode FirstBase;
    TArray<AMapTile *> MyTiles;
    FMapNode TheLastBefore;
    size_t mapMid = 0;
    UPROPERTY()
    UWorld* MyWorld;

    //Path for enemies
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USplineComponent>	Spline;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:    
    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
    USceneComponent* MeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WalkableSpawn")
    TSubclassOf<AMapTile> WalkableTile;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TunnelTileSpawn")
    TSubclassOf<AMapTile> TunnelTile;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuildableSpawn")
    TSubclassOf<AMapTile> BuildableTile;

    FTimerHandle TimerHandle;
    bool CheckWalkable(const FVector& Location, float Radius);
    bool CheckPathable(const FVector& Location);
    void Expand(void);
    void TunnelExpand(void);
    void FillTheWorld(void);
    AMapTile *SpawnNewTile(int Type, FVector Location);
    void FillTheWallsAround(const FVector PosToSurround, const FVector PosToStayAwayFrom, const float Distance, int Depth);
};
