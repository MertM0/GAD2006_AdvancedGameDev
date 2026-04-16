#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameSlot.h"
#include "UnitBase.generated.h"

class AGameSlot;

UCLASS()
class AUnitBase : public AActor
{
    GENERATED_BODY()

public:
    AUnitBase();

    void AssignToSlot(AGameSlot* NewSlot);

    UFUNCTION(BlueprintImplementableEvent, BlueprintPure)
    bool IsControlledByThePlayer();

    UPROPERTY(EditAnywhere)
    FVector StartOffset;

    UPROPERTY(VisibleAnywhere)
    AGameSlot* Slot;

protected:
    virtual void BeginPlay() override;
};