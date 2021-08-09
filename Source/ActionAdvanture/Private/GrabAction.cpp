// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabAction.h"
#include "CommonUtils.h"
#include "DrawDebugHelpers.h"


UGrabAction::UGrabAction()
{
	ActionName = "GrabAction";
}

bool UGrabAction::CanStart_Implementation(AActor* Instigator)
{
	if (!Super::CanStart_Implementation(Instigator)) return false;

	if (IsValid(GrabbedActor)) return false;

	FVector const Direction = GetOwner()->GetActorForwardVector();
	FVector const Start = GetOwner()->GetActorLocation();
	FVector const End = Start + Direction * GrabRange;

	FHitResult hit; FCollisionQueryParams params; float const Radius = 100.0f;
	params.AddIgnoredActor(GetOwner());
	GetOwner()->GetWorld()->SweepSingleByObjectType(hit, Start, End, FQuat::Identity, ECollisionChannel::ECC_PhysicsBody, FCollisionShape::MakeSphere(Radius), params);

	DrawDebugCapsule(GetOwner()->GetWorld(), (Start + End) * 0.5f, (End - Start).Size() * 0.5f, Radius, FRotationMatrix::MakeFromZ(End - Start).ToQuat(), hit.GetActor()? FColor::Green: FColor::Red, false, 0.5f);

	GrabbedActor = hit.GetActor();
	if (!IsValid(GrabbedActor)) return false;

	//UGrabAndThrowListenerComponent* Listener = GrabbedActor->FindComponentByClass<UGrabAndThrowListenerComponent>();
	//if (!IsValid(Listener)) { GrabbedActor = nullptr; return; }//this will not happen. If you are only tracing objects with UGrabAndThrowListenerComponent.
	
	return true;
}

void UGrabAction::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	condition(GrabbedActor);
	UPrimitiveComponent* Collider = GrabbedActor->FindComponentByClass<UPrimitiveComponent>();
	condition(Collider);
	Collider->SetMobility(EComponentMobility::Movable);
	Collider->SetSimulatePhysics(false);

	GrabbedActor->AttachToComponent(GetOwnerMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, GrabSocketNameInOwner);
}


//case Stop: Throw Action calls GrabAction::StopAction  (bCancle = false)
//case Cancel: Montage cancel or Action cancel
void UGrabAction::StopAction_Implementation(AActor* Instigator, bool bCancel)
{
	Super::StopAction_Implementation(Instigator, bCancel);

	condition(GrabbedActor);
	if (bCancel)
	{
		GrabbedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		GrabbedActor->FindComponentByClass<UPrimitiveComponent>()->SetSimulatePhysics(true);
	}
	GrabbedActor = nullptr;
}

