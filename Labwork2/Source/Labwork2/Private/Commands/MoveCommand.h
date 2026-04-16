#pragma once

#include "CoreMinimal.h"
#include "Command.h"

class MoveCommand : public Command
{
public:
    MoveCommand(FSGridPosition Src, FSGridPosition Dst);
    ~MoveCommand();

    virtual void Execute() override;
    virtual void Revert() override;

private:
    FSGridPosition Source, Destination;
};