// Fill out your copyright notice in the Description page of Project Settings.


#include "CooldownAction.h"
#include "CommonUtils.h"
#include "ActorTimerComponent.h"

void UCooldownAction::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	condition(IsValid(GetWorld()));
	if (bActorTimer) condition(GetOwner()->FindComponentByClass<UActorTimerComponent>());
	TimeStarted = GetWorld()->GetTimeSeconds();
	auto& TimerManager = bActorTimer? GetOwner()->FindComponentByClass<UActorTimerComponent>()->GetActorTimerManager() : GetWorld()->GetTimerManager();
	
	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &UCooldownAction::CommitStopAction, Instigator, false);

		TimerManager.SetTimer(DurationHandle, Delegate, Duration, false);
	}
}

void UCooldownAction::StopAction_Implementation(AActor* Instigator, bool bCancel)
{
	Super::StopAction_Implementation(Instigator, bCancel);
	condition(IsValid(GetWorld()));
	auto& TimerManager = bActorTimer ? GetOwner()->FindComponentByClass<UActorTimerComponent>()->GetActorTimerManager() : GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(DurationHandle);
}

void UCooldownAction::AddCooldownDuration(float CooldownDuration, bool InActorTimer)
{
	if (CooldownDuration != 0.0f)
	{
		CommitStopAction(nullptr, false);
		bActorTimer = InActorTimer;
		Duration = GetTimeRemaining() + CooldownDuration;
		CommitStartAction(nullptr);
	}
}

void UCooldownAction::StartCooldownDuration(float CooldownDuration, bool InActorTimer)
{
	if (CooldownDuration != 0.0f)
	{
		CommitStopAction(nullptr, false);
		bActorTimer = InActorTimer;
		Duration = CooldownDuration;
		CommitStartAction(nullptr);
	}
}

float UCooldownAction::GetTimeRemaining() const
{
	condition_return(IsValid(GetWorld()),0.0f);
	auto& TimerManager = bActorTimer ? GetOwner()->FindComponentByClass<UActorTimerComponent>()->GetActorTimerManager() : GetWorld()->GetTimerManager();
	if (!IsRunning())
		return 0.0f;
	else
		return TimerManager.GetTimerRemaining(DurationHandle);
}

