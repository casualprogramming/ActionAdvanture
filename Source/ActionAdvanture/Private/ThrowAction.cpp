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

bool UThrowAction::CanStart_Implementation(AActor* Instigator)
{
	if(!Super::CanStart_Implementation(Instigator)) return false;
	UGrabAction* GrabAction = GetOwnerActionSystem()->GetAction<UGrabAction>();
	conditionb(GrabAction);
	GrabbedActor = GrabAction->GetGrabbedActor();
	return IsValid(GrabbedActor);
}

void UThrowAction::StartAction_Implementation(AActor* Instigator)
{
	//Get GrabbedActor from GrabAction and stop GrabAction. Prevents the throw action montage from canceling the grab ation montage. 
	UGrabAction* GrabAction = GetOwnerActionSystem()->GetAction<UGrabAction>();
	condition(GrabAction);
	GrabAction->CommitStopAction(GetOwner(), false);
	
	Super::StartAction_Implementation(Instigator);
	
	GetOwnerActionSystem()->GetEventDelegate("Throw").AddDynamic(this, &UThrowAction::OnThrow);
	GrabbedActor->OnDestroyed.AddDynamic(this, &UThrowAction::OnGrabbedActorDestroyedDoCancelAction);
}

void UThrowAction::OnThrow(AActor* Instigator)
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
	GetOwnerActionSystem()->GetEventDelegate("Throw").RemoveDynamic(this, &UThrowAction::OnThrow);
	condition(GrabbedActor);//The grabbed actor cannot be null because the action cancel is called before being destroyed.
	GrabbedActor->OnDestroyed.RemoveDynamic(this, &UThrowAction::OnGrabbedActorDestroyedDoCancelAction);

	if (bCancel)
	{
		GrabbedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		GrabbedActor->FindComponentByClass<UPrimitiveComponent>()->SetSimulatePhysics(true);
	}

	GrabbedActor = nullptr;
}


