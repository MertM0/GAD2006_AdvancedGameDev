// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class ADVANCEDGAMEDEV_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AGD")
	bool bDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AGD")
	float Health;

	UPROPERTY(EditAnywhere, Category = "AGD")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AGD")
	float HealingRate;

	UPROPERTY(EditAnywhere, Category = "AGD")
	float WalkSpeed;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};