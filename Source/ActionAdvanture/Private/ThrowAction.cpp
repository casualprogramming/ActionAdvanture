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
	//RequiredTags.AddTag("GrabAction");
}

void UThrowAction::Initialize(UActionSystemComponent* ActionSystemComponent)
{
	Super::Initialize(ActionSystemComponent);
	OwnerMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();	condition(OwnerMesh);
	OwnerAnim = OwnerMesh->GetAnimInstance(); condition(OwnerAnim);

	
	CommitStopDelegate.BindUFunction(this, "CommitStopAction");
}

void UThrowAction::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	condition3(OwnerAnim, ThrowMontage, OwnerMesh);
	
	//Get GrabbedActor from GrabAction and stop GrabAction
	UGrabAction* GrabAction = GetOwnerActionSystem()->GetAction<UGrabAction>();
	condition(GrabAction);
	GrabbedActor = GrabAction->GetGrabbedActor();
	condition(GrabbedActor);
	GrabAction->CommitStopAction(GetOwner(), false);

	//Play Throw
	OwnerAnim->Montage_Play(ThrowMontage);
	OwnerAnim->Montage_SetEndDelegate(CommitStopDelegate, ThrowMontage);
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
	GrabbedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	GrabbedActor->FindComponentByClass<UPrimitiveComponent>()->SetSimulatePhysics(true);

	//Add Throw Force
	FVector Direction = ThrowDirection.Rotation().Quaternion() * GetOwner()->GetActorForwardVector();
	GrabbedActor->FindComponentByClass<UPrimitiveComponent>()->AddImpulse(Direction * ThrowForce);

	GrabbedActor = nullptr;
}

void UThrowAction::OnMontageCancel()
{
	//OwnerAnim->PlayMontage(CancelAnim);
}

void UThrowAction::StopAction_Implementation(AActor* Instigator, bool bCancel)
{
	Super::StopAction_Implementation(Instigator, bCancel);
	GetWorld()->GetTimerManager().ClearTimer(ThrowTimeHandle);
	GrabbedActor = nullptr;
}


