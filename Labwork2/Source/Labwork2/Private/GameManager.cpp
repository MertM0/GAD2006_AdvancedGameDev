#include "GameManager.h"
#include "TBPlayerController.h"
#include "Commands/MoveCommand.h"
#include "GameGrid.h"
#include "UnitBase.h"

AGameManager::AGameManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AGameManager::BeginPlay()
{
    Super::BeginPlay();

    if (auto PlayerController = GWorld->GetFirstPlayerController<ATBPlayerController>())
    {
        PlayerController->GameManager = this;
    }

    if (Levels.IsValidIndex(CurrentLevel))
    {
        CreateLevelActors(Levels[CurrentLevel]);
    }
}

void AGameManager::CreateLevelActors(FSLevelInfo& Info)
{
    ThePlayer = nullptr;

    for (auto UnitInfo : Info.Units)
    {
        if (AGameSlot* Slot = GameGrid->GetSlot(UnitInfo.StartPosition))
        {
            Slot->SpawnUnitHere(UnitInfo.UnitClass);

            if (Slot->Unit && Slot->Unit->IsControlledByThePlayer()) {
                ThePlayer = Slot->Unit;
            }
        }
    }
}

void AGameManager::OnActorClicked(AActor* Actor, FKey button)
{
    if (CurrentCommand.IsValid() && CurrentCommand->IsExecuting()) return;

    AGameSlot* Slot = Cast<AGameSlot>(Actor);

    if (!Slot) return;

    if (!ThePlayer) {
        UE_LOG(LogTemp, Error, TEXT("No Player Unit Detected!"));
        return;
    }

    if (Slot->Unit == nullptr)
    {
        TSharedRef<MoveCommand> Cmd = MakeShared<MoveCommand>(ThePlayer->Slot->GridPosition, Slot->GridPosition);

        CommandPool.Add(Cmd);
        Cmd->Execute();

        CurrentCommand = Cmd;
    }
}

bool AGameManager::UndoLastMove()
{
    if (CommandPool.IsEmpty())
    {
        return false;
    }

    TSharedRef<Command> LastCommand = CommandPool.Pop();

    LastCommand->Revert();

    return true;
}

void AGameManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (CurrentCommand.IsValid()) {
        CurrentCommand->Update(DeltaTime);
    }
}