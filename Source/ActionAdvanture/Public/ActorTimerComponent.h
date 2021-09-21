// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"
#include "ActorTimerComponent.generated.h"


//TODO: Declare the timer in the actor class. I made ActorTimerComponent temporarily to avoid modifying the actor class.
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONADVANTURE_API UActorTimerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorTimerComponent();
	TUniquePtr<FTimerManager> ActorTimerManager;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	inline FTimerManager& GetActorTimerManager() const { return *ActorTimerManager.Get(); }

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Actor Timer by Event", ScriptName = "SetActorTimerDelegate", AdvancedDisplay = "InitialStartDelay, InitialStartDelayVariance"), Category = "Utilities|ActorTime")
	FTimerHandle K2_SetTimerDelegate(UPARAM(DisplayName = "Event") FTimerDynamicDelegate Delegate, float Time, bool bLooping, float InitialStartDelay = 0.f, float InitialStartDelayVariance = 0.f);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Clear Actor Timer by Handle", ScriptName = "ActorTimerHandle"), Category = "Utilities|ActorTime")
	void K2_ClearTimerHandle(FTimerHandle Handle);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Actor Timer Elapsed Time by Handle", ScriptName = "GetActorTimerElapsedTimeHandle"), Category = "Utilities|ActorTime")
	float K2_GetTimerElapsedTimeHandle(FTimerHandle Handle);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Actor Timer Remaining Time by Handle", ScriptName = "GetActorTimerRemainingTimeHandle"), Category = "Utilities|ActorTime")
	float K2_GetTimerRemainingTimeHandle(FTimerHandle Handle);

};
