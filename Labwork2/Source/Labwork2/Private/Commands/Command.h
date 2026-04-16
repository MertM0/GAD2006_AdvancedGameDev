#pragma once

#include "CoreMinimal.h"
#include "GameSlot.h"

class Command
{
public:
    Command() {};
    virtual ~Command() {};

    virtual void Execute() = 0;
    virtual void Revert() = 0;
    virtual void Update(float DeltaTime) {};
    virtual bool IsExecuting() { return false; };
};