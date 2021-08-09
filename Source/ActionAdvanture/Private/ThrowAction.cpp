// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowAction.h"
#include "GrabAction.h"
#include "CommonUtils.h"
#include "ActionSystemComponent.h"
#include "Components/PrimitiveComponent.h"

UThrowAction::UThrowAction()
{
	ActionName = "ThrowAction";
	ThrowDirection = FVector(1, 1, 0);
	ThrowForce = 100;
	//RequiredTags.AddTag("Grab");
}

void UThrowAction::Initialize(UActionSystemComponent* ActionSystemComponent)
{
	Super::Initialize(ActionSystemComponent);
}

void UThrowAction::StartAction_Implementation(AActor* Instigator)
{
	//Get GrabbedActor from GrabAction and stop GrabAction. Prevents the throw action montage from canceling the grab ation montage. 
	UGrabAction* GrabAction = GetOwnerActionSystem()->GetAction<UGrabAction>();
	condition(GrabAction);
	GrabbedActor = GrabAction->GetGrabbedActor();
	condition(GrabbedActor);
	GrabAction->CommitStopAction(GetOwner(), false);
	
	Super::StartAction_Implementation(Instigator);
	
	if (Delay > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &UThrowAction::OnThrow);
		GetWorld()->GetTimerManager().SetTimer(ThrowTimeHandle, Delegate, Delay, false);
	}
	else
		OnThrow();
}

void UThrowAction::OnThrow()
{
	if (IsValid(GrabbedActor))
	{
		GrabbedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		GrabbedActor->FindComponentByClass<UPrimitiveComponent>()->SetSimulatePhysics(true);

		//Add Throw Force
		FVector Direction = ThrowDirection.Rotation().Quaternion() * GetOwner()->GetActorForwardVector();
		GrabbedActor->FindComponentByClass<UPrimitiveComponent>()->AddImpulse(Direction * ThrowForce);
	}
}

void UThrowAction::StopAction_Implementation(AActor* Instigator, bool bCancel)
{
	Super::StopAction_Implementation(Instigator, bCancel);
	GetWorld()->GetTimerManager().ClearTimer(ThrowTimeHandle);

	if (bCancel)
	{
		if(IsValid(GrabbedActor))
			GrabbedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			GrabbedActor->FindComponentByClass<UPrimitiveComponent>()->SetSimulatePhysics(true);
	}

	GrabbedActor = nullptr;
}


