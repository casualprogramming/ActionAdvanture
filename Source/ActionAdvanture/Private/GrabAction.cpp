// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabAction.h"
#include "CommonUtils.h"
#include "DrawDebugHelpers.h"

void UGrabAction::Initialize(UActionSystemComponent* ActionSystemComponent)
{
	Super::Initialize(ActionSystemComponent);
	OwnerMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();	condition(OwnerMesh);
	OwnerAnim = OwnerMesh->GetAnimInstance(); condition(OwnerAnim);
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
	UKismetSystemLibrary::PrintString(GetOwner(),"hello");

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
	condition4(OwnerAnim, GrabAnim, GrabbedActor, OwnerMesh);
	UPrimitiveComponent* Collider = GrabbedActor->FindComponentByClass<UPrimitiveComponent>();
	condition(Collider);
	Collider->SetMobility(EComponentMobility::Movable);
	Collider->SetSimulatePhysics(false);

	//Owner
	OwnerAnim->Montage_Play(GrabAnim);
	GrabbedActor->AttachToComponent(OwnerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, GrabSocketNameInOwner);
}

void UGrabAction::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);
	//TODO: StopAction: Throw Action. Cancel Action.
	condition2(GrabStopAnim, GrabbedActor);
	OwnerAnim->Montage_Play(GrabStopAnim);
	GrabbedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	GrabbedActor->FindComponentByClass<UPrimitiveComponent>()->SetSimulatePhysics(true);
	GrabbedActor = nullptr;
}

