#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameGrid.h"
#include "Components/BoxComponent.h"
#include "GameSlot.generated.h"

class AUnitBase;

UENUM(Blueprintable)
enum EGridState
{
    GS_Default,
    GS_Highlighted,
    GS_Offensive,
    GS_Supportive,
};

UCLASS()
class AGameSlot : public AActor
{
    GENERATED_BODY()

public:
    AGameSlot();

    UPROPERTY(EditAnywhere)
    UBoxComponent* Box;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* Plane;

    UPROPERTY(BlueprintReadWrite)
    FSGridPosition GridPosition;

    EGridState GridState;

    UFUNCTION()
    void SetState(EGridState NewState);

    void SpawnUnitHere(TSubclassOf<AUnitBase>& UnitClass);

    UPROPERTY(VisibleAnywhere)
    AUnitBase* Unit;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnGridClicked(AActor* TouchedActor, FKey ButtonPressed);

public:
    virtual void Tick(float DeltaTime) override;
};