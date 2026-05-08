#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NetBaseCharacter.h"
#include "NetGameInstance.generated.h"

UCLASS()
class UNetGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void Host(FString MapName, FSPlayerInfo Info);

    UFUNCTION(BlueprintCallable)
    void Join(FString Address, FSPlayerInfo Info);

    UPROPERTY(BlueprintReadWrite)
    FSPlayerInfo PlayerInfo;
};