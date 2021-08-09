// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MontageAction.h"
#include "ThrowAction.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONADVANTURE_API UThrowAction : public UMontageAction
{
	GENERATED_BODY()
private:
	UPROPERTY()
	AActor* GrabbedActor;

	UPROPERTY(EditAnywhere, Category = "Throw")
	float ThrowForce;

	UPROPERTY(EditAnywhere, Category = "Throw")
	FVector ThrowDirection;
	
	UPROPERTY(EditAnywhere, Category = "Throw")
	float Delay;

	FTimerHandle ThrowTimeHandle;

public:
	UThrowAction();
	virtual void Initialize(UActionSystemComponent* ActionSystemComponent) override;
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator, bool bCancel) override;
	virtual void OnThrow();
};
