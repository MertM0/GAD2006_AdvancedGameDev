#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TilePlayerController.generated.h"

class ATileGameManager;

UCLASS()
class ATilePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	ATileGameManager* GameManager;
};