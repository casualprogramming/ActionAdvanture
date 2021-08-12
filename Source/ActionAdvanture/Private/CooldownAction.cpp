// Fill out your copyright notice in the Description page of Project Settings.


#include "CooldownAction.h"
#include "CommonUtils.h"

void UCooldownAction::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	condition(IsValid(GetWorld()));
	TimeStarted = GetWorld()->GetTimeSeconds();
	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &UCooldownAction::CommitStopAction, Instigator, false);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}
}

void UCooldownAction::StopAction_Implementation(AActor* Instigator, bool bCancel)
{
	Super::StopAction_Implementation(Instigator, bCancel);
	condition(IsValid(GetWorld()));
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
}

void UCooldownAction::AddCooldownDuration(float CooldownDuration)
{
	if (CooldownDuration != 0.0f)
	{
		CommitStopAction(nullptr, false);
		Duration = GetTimeRemaining() + CooldownDuration;
		CommitStartAction(nullptr);
	}
}

void UCooldownAction::StartCooldownDuration(float CooldownDuration)
{
	if (CooldownDuration != 0.0f)
	{
		CommitStopAction(nullptr, false);
		Duration = CooldownDuration;
		CommitStartAction(nullptr);
	}
}

float UCooldownAction::GetTimeRemaining() const
{
	condition_return(IsValid(GetWorld()),0.0f);
	if (!IsRunning())
		return 0.0f;
	else
	{
		float EndTime = TimeStarted + Duration;
		float CurrentTime = GetOwner()->GetWorld()->GetTimeSeconds();
		return EndTime - CurrentTime;
	}
}

