// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowAction.h"
#include "GrabAction.h"
#include "CommonUtils.h"
#include "ActionSystemComponent.h"

UThrowAction::UThrowAction()
{
	ActionName = "ThrowAction";
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
	
	OwnerAnim->Montage_Play(ThrowMontage);
	

	OwnerAnim->OnMontageEnded.Add(CommitStopDelegate);

	if (ThrowTime > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &UThrowAction::OnThrow);
		GetWorld()->GetTimerManager().SetTimer(ThrowTimeHandle, Delegate, ThrowTime, false);
	}
	else
		OnThrow();
}

void UThrowAction::OnThrow()
{
	UGrabAction* GrabAction = GetOwnerActionSystem()->GetAction<UGrabAction>();
	condition(GrabAction);
	AActor* GrabbedActor = GrabAction->GetGrabbedActor();
	
	GrabAction->CommitStopAction(GetOwner(),false);
	GrabbedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	GrabbedActor->FindComponentByClass<UPrimitiveComponent>()->SetSimulatePhysics(true);
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
	OwnerAnim->OnMontageEnded.Remove(CommitStopDelegate);
}


