// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ABaseCharacter::ABaseCharacter()
	: bDead(false),
	Health(100.0f),
	MaxHealth(100.0f),
	HealingRate(0.0f),
	WalkSpeed(300.0f)
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bDead) {
		Health = FMath::Min(MaxHealth, Health + HealingRate * DeltaTime);
	}
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}