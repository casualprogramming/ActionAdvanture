// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONADVANTURE_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()
	

protected:
	void SetupInputComponent() override;

	//Common control functions for Third Person Camera (TPS)
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void Jump();
};
