// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "AvatarCharacter.generated.h"

/**
 * */
UCLASS()
class AAvatarCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AAvatarCharacter();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	UCameraComponent* mCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	USpringArmComponent* mSpringArm;

private:
	void RunPressed();
	void RunReleased();

	void UpdateMovementParams();

	UPROPERTY(EditAnywhere, Category = "AGD")
	float RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	float StaminaGainRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	float StaminaDrainRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	bool bStaminaDrained;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	bool bRunning;
};