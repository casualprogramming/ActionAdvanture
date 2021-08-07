// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEffect.h"
#include "ActionSystemComponent.h"
#include "CommonUtils.h"


void UActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	TimeStarted = GetOwner()->GetWorld()->GetTimeSeconds();
	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &UActionEffect::CommitStopAction, Instigator, false);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}

	if (Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void UActionEffect::StopAction_Implementation(AActor* Instigator, bool bCancel)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}

	Super::StopAction_Implementation(Instigator, bCancel);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
}

float UActionEffect::GetTimeRemaining() const
{
	if (!IsRunning()) 
		return 0.0f;
	else
	{
		float EndTime = TimeStarted + Duration;
		float CurrentTime = GetOwner()->GetWorld()->GetTimeSeconds();
		return EndTime - CurrentTime;
	}
}


void UActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{

}
