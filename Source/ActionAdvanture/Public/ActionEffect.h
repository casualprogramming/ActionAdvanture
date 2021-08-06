// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "ActionEffect.generated.h"

class UAction;
/**
 * 
 */
UCLASS()
class ACTIONADVANTURE_API UActionEffect : public UAction
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActionEffect")
	float Duration;

	/* Time between 'ticks' to apply effect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActionEffect")
	float Period;

	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;

	float TimeStarted;
	void TimerStop(AActor* Instigator);
public:
	
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;

	UFUNCTION(BlueprintNativeEvent, Category = "ActionEffect")
	void ExecutePeriodicEffect(AActor* Instigator);

public:

	UFUNCTION(BlueprintCallable, Category = "ActionEffect")
	float GetTimeRemaining() const;

};
