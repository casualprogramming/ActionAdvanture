// Fill out your copyright notice in the Description page of Project Settings.


#include "TelekinesisAction.h"
#include "ActionSystemComponent.h"
#include "CommonUtils.h"
#include "DrawDebugHelpers.h"
#include "ExplosionComponent.h"

UTelekinesisAction::UTelekinesisAction()
{
	ActionName = "TelekinesisAction";
}

bool UTelekinesisAction::CanStart_Implementation(AActor* Instigator)
{
	if (!Super::CanStart_Implementation(Instigator)) return false;

	if (IsValid(HitActor)) return false;

	FVector const Direction = GetOwner()->GetActorForwardVector();
	FVector const Start = GetOwner()->GetActorLocation();
	FVector const End = Start + Direction * FindRange;

	FHitResult hit; FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());
	GetOwner()->GetWorld()->SweepSingleByObjectType(hit, Start, End, FQuat::Identity, ECollisionChannel::ECC_PhysicsBody, FCollisionShape::MakeSphere(Radius), params);

	DrawDebugCapsule(GetOwner()->GetWorld(), (Start + End) * 0.5f, (End - Start).Size() * 0.5f, Radius, FRotationMatrix::MakeFromZ(End - Start).ToQuat(), hit.GetActor() ? FColor::Green : FColor::Red, false, 0.5f);

	HitActor = hit.GetActor();
	if (!IsValid(HitActor)) return false;

	HitActorCollider = HitActor->FindComponentByClass<UPrimitiveComponent>();
	return true;
}


void UTelekinesisAction::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	GetOwnerActionSystem()->GetEventDelegate("Telekinesis").AddDynamic(this, &UTelekinesisAction::GrabEvent);
	HitActor->OnDestroyed.AddDynamic(this, &UTelekinesisAction::OnHitActorDestroyedDoCancelAction);
}

void UTelekinesisAction::GrabEvent(AActor* Instigator)
{
	condition2(HitActor, HitActorCollider);

	HitActorCollider->SetMobility(EComponentMobility::Movable);
	HitActorCollider->SetSimulatePhysics(false);

	//Play MoveToSocket
	FTimerDelegate Delegate;
	Delegate.BindWeakLambda(this, [=]() {
		FVector SocketLocation = GetOwnerMesh()->GetSocketLocation(GrabSocketNameInOwner);
		//if ((HitActor->GetActorLocation() - SocketLocation).Size() < AttachThreshold)
		//{
			//HitActor->AttachToComponent(GetOwnerMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, GrabSocketNameInOwner);
			//return;
		//}
		//move to socket
		//else
		//{
			HitActor->SetActorLocation(FMath::Lerp(HitActor->GetActorLocation(), SocketLocation, GrabSpeed));
		//}
		});
	//TODO: Make sure the action supports ticks to use instead of timers.
	GetWorld()->GetTimerManager().SetTimer(MoveToSocketHandle, Delegate, 0.016f, true);

}

void UTelekinesisAction::StopAction_Implementation(AActor* Instigator, bool bCancel)
{
	Super::StopAction_Implementation(Instigator, bCancel);
	GetWorld()->GetTimerManager().ClearTimer(MoveToSocketHandle);
	HitActor->OnDestroyed.RemoveDynamic(this, &UTelekinesisAction::OnHitActorDestroyedDoCancelAction);
	GetOwnerActionSystem()->GetEventDelegate("Telekinesis").RemoveDynamic(this, &UTelekinesisAction::GrabEvent);

	//if (HitActor->IsAttachedTo(GetOwner()))
	//{
	//	HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	//}
	HitActorCollider->SetSimulatePhysics(true);

	//Stop
	if (bCancel == false)
	{
		//launch Hit actor
		HitActorCollider->AddImpulse(GetOwner()->GetActorForwardVector() * LaunchForce);
		UExplosionComponent* Explosion = HitActor->FindComponentByClass<UExplosionComponent>();
		if (Explosion)
		{
			Explosion->ActivateCollisionListener();
		}
	}

	HitActor = nullptr;
}