// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "CooldownAction.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONADVANTURE_API UCooldownAction : public UAction
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActionEffect")
	float Duration;

	FTimerHandle DurationHandle;

	float TimeStarted;
public:

	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator, bool bCancel) override;
	void AddCooldownDuration(float CooldownDuration);
	void StartCooldownDuration(float CooldownDuration);
	float GetTimeRemaining() const;
};
