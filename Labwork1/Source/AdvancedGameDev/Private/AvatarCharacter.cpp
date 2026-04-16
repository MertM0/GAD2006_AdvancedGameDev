// Fill out your copyright notice in the Description page of Project Settings.

#include "AvatarCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AAvatarCharacter::AAvatarCharacter()
{
	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	mSpringArm->TargetArmLength = 300.0f;
	mSpringArm->SetupAttachment(RootComponent);

	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	mCamera->SetupAttachment(mSpringArm, USpringArmComponent::SocketName);

	mCamera->bUsePawnControlRotation = false;
	mSpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;

	RunSpeed = 600.0f;

	MaxStamina = 100.0f;
	Stamina = MaxStamina;
	StaminaDrainRate = 20.0f;
	StaminaGainRate = 15.0f;
	bStaminaDrained = false;
	bRunning = false;
}

void AAvatarCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AAvatarCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool bIsMoving = GetVelocity().SizeSquared() > 0.0f;
	bool bIsWalkingMode = GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking;

	if (bRunning && bIsMoving && bIsWalkingMode && !bStaminaDrained)
	{
		Stamina -= StaminaDrainRate * DeltaTime;

		if (Stamina <= 0.0f)
		{
			Stamina = 0.0f;
			bStaminaDrained = true;
			UpdateMovementParams();
		}
	}
	else
	{
		if (Stamina < MaxStamina)
		{
			Stamina += StaminaGainRate * DeltaTime;

			if (Stamina >= MaxStamina)
			{
				Stamina = MaxStamina;

				if (bStaminaDrained)
				{
					bStaminaDrained = false;
					UpdateMovementParams();
				}
			}
		}
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::Yellow, FString::Printf(TEXT("Stamina: %d / %d"), FMath::RoundToInt(Stamina), FMath::RoundToInt(MaxStamina)));
		if (bStaminaDrained)
		{
			GEngine->AddOnScreenDebugMessage(2, 0.0f, FColor::Red, TEXT("STAMINA DRAINED!"));
		}
	}
}

void AAvatarCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ACharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("MoveForward", this, &AAvatarCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAvatarCharacter::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AAvatarCharacter::RunPressed);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AAvatarCharacter::RunReleased);
}

void AAvatarCharacter::MoveForward(float Value)
{
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0, Rotation.Yaw, 0);
	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, Value);
}

void AAvatarCharacter::MoveRight(float Value)
{
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0, Rotation.Yaw, 0);
	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, Value);
}

void AAvatarCharacter::RunPressed()
{
	bRunning = true;
	UpdateMovementParams();
}

void AAvatarCharacter::RunReleased()
{
	bRunning = false;
	UpdateMovementParams();
}

void AAvatarCharacter::UpdateMovementParams()
{
	if (bRunning && !bStaminaDrained)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}