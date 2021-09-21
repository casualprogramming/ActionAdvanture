// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorTimerComponent.h"

// Sets default values for this component's properties
UActorTimerComponent::UActorTimerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	ActorTimerManager = MakeUnique<FTimerManager>();
	// ...
}


// Called when the game starts
void UActorTimerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UActorTimerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	GetActorTimerManager().Tick(DeltaTime);
	// ...
}

FTimerHandle UActorTimerComponent::K2_SetTimerDelegate(FTimerDynamicDelegate Delegate, float Time, bool bLooping, float InitialStartDelay, float InitialStartDelayVariance)
{
	FTimerHandle Handle;
	if (Delegate.IsBound())
	{
		//const UWorld* const World = GEngine->GetWorldFromContextObject(Delegate.GetUObject(), EGetWorldErrorMode::LogAndReturnNull);
		
		InitialStartDelay += FMath::RandRange(-InitialStartDelayVariance, InitialStartDelayVariance);
		if (Time <= 0.f || (Time + InitialStartDelay) < 0.f)
		{
			FString ObjectName = GetNameSafe(Delegate.GetUObject());
			FString FunctionName = Delegate.GetFunctionName().ToString();
			FFrame::KismetExecutionMessage(*FString::Printf(TEXT("%s %s SetTimer passed a negative or zero time. The associated timer may fail to be created/fire! If using InitialStartDelayVariance, be sure it is smaller than (Time + InitialStartDelay)."), *ObjectName, *FunctionName), ELogVerbosity::Warning);
		}

		FTimerManager& TimerManager = GetActorTimerManager();
		Handle = TimerManager.K2_FindDynamicTimerHandle(Delegate);
		TimerManager.SetTimer(Handle, Delegate, Time, bLooping, (Time + InitialStartDelay));
	}
	else
	{
		UE_LOG(LogBlueprintUserMessages, Warning,
			TEXT("SetTimer passed a bad function (%s) or object (%s)"),
			*Delegate.GetFunctionName().ToString(), *GetNameSafe(Delegate.GetUObject()));
	}

	return Handle;
}

void UActorTimerComponent::K2_ClearTimerHandle(FTimerHandle Handle)
{
	if (Handle.IsValid())
	{
		GetActorTimerManager().ClearTimer(Handle);
	}
}

float UActorTimerComponent::K2_GetTimerElapsedTimeHandle(FTimerHandle Handle)
{
	float ElapsedTime = 0.f;
	if (Handle.IsValid())
	{
		ElapsedTime = GetActorTimerManager().GetTimerElapsed(Handle);
	}

	return ElapsedTime;
}

float UActorTimerComponent::K2_GetTimerRemainingTimeHandle(FTimerHandle Handle)
{
	float RemainingTime = 0.f;
	if (Handle.IsValid())
	{
		RemainingTime = GetActorTimerManager().GetTimerRemaining(Handle);
	}

	return RemainingTime;
}