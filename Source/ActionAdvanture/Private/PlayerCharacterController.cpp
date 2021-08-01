// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"
#include "GameFramework/Character.h"

void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &APlayerCharacterController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerCharacterController::MoveRight);
	InputComponent->BindAxis("Turn", this, &APlayerCharacterController::Turn);
	InputComponent->BindAxis("LookUp", this, &APlayerCharacterController::LookUp);
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &APlayerCharacterController::Jump);
}

void APlayerCharacterController::MoveForward(float Value)
{
	if (!IsValid(GetPawn())) return;

	FRotator ControlRot = GetControlRotation();
	//We only care about the z-axis
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	GetPawn()->AddMovementInput(ControlRot.Vector(), Value);
}

void APlayerCharacterController::MoveRight(float Value)
{
	if (!IsValid(GetPawn())) return;

	FVector RightVector = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y);//UKismetMathLibrary::GetRightVector

	GetPawn()->AddMovementInput(RightVector, Value);
}
void APlayerCharacterController::Turn(float Value)
{
	if (!IsValid(GetPawn())) return;

	GetPawn()->AddControllerYawInput(Value);
}
void APlayerCharacterController::LookUp(float Value)
{
	if (!IsValid(GetPawn())) return;

	GetPawn()->AddControllerPitchInput(Value);
}

void APlayerCharacterController::Jump()
{
	if (!IsValid(GetCharacter())) return;

	GetCharacter()->Jump();
}