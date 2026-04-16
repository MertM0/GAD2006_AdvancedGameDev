#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameGrid.generated.h"

class AGameSlot;

USTRUCT(Blueprintable)
struct FSGridPosition
{
    GENERATED_USTRUCT_BODY();

    FSGridPosition() {}
    FSGridPosition(int col, int row) : Col(col), Row(row) {}

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    uint8 Col;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    uint8 Row;
};

UCLASS()
class AGameGrid : public AActor
{
    GENERATED_BODY()

public:
    AGameGrid();
    virtual void OnConstruction(const FTransform& Transform) override;

    UPROPERTY(EditAnywhere)
    TSubclassOf<AGameSlot> GridClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int NumRows;

    UPROPERTY(EditAnywhere)
    int NumCols;

public:
    virtual void Tick(float DeltaTime) override;

    AGameSlot* GetSlot(FSGridPosition& Position);
    static AGameSlot* FindSlot(FSGridPosition Position);

private:
    static AGameGrid* GameGrid;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    TArray<UChildActorComponent*> GridActors;
};