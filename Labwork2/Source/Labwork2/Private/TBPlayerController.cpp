#include "TBPlayerController.h"

ATBPlayerController::ATBPlayerController()
{
}

void ATBPlayerController::BeginPlay()
{
    bEnableClickEvents = true;
    bShowMouseCursor = true;
    ClickEventKeys.AddUnique(EKeys::RightMouseButton);
}

void ATBPlayerController::OnActorClicked(AActor* Actor, FKey key)
{
    UE_LOG(LogTemp, Warning, TEXT("OnClicked: %s %s"), *Actor->GetActorLabel(), *key.ToString());

    if (GameManager) GameManager->OnActorClicked(Actor, key);
}